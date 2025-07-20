#include "../../CommonLib/Logging/Log.h"
#include "../../CommonLib/Stopwatch.h"

#include "../../ArchiveLib/ArchiveFactory.h"

#include "BooksParser.h"
#include "InpxParser.h"

using namespace bookslibrary;

CInpxParser::CInpxParser(const std::wstring& inpxPath, const std::wstring& booksDirPath)
	: m_inpxPath(inpxPath)
	, m_booksDirPath(booksDirPath)
{
}

uint64_t CInpxParser::Parse(const std::shared_ptr<IBooksStorageWriter>& ptrBooksStorage)
{
	LOG.Write("Parse inpx");

	util::CLogStopWatch stopWatch("Import took {} seconds");

	CBooksParser booksParser;

	const std::shared_ptr<archive::IArchive> ptrInpxArchive = archive::CArchiveFactory::Open4Read(m_inpxPath);

	const std::vector<std::string> allFiles = ptrInpxArchive->EnumEntries();

	uint64_t booksCount = 0;

	for (const std::string& file : allFiles)
	{
		if (std::filesystem::path(file).extension().string() != ".inp")
			continue;

		const std::string fileContent = ptrInpxArchive->ReadFileToStr(file);

		std::vector<std::shared_ptr<SBook>> books = booksParser.Parse(fileContent, file);
		booksCount += books.size();

		ptrBooksStorage->AddNewBooks(std::move(books));
	}

	LOG.Write("Total of {} books were processed", std::to_string(booksCount));

	return booksCount;
}
