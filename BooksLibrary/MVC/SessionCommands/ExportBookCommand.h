#pragma once

#include <string>

#include <boost/program_options.hpp>

#include "../Controller.h"
#include "../IViewCallback.h"

class CExportBookCommand
{
public:
	static std::string GetName();

	static std::shared_ptr<boost::program_options::options_description> GetOptions();

	static void Run(
		const boost::program_options::variables_map& vm,
		const std::shared_ptr<CController>& ptrController,
		IViewCallBack* pCallBack);
};