#include <fstream>
#include <filesystem>

#include <zip.h>

#include "../CommonLib/Logging/Log.h"
#include "../CommonLib/Str.h"

#include "LibzipArchive.h"

namespace
{
    std::string GetZipError(int err)
    {
        zip_error_t error;
        zip_error_init_with_code(&error, err);
        
        std::string errotStr = zip_error_strerror(&error);

        zip_error_fini(&error);

        return errotStr;
    }
}

using namespace archive;

CLibzipArchive::~CLibzipArchive()
{
    if (m_pArchive != NULL)
        if (int errCode = zip_close(m_pArchive); errCode != 0)
        {
            const std::string error = GetZipError(errCode);
            LOG.Write("Failed to close archive [{}] with error [{}]", m_archivePath, error);
        }
}

std::shared_ptr<IArchive> CLibzipArchive::Open4Read(const std::string& path)
{
    zip_t* pArhive;
    int errCode;

    if (pArhive = zip_open(path.c_str(), ZIP_RDONLY, &errCode);  pArhive == NULL)
    {
        const std::string error = GetZipError(errCode);
        throw std::exception(str::Format("Failed to open archive [{}] with error [{}]", path, error).c_str());
    }

    return std::shared_ptr<IArchive>(new CLibzipArchive(pArhive, path));
}

std::shared_ptr<IArchive> CLibzipArchive::Open4Read(const std::wstring& path)
{
    return Open4Read(str::WstringToUtf8String(path));
}

std::shared_ptr<IArchive> CLibzipArchive::Open4Write(const std::string& path)
{
    zip_t* pArhive;
    int errCode;

    if (pArhive = zip_open(path.c_str(), ZIP_CREATE | ZIP_EXCL, &errCode);  pArhive == NULL)
    {
        const std::string error = GetZipError(errCode);
        throw std::exception(str::Format("Failed to open archive [{}] with error [{}]", path, error).c_str());
    }

    return std::shared_ptr<IArchive>(new CLibzipArchive(pArhive, path));
}

std::vector<std::string> CLibzipArchive::EnumEntries() const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    const zip_int64_t entriesNum = zip_get_num_entries(m_pArchive, 0);

    std::vector<std::string> result;
    result.reserve(entriesNum);

    for (zip_int64_t i = 0; i < entriesNum; ++i)
    {
        const char* name = zip_get_name(m_pArchive, i, 0);

        if (name == NULL)
            throw std::exception("Failed to get name of entrie");

        result.emplace_back(name);
    }

    return result;
}

std::string CLibzipArchive::ReadFileToStr(const std::string& fileName) const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    const uint64_t fileSize = GetFileSize(fileName);

    zip_file* file = zip_fopen(m_pArchive, fileName.c_str(), 0);

    if (file == NULL)
        throw std::exception(str::Format("Failed to open file [{}] with error [{}]", fileName, zip_strerror(m_pArchive)).c_str());

    std::string content;
    content.resize(fileSize);

    zip_int64_t bytes_read;

    bytes_read = zip_fread(file, content.data(), content.size());

    if (bytes_read == -1)
    {
        zip_fclose(file);
        throw std::exception(str::Format("Failed to read file [{}] with error [{}]", fileName, zip_strerror(m_pArchive)).c_str());
    }

    zip_fclose(file);

    return content;
}

uint64_t CLibzipArchive::GetFileSize(const std::string& fileName) const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    struct zip_stat st;
    zip_stat_init(&st);

    if (zip_stat(m_pArchive, fileName.c_str(), 0, &st) != 0)
        throw std::exception(str::Format("Failed to get size of file [{}] with error [{}]", fileName, zip_strerror(m_pArchive)).c_str());

    return st.size;
}

void CLibzipArchive::ExtractFile(const std::string& fileName, const std::wstring& targetPath) const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    std::ofstream ostream(targetPath, std::ios::binary);
    ReadFileToStream(fileName, ostream);
    ostream.close();
}

void CLibzipArchive::ExtractFile(const std::string& fileName, const std::string& targetPath) const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    std::ofstream ostream(targetPath, std::ios::binary);
    ReadFileToStream(fileName, ostream);
    ostream.close();
}

void CLibzipArchive::AddFileToArchive(const std::string& sourceFilePath)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    zip_source_t* source = zip_source_file(m_pArchive, sourceFilePath.c_str(), 0, 0);

    if (source == NULL)
        throw std::exception(str::Format("Failed to open source file [{}] with error [{}]", sourceFilePath, zip_strerror(m_pArchive)).c_str());

    zip_int64_t idx = zip_file_add(m_pArchive, std::filesystem::path(sourceFilePath).filename().string().c_str(), source, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8);

    if (idx < 0)
    {
        zip_source_free(source);
        throw std::exception(str::Format("Failed to add file [{}] to archive with error [{}]", sourceFilePath, zip_strerror(m_pArchive)).c_str());
    }

    if (zip_set_file_compression(m_pArchive, idx, ZIP_CM_DEFLATE, 9) < 0)
        throw std::exception(str::Format("Error setting compression level [{}]", sourceFilePath, zip_strerror(m_pArchive)).c_str());
}

CLibzipArchive::CLibzipArchive(zip_t* pArchive, const std::string& path)
    : m_pArchive(pArchive)
    , m_archivePath(path)
{
}

void CLibzipArchive::ReadFileToStream(const std::string& fileName, std::ostream& stream) const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    const uint64_t fileSize = GetFileSize(fileName);

    zip_file* file = zip_fopen(m_pArchive, fileName.c_str(), 0);

    if (file == NULL)
        throw std::exception(str::Format("Failed to open file [{}] with error [{}]", fileName, zip_strerror(m_pArchive)).c_str());

    constexpr size_t bufferSize = 64 * 1024;
    char buffer[64 * 1024];

    zip_int64_t bytesRead;

    while ((bytesRead = zip_fread(file, buffer, bufferSize)) > 0)
    {
        stream.write(buffer, bytesRead);
        if (stream.fail())
        {
            zip_fclose(file);
            throw std::exception(str::Format("Failed to write file [{}] to stream", fileName).c_str());
        }
    }

    if (bytesRead < 0)
    {
        zip_fclose(file);
        throw std::exception(str::Format("Failed to read file [{}] with error [{}]", fileName, zip_strerror(m_pArchive)).c_str());
    }

    zip_fclose(file);
}
