#include <filesystem>

#include "../CommonLib/Logging/Log.h"
#include "../CommonLib/Str.h"
#include "../CommonLib/Stopwatch.h"

#include "../ArchiveLib/ArchiveFactory.h"

#include "BooksStorage/PropertyTreeBooksStorageWriter.h"
#include "BooksStorage/PropertyTreeBooksStorageReader.h"

#include "InpxParsing/InpxParser.h"

#include "BooksLibraryApi.h"

using namespace bookslibrary;

std::shared_ptr<CBooksLibraryApi> CBooksLibraryApi::Get()
{
	static std::shared_ptr<CBooksLibraryApi> ptrApi(new CBooksLibraryApi);
	return ptrApi;
}

void CBooksLibraryApi::ImportNewLib(const std::wstring& inpxPath, const std::wstring& booksDirPath, const std::wstring& storagePath)
{
	LOG.Write("CBooksLibraryApi::ImportNewLib: inpx: \"{}\", books: \"{}\", storage: \"{}\"",
		str::WstringToUtf8String(inpxPath),
		str::WstringToUtf8String(booksDirPath),
		str::WstringToUtf8String(storagePath));

	{
		CInpxParser booksParser(inpxPath, booksDirPath);

		const std::shared_ptr<IBooksStorageWriter> ptrBooksStorageWriter = CPropertyTreeBooksStorageWriter::CreateNew(storagePath);
		booksParser.Parse(ptrBooksStorageWriter);
	}

	OpenExistingLib(booksDirPath, storagePath);
}

void CBooksLibraryApi::OpenExistingLib(const std::wstring& booksDirPath, const std::wstring& storagePath)
{
	LOG.Write("CBooksLibraryApi::OpenExistingLib: books: \"{}\", storage: \"{}\"",
		str::WstringToUtf8String(booksDirPath),
		str::WstringToUtf8String(storagePath));

	m_ptrBooksStorage = CPropertyTreeBooksStorageReader::CreateNew(storagePath);
	m_booksDir = booksDirPath;
}

std::vector<std::shared_ptr<SBook>> CBooksLibraryApi::SearchByPartialTitle(const std::wstring& title) const
{
	LOG.Write("CBooksLibraryApi::SearchByPartialTitle: title: \"{}\"", str::WstringToUtf8String(title));

	return m_ptrBooksStorage->SearchByPartialTitle(title);
}

void CBooksLibraryApi::ExportBook(uint64_t id, const std::wstring& dirPath) const
{
	LOG.Write("CBooksLibraryApi::ExportBook: to dir: \"{}\"", str::WstringToUtf8String(dirPath));

	util::CLogStopWatch stopWatch("Export the book took {} seconds");

	const std::shared_ptr<SBook> ptrBook = m_ptrBooksStorage->GetBook(id);

	const std::filesystem::path archivePath = std::filesystem::path(m_booksDir)/ptrBook->m_file.m_archiveFileName;
	const std::filesystem::path fileTargetPath = std::filesystem::path(dirPath)/ptrBook->m_file.m_fileName;

	const std::shared_ptr<archive::IArchive> ptrArchive = archive::CArchiveFactory::Open4Read(archivePath.wstring());

	ptrArchive->ExtractFile(ptrBook->m_file.m_fileName, fileTargetPath.wstring());
}

std::vector<std::shared_ptr<SBook>> CBooksLibraryApi::GetAllBooks() const
{
	LOG.Write("CBooksLibraryApi::GetAllBooks");

	return m_ptrBooksStorage->GetAllBooks();
}

CBooksLibraryApi::CBooksLibraryApi()
{
}
