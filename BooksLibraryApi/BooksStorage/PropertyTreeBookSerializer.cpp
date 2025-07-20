#include "../../CommonLib/Str.h"

#include "PropertyTreeBookSerializer.h"

using namespace bookslibrary;

void CPropertyTreeBookSerializer::Serialize(const std::shared_ptr<SBook>& ptrBook, boost::property_tree::ptree& ptBook)
{
	ptBook.add<std::string>("title", str::WstringToUtf8String(ptrBook->m_title));
	ptBook.add<uint64_t>("id", ptrBook->m_id);
	ptBook.add<uint32_t>("lang", static_cast<uint32_t>(ptrBook->m_lang));
	ptBook.add<std::string>("keys", str::WstringToUtf8String(ptrBook->m_keyWords));

	{
		boost::property_tree::ptree ptAuthors;

		for (const SAuthor& author : ptrBook->m_authors)
		{
			boost::property_tree::ptree ptAuthor;

			ptAuthor.add<std::string>("author", str::WstringToUtf8String(author.m_author));
			ptAuthor.add<uint64_t>("id", author.m_id);

			ptAuthors.push_back(std::make_pair("", ptAuthor));
		}

		ptBook.add_child("authors", ptAuthors);
	}

	{
		boost::property_tree::ptree ptGenres;

		for (const std::string& genre : ptrBook->m_genres)
		{
			boost::property_tree::ptree ptGenre;
			ptGenre.add<std::string>("genre", genre);

			ptGenres.push_back(std::make_pair("", ptGenre));
		}

		ptBook.add_child("genres", ptGenres);
	}

	if (ptrBook->m_series.has_value())
	{
		boost::property_tree::ptree ptSeries;

		ptSeries.add<std::string>("name", str::WstringToUtf8String(ptrBook->m_series.value().m_series));
		ptSeries.add<uint64_t>("num", ptrBook->m_series.value().m_serialNumber);

		ptBook.add_child("series", ptSeries);
	}

	{
		boost::property_tree::ptree ptFile;

		ptFile.add<std::string>("file", ptrBook->m_file.m_fileName);
		ptFile.add<uint64_t>("size", ptrBook->m_file.m_size);
		ptFile.add<std::string>("archive", ptrBook->m_file.m_archiveFileName);

		ptBook.add_child("file", ptFile);
	}
}

std::shared_ptr<SBook> CPropertyTreeBookSerializer::Deserialize(const boost::property_tree::ptree& ptBook)
{
	std::shared_ptr<SBook> ptrBook = std::make_shared<SBook>();

	ptrBook->m_title = str::Utf8StringToWString(ptBook.get<std::string>("title"));
	ptrBook->m_id = ptBook.get<uint64_t>("id");
	ptrBook->m_lang = static_cast<ELang>(ptBook.get<uint32_t>("lang"));
	ptrBook->m_keyWords = str::Utf8StringToWString(ptBook.get<std::string>("keys"));

	{
		const boost::property_tree::ptree& ptAuthors = ptBook.get_child("authors");

		for (const auto&[emty_name, ptAuthor] : ptAuthors)
		{
			SAuthor author;

			author.m_author = str::Utf8StringToWString(ptAuthor.get<std::string>("author"));
			author.m_id = ptAuthor.get<uint64_t>("id");

			ptrBook->m_authors.push_back(std::move(author));
		}
	}

	{
		const boost::property_tree::ptree& ptGenres = ptBook.get_child("genres");

		for (const auto& [emty_name, ptGenre] : ptGenres)
			ptrBook->m_genres.push_back(ptGenre.get<std::string>("genre"));
	}


	{
		auto optPtSeries = ptBook.get_child_optional("series");

		if (optPtSeries.has_value())
		{
			const boost::property_tree::ptree& ptSeries = optPtSeries.value();

			SBookSeries series;

			series.m_series = str::Utf8StringToWString(ptSeries.get<std::string>("name"));
			series.m_serialNumber = ptSeries.get<uint64_t>("num");

			ptrBook->m_series = std::move(series);
		}
	}

	{
		const boost::property_tree::ptree& ptFile = ptBook.get_child("file");

		ptrBook->m_file.m_fileName = ptFile.get<std::string>("file");
		ptrBook->m_file.m_size = ptFile.get<uint64_t>("size");
		ptrBook->m_file.m_archiveFileName = ptFile.get<std::string>("archive");
	}

	return ptrBook;
}
