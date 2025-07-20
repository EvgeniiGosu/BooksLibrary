#pragma once

#include <boost/property_tree/ptree.hpp>

#include "../Data/Book.h"

namespace bookslibrary
{
	class CPropertyTreeBookSerializer
	{
	public:
		static void Serialize(const std::shared_ptr<SBook>& ptrBook, boost::property_tree::ptree& ptBook);
		static std::shared_ptr<SBook> Deserialize(const boost::property_tree::ptree& ptBook);
	};
}