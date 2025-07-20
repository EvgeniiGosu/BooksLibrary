#include "Author.h"

using namespace bookslibrary;

bool SAuthor::operator==(const SAuthor& other) const
{
	if (m_author != other.m_author)
		return false;

	if (m_id != other.m_id)
		return false;

	return true;
}
