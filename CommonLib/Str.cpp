#include <random>

#include <boost/locale.hpp>
#include <boost/lexical_cast.hpp>

#include "Str.h"

std::string str::GetRandom(uint64_t length)
{
	const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	std::string result;
	result.resize(length);

	std::random_device rd;
	std::mt19937 generator(rd()); 
	std::uniform_int_distribution<> distribution(0, int(characters.size() - 1));

	for (size_t i = 0; i < length; ++i)
		result[i] = characters[distribution(generator)];

	return result;
}

std::vector<std::string_view> str::Split(const std::string_view& line, char delimiter, bool ignoreEmptyTokens)
{
    std::vector<std::string_view> tokens;
    size_t start = 0;
    size_t end = line.find(delimiter, start);

    while (end != std::string_view::npos)
    {
        auto token = line.substr(start, end - start);
        if (!ignoreEmptyTokens || !token.empty())
            tokens.push_back(token);

        start = end + 1;
        end = line.find(delimiter, start);
    }

    auto token = line.substr(start);
    if (!ignoreEmptyTokens || !token.empty())
        tokens.push_back(token);

    return tokens;
}

uint64_t str::ToUint64(const std::string_view& str)
{
	return boost::lexical_cast<uint64_t>(str);
}

std::wstring str::Utf8StringToWString(const std::string& str)
{
    return boost::locale::conv::utf_to_utf<wchar_t>(str);
}

std::wstring str::Utf8StringToWString(const std::string_view& str)
{
    std::string s(str);
    return boost::locale::conv::utf_to_utf<wchar_t>(s);
}

std::string str::WstringToUtf8String(const std::wstring& str)
{
    return boost::locale::conv::utf_to_utf<char>(str);
}

