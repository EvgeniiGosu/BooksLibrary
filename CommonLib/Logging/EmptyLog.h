#pragma once

#include "ILog.h"

namespace logging
{
	class EmptyLog : public ILog
	{
	public:
		void Write(const std::string& message) override;
	};
}
