#include "Book.h"

using namespace bookslibrary;

bool SBook::operator==(const SBook& other) const
{
	if (m_authors.size() != other.m_authors.size())
		return false;

	if (!std::equal(m_authors.cbegin(), m_authors.cend(), other.m_authors.cbegin()))
		return false;

	if (m_genres.size() != other.m_genres.size())
		return false;

	if (!std::equal(m_genres.cbegin(), m_genres.cend(), other.m_genres.cbegin()))
		return false;

	if (m_title != other.m_title)
		return false;

	if (m_id != other.m_id)
		return false;

	if (m_keyWords != other.m_keyWords)
		return false;

	if (m_series != other.m_series)
		return false;

	if (m_file != other.m_file)
		return false;

	return true;
}
