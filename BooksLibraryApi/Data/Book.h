#pragma once

#include <string>
#include <optional>
#include <vector>

#include "BookFile.h"
#include "BookSeries.h"
#include "Author.h"
#include "Lang.h"

namespace bookslibrary
{
	struct SBook
	{
		std::vector<SAuthor>		m_authors;
		std::vector<std::string>	m_genres;
		std::wstring				m_title;
		uint64_t					m_id = 0;
		ELang						m_lang;
		std::wstring				m_keyWords;
		std::optional<SBookSeries>	m_series;
		SBookFile					m_file;

		bool operator==(const SBook& other) const;
	};
}