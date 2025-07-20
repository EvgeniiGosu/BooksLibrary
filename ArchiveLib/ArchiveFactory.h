#pragma once

#include <memory>
#include <string>

#include "IArchive.h"

namespace archive
{
	class CArchiveFactory
	{
	public:
		static std::shared_ptr<IArchive> Open4Read(const std::string& path);
		static std::shared_ptr<IArchive> Open4Read(const std::wstring& path);
	};
}