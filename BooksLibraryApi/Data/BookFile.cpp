#include "BookFile.h"

using namespace bookslibrary;

bool SBookFile::operator==(const SBookFile& other) const
{
	if (m_fileName != other.m_fileName)
		return false;

	if (m_size != other.m_size)
		return false;

	if (m_archiveFileName != other.m_archiveFileName)
		return false;

	return true;
}
