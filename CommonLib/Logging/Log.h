#pragma once

#include <functional>
#include <memory>
#include <string>

#include "ILog.h"
#include "LogFactory.h"

namespace logging
{
	class Log
	{
	public:
		static void Init(const std::shared_ptr<ILog>& logger);
		static std::string GetCurrentTime();
		static std::string GetCurrentThreadId();

	public:
		Log();

		void Write(const std::string& message);
		void Write(const std::string& message, const std::string& arg0);
		void Write(const std::string& message, const std::string& arg0, const std::string& arg1);
		void Write(const std::string& message, const std::string& arg0, const std::string& arg1, const std::string& arg2);

	private:
		static std::shared_ptr<ILog> m_logger;
		const std::string m_logHeader;
	};
}

#define LOG_INIT(LOGGER)	logging::Log::Init(LOGGER)
#define LOG					logging::Log()
