#pragma once

#include <vector>
#include <string>

namespace archive
{
	class IArchive
	{
	public:
		~IArchive() = default;

		virtual std::vector<std::string> EnumEntries() const = 0;
		virtual std::string ReadFileToStr(const std::string& fileName) const = 0;
		virtual uint64_t GetFileSize(const std::string& fileName) const = 0;
		virtual void ExtractFile(const std::string& fileName, const std::wstring& targetPath) const = 0;
		virtual void ExtractFile(const std::string& fileName, const std::string& targetPath) const = 0;
	};
}