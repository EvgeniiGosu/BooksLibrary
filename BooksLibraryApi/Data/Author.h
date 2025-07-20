#pragma once

#include <string>

namespace bookslibrary
{
	struct SAuthor
	{
		std::wstring m_author;
		uint64_t m_id = 0;

		bool operator==(const SAuthor& other) const;
	};
}