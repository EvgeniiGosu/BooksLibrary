#include "StreamLog.h"

using namespace logging;

StreamLog::StreamLog(std::unique_ptr<std::ostream>&& stream)
	: m_stream(std::move(stream))
{
}

StreamLog::~StreamLog() = default;

void StreamLog::Write(const std::string& message)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	*m_stream << message << std::endl;
}
