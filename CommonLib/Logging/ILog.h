#pragma once

#include <string>

namespace logging
{
	class ILog
	{
	public:
		~ILog() = default;

		virtual void Write(const std::string& message) = 0;
	};
}
