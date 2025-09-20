#include <fstream>
#include <filesystem>

#include <boost/property_tree/json_parser.hpp>

#include "../../CommonLib/Str.h"
#include "../../CommonLib/Logging/Log.h"
#include "../../CommonLib/Stopwatch.h"
#include "../../ArchiveLib/ArchiveFactory.h"

#include "PropertyTreeBookSerializer.h"
#include "PropertyTreeBooksStorageReader.h"

using namespace bookslibrary;

std::shared_ptr<IBooksStorageReader> CPropertyTreeBooksStorageReader::CreateNew(const std::wstring& storagePath)
{
	util::CLogStopWatch stopWatch("Reading the storage took {} seconds");

	if (!std::filesystem::exists(storagePath))
		throw std::exception(str::Format("Storage [{}] not exist", str::WstringToUtf8String(storagePath)).c_str());

	std::vector<std::shared_ptr<SBook>> books;
	
	std::stringstream stream(archive::CArchiveFactory::Open4Read(str::WstringToUtf8String(storagePath))->ReadFileToStr(std::filesystem::path(storagePath).filename().string()));

	std::string line;
	while (std::getline(stream, line))
	{
		if (line.empty())
			break;

		boost::property_tree::ptree ptBook;

		std::stringstream lineStream(line);
		boost::property_tree::json_parser::read_json(lineStream, ptBook);

		books.push_back(CPropertyTreeBookSerializer::Deserialize(ptBook));
	}

	LOG.Write("Total of {} books were processed", std::to_string(books.size()));

	return std::shared_ptr<IBooksStorageReader>(new CPropertyTreeBooksStorageReader(std::move(books)));
}

std::vector<std::shared_ptr<SBook>> CPropertyTreeBooksStorageReader::GetAllBooks() const
{
	return m_books;
}

std::vector<std::shared_ptr<SBook>> CPropertyTreeBooksStorageReader::SearchByPartialTitle(const std::wstring& title) const
{
	std::vector<std::shared_ptr<SBook>> result;

	for (const std::shared_ptr<SBook>& ptrBook : m_books)
		if (ptrBook->m_title.find(title) != std::string::npos)
			result.push_back(ptrBook);

	return result;
}

std::shared_ptr<SBook> CPropertyTreeBooksStorageReader::GetBook(uint64_t id) const
{
	const auto it = std::find_if(m_books.cbegin(), m_books.cend(), [id](const std::shared_ptr<SBook>& ptrBook)
		{
			return ptrBook->m_id == id;
		});

	if (it == m_books.cend())
		throw std::exception(str::Format("Book with id [{}] not found", std::to_string(id)).c_str());

	return *it;
}

CPropertyTreeBooksStorageReader::CPropertyTreeBooksStorageReader(std::vector<std::shared_ptr<SBook>>&& books)
	: m_books(std::move(books))
{
}
