#include "../../CommonLib/Logging/Log.h"
#include "../../CommonLib/Str.h"

#include "BooksParser.h"

using namespace bookslibrary;

namespace {

	std::vector<std::string> ParseMultiTokensOption(const std::string_view& option)
	{
		const std::vector<std::string_view> authors = str::Split(option, ':', true);
		return std::vector<std::string>(authors.cbegin(), authors.cend());
	}
}

std::vector<std::shared_ptr<SBook>> CBooksParser::Parse(const std::string& inpFileContent, const std::string& inpFileName)
{
	LOG.Write("Parse books from [{}]", inpFileName);

	std::vector<std::shared_ptr<SBook>> books;

	const std::string_view textView(inpFileContent);

	size_t start = 0;
	size_t end	 = textView.find('\n', start);

	while (end != std::string_view::npos)
	{
		const std::string_view line = textView.substr(start, end - start);

		start = end + 1;
		end	  = textView.find('\n', start);

		if (line.empty())
			continue;

		const std::vector<std::string_view> bookOptions = str::Split(line, L'\x04', false);

		if (!bookOptions[8].empty() && str::ToUint64(bookOptions[8]) > 0) // skip deleted book
			continue;
		
		std::string lang;
		lang = bookOptions[11];

		if (m_allowedLanguages.find(lang) == m_allowedLanguages.cend()) // skip unnecessary languages
			continue;

		std::shared_ptr<SBook> ptrBook = std::make_shared<SBook>();

		for (const std::string& authorStr : ParseMultiTokensOption(bookOptions[0]))
		{
			SAuthor author;
			author.m_author = str::Utf8StringToWString(authorStr);
			author.m_id = GerOrCreateAuthorId(author.m_author);
			ptrBook->m_authors.push_back(std::move(author));
		}

		ptrBook->m_genres	= ParseMultiTokensOption(bookOptions[1]);
		ptrBook->m_title	= str::Utf8StringToWString(bookOptions[2]);

		if (!bookOptions[3].empty())
		{
			SBookSeries series;

			series.m_series = str::Utf8StringToWString(bookOptions[3]);
			if (!bookOptions[4].empty())
				series.m_serialNumber = str::ToUint64(bookOptions[4]);

			ptrBook->m_series = series;
		}
		ptrBook->m_file.m_fileName			= std::filesystem::path(bookOptions[5]).replace_extension(bookOptions[9]).string();
		ptrBook->m_file.m_size				= str::ToUint64(bookOptions[6]);
		ptrBook->m_file.m_archiveFileName	= std::filesystem::path(inpFileName).replace_extension(".zip").string();

		ptrBook->m_id		= str::ToUint64(bookOptions[7]);
		ptrBook->m_lang		= FromString(lang);
		ptrBook->m_keyWords = str::Utf8StringToWString(bookOptions[13]);

		books.push_back(std::move(ptrBook));
	}

	LOG.Write("Parsed {} books", std::to_string(books.size()));

	return books;
}

uint64_t CBooksParser::GerOrCreateAuthorId(const std::wstring& author)
{
	const auto it = m_AuthorsId.find(author);

	if (it != m_AuthorsId.cend())
		return it->second;

	m_lastAuthorId++;
	m_AuthorsId.emplace(author, m_lastAuthorId);

	return m_lastAuthorId;
}
