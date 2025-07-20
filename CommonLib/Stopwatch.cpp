#include "Logging/Log.h"

#include "Stopwatch.h"

using namespace util;

void CStopWatch::Start()
{
	m_StartTime = std::chrono::steady_clock::now();
}

uint64_t CStopWatch::Stop()
{
	auto endTime = std::chrono::steady_clock::now();

	return std::chrono::duration_cast<std::chrono::seconds>(endTime - m_StartTime).count();
}

CLogStopWatch::CLogStopWatch(const std::string& endMsgTemplate)
	: m_endMsgTemplate(endMsgTemplate)
{
	m_stopWatch.Start();
}

CLogStopWatch::~CLogStopWatch()
{
	const auto sec = m_stopWatch.Stop();

	LOG.Write(m_endMsgTemplate, std::to_string(sec));
}
