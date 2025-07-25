#pragma once

#include <string>

#include <boost/program_options.hpp>

#include "../../CommonLib/Str.h"

class CCommandlineUtil
{
public:
	template<class T>
	static T Get(const std::string& name, const boost::program_options::variables_map& vm)
	{
		if (vm.contains(name))
			return vm[name].as<T>();

		throw std::exception(str::Format("Unknown option {}", name).c_str());
	}
};