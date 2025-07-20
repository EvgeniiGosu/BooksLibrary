#pragma once

#include <cstdint>
#include <string>

namespace bookslibrary
{
	enum class ELang : uint32_t
	{
		ru = 1,
		en,
		cs,
	};

	std::string ToString(ELang lang);
	ELang FromString(const std::string& lang);

}