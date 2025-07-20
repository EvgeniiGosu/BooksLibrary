#include <filesystem>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "../../CommonLib/Str.h"

#include "Config.h"

std::string CConfig::GetConfigPath()
{
	return "config.ini";
}

std::shared_ptr<CConfig> CConfig::FindConfig()
{
	if (!std::filesystem::exists(GetConfigPath()))
		return nullptr;

	boost::property_tree::ptree config;

	boost::property_tree::ini_parser::read_ini(GetConfigPath(), config);

	auto ptrConfig = std::make_shared<CConfig>();

	ptrConfig->m_booksDirPath = str::Utf8StringToWString(config.get<std::string>("books"));
	ptrConfig->m_storagePath = str::Utf8StringToWString(config.get<std::string>("storage"));

	return ptrConfig;
}

void CConfig::SaveConfig(const std::wstring& booksDirPath, const std::wstring& storagePath)
{
	boost::property_tree::ptree config;

	config.add<std::string>("books", str::WstringToUtf8String(booksDirPath));
	config.add<std::string>("storage", str::WstringToUtf8String(storagePath));

	boost::property_tree::ini_parser::write_ini(GetConfigPath(), config, true);
}

const std::wstring CConfig::GetBooksDir() const
{
	return m_booksDirPath;
}

const std::wstring CConfig::GetStoragePath() const
{
	return m_storagePath;
}
