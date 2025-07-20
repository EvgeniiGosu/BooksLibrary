#include <filesystem>

#include "../CommonLib/Str.h"

#include "TempDir.h"

using namespace util;

CTempDir::~CTempDir()
{
	if (m_dirPath)
		std::filesystem::remove_all(*m_dirPath);
}

std::string CTempDir::GetFilePath(const std::string& fileName)
{
	CheckAndCreateDir();
	return ((*m_dirPath)/fileName).string();
}

std::string util::CTempDir::GetDirPath()
{
	CheckAndCreateDir();
	return m_dirPath->string();
}

std::wstring util::CTempDir::GetWFilePath(const std::wstring& fileName)
{
	CheckAndCreateDir();
	return ((*m_dirPath) / fileName).wstring();
}

std::wstring util::CTempDir::GetWDirPath()
{
	CheckAndCreateDir();
	return m_dirPath->wstring();
}

void util::CTempDir::CheckAndCreateDir()
{
	if (!m_dirPath)
	{
		m_dirPath = std::make_shared<std::filesystem::path>(std::filesystem::temp_directory_path() / str::GetRandom(32));
		std::filesystem::create_directory(*m_dirPath);
	}
}
