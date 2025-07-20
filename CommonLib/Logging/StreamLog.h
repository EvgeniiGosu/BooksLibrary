#pragma once

#include <ostream>
#include <mutex>

#include "ILog.h"

namespace logging
{
	class StreamLog : public ILog
	{
	public:
		StreamLog(std::unique_ptr<std::ostream>&& stream);
		~StreamLog();

		void Write(const std::string& message) override;

	private:
		std::unique_ptr<std::ostream> m_stream;
		std::mutex m_mutex;
	};
}
