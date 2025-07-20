#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <set>

#include "../Data/Book.h"

namespace bookslibrary
{
	class CBooksParser
	{
	public:
		std::vector<std::shared_ptr<SBook>> Parse(const std::string& inpFileContent, const std::string& inpFileName);
		
	private:
		uint64_t GerOrCreateAuthorId(const std::wstring& author);

	private:
		std::map<std::wstring, uint64_t> m_AuthorsId;
		uint64_t m_lastAuthorId = 0;
		std::set<std::string> m_allowedLanguages = { "RU", "en", "cs", "ru", "ru-RU" };
	};
}