#pragma once

#include <string>

namespace bookslibrary
{
	struct SBookFile
	{
		std::string m_fileName;
		uint64_t m_size = 0;

		std::string m_archiveFileName;

		bool operator==(const SBookFile& other) const;
	};
}