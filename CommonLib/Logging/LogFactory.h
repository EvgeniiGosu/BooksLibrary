#pragma once

#include <memory>
#include <string>
#include <filesystem>

#include "ILog.h"

namespace logging
{
	class LogFactory
	{
	public:
		static std::shared_ptr<ILog> CreateEmptyLog();
		static std::shared_ptr<ILog> CreateFileLog(const std::filesystem::path& path, bool deletePreviousLog = false);
	};

}
