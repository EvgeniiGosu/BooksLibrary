#include <chrono>
#include <sstream> 
#include <ctime>
#include <thread>

#include "../Str.h"

#include "Log.h"
#include "LogFactory.h"

using namespace logging;

std::shared_ptr<ILog> Log::m_logger = LogFactory::CreateEmptyLog();

void Log::Init(const std::shared_ptr<ILog>& logger)
{
	Log::m_logger = logger;
}

std::string Log::GetCurrentThreadId()
{
	std::stringstream stream;

	stream << std::this_thread::get_id();

	return stream.str();
}

std::string Log::GetCurrentTime()
{
	auto now = std::chrono::system_clock::now();
	auto ms = duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

	auto timer = std::chrono::system_clock::to_time_t(now);

	struct tm time;
	localtime_s(&time, &timer);

	std::stringstream stream;

	stream << std::put_time(&time, "%Y-%m-%d %H:%M:%S");
	stream << ":" << std::setw(3) << ms.count();

	return stream.str();
}

Log::Log()
	: m_logHeader(str::Format("[{}][{}]:	", Log::GetCurrentTime(), Log::GetCurrentThreadId()))
{
}

void Log::Write(const std::string& message)
{
	m_logger->Write(m_logHeader + message);
}

void Log::Write(const std::string& message, const std::string& arg0)
{
	m_logger->Write(m_logHeader + str::Format(message, arg0));
}

void Log::Write(const std::string& message, const std::string& arg0, const std::string& arg1)
{
	m_logger->Write(m_logHeader + str::Format(message, arg0, arg1));
}

void Log::Write(const std::string& message, const std::string& arg0, const std::string& arg1, const std::string& arg2)
{
	m_logger->Write(m_logHeader + str::Format(message, arg0, arg1, arg2));
}
