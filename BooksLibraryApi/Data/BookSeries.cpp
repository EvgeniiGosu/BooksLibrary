#include "BookSeries.h"

using namespace bookslibrary;

bool SBookSeries::operator==(const SBookSeries& other) const
{
	if (m_series != other.m_series)
		return false;

	if (m_serialNumber != other.m_serialNumber)
		return false;

	return true;
}
