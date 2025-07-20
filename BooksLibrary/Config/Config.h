#pragma once

#include <string>
#include <memory>

class CConfig
{
public:
	static std::string GetConfigPath();
	static std::shared_ptr<CConfig> FindConfig();
	static void SaveConfig(const std::wstring& booksDirPath, const std::wstring& storagePath);

	const std::wstring GetBooksDir() const;
	const std::wstring GetStoragePath() const;

private:
	std::wstring m_booksDirPath;
	std::wstring m_storagePath;
};