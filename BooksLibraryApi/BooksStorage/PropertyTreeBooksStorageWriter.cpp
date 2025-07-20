#include <algorithm>
#include <iterator>
#include <filesystem>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "../../CommonLib/Str.h"
#include "../../CommonLib/Logging/Log.h"

#include "PropertyTreeBookSerializer.h"
#include "PropertyTreeBooksStorageWriter.h"

using namespace bookslibrary;

std::shared_ptr<IBooksStorageWriter> CPropertyTreeBooksStorageWriter::CreateNew(const std::wstring& storagePath)
{
	if (std::filesystem::exists(storagePath))
		throw std::exception(str::Format("Storage [{}] already exist", str::WstringToUtf8String(storagePath)).c_str());

	return std::shared_ptr<IBooksStorageWriter>(new CPropertyTreeBooksStorageWriter(storagePath));
}

void CPropertyTreeBooksStorageWriter::CPropertyTreeBooksStorageWriter::AddNewBooks(std::vector<std::shared_ptr<SBook>>&& books)
{
	for (const std::shared_ptr<SBook>& ptrBook : books)
	{
		boost::property_tree::ptree ptBook;

		CPropertyTreeBookSerializer::Serialize(ptrBook, ptBook);

		boost::property_tree::json_parser::write_json(m_stream, ptBook, false);
		m_stream << std::endl;
	}
}

CPropertyTreeBooksStorageWriter::CPropertyTreeBooksStorageWriter(const std::wstring& path)
	: m_stream(path)
{
}
