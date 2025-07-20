#pragma once

#include <functional>

#include <boost/program_options.hpp>

#include "Controller.h"
#include "IViewCallback.h"

class CSessionCommandDispatcher
{
public:
	CSessionCommandDispatcher();

	bool Run(const std::string& commandName, const std::wstring& args, const std::shared_ptr<CController>& ptrController, IViewCallBack* pCallBack);
	void PrintHelp();

private:
	std::map<std::string, std::shared_ptr<boost::program_options::options_description>> m_commandsOptions;
	std::map<std::string, std::function<void(const boost::program_options::variables_map&, const std::shared_ptr<CController>&, IViewCallBack*)>> m_commands;
};