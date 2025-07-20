#pragma once

#include <format>
#include <string>
#include <vector>

namespace str {

	std::string GetRandom(uint64_t length);

	template<typename... Args>
	std::string Format(const std::string& str, Args&&... args)
	{
		return std::vformat(str, std::make_format_args(args...));
	}

	std::vector<std::string_view> Split(const std::string_view& line, char delimiter, bool ignoreEmptyTokens);

	uint64_t ToUint64(const std::string_view& str);

	std::wstring Utf8StringToWString(const std::string& str);
	std::wstring Utf8StringToWString(const std::string_view& str);
	std::string WstringToUtf8String(const std::wstring& str);
}