#include <chrono>
#include <ctime>

namespace util
{
	class CStopWatch
	{
	public:
		void Start();
		uint64_t Stop();

	private:
		std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
	};

	class CLogStopWatch
	{
	public:
		CLogStopWatch(const std::string& endMsgTemplate);
		~CLogStopWatch();

	private:
		std::string m_endMsgTemplate;
		CStopWatch m_stopWatch;
	};
}