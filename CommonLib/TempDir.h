#pragma once

#include <string>
#include <memory>
#include <filesystem>

namespace util
{
	class CTempDir
	{
	public:
		~CTempDir();

		std::string GetFilePath(const std::string& fileName);
		std::string GetDirPath();

		std::wstring GetWFilePath(const std::wstring& fileName);
		std::wstring GetWDirPath();

	private:
		void CheckAndCreateDir();

	private:
		std::shared_ptr<std::filesystem::path> m_dirPath = nullptr;
	};
}