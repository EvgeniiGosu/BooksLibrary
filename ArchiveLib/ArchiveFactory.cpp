#include "ArchiveFactory.h"

#include "LibzipArchive.h"

using namespace archive;

std::shared_ptr<IArchive> CArchiveFactory::Open4Read(const std::string& path)
{
	return CLibzipArchive::Open4Read(path);
}

std::shared_ptr<IArchive> CArchiveFactory::Open4Read(const std::wstring& path)
{
	return CLibzipArchive::Open4Read(path);
}
