#pragma once

#include <string>

namespace bookslibrary
{
	struct SBookSeries
	{
		std::wstring m_series;
		uint64_t	 m_serialNumber = 0;

		bool operator==(const SBookSeries& other) const;
	};
}