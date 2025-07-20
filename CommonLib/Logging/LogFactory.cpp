#include <fstream>
#include <filesystem>

#include "EmptyLog.h"
#include "StreamLog.h"

#include "LogFactory.h"

using namespace logging;

std::shared_ptr<ILog> LogFactory::CreateEmptyLog()
{
	return std::make_shared<EmptyLog>();
}

std::shared_ptr<ILog> LogFactory::CreateFileLog(const std::filesystem::path& path, bool deletePreviousLog)
{
	std::unique_ptr<std::ofstream> outputStream = std::make_unique<std::ofstream>();

	outputStream->open(path, std::ofstream::out | (deletePreviousLog ? std::ofstream::trunc : std::ofstream::app));

	return std::make_shared<StreamLog>(std::move(outputStream));
}
