#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../Data/Book.h"

namespace bookslibrary
{
	class IBooksStorageWriter
	{
	public:
		~IBooksStorageWriter() = default;

		virtual void AddNewBooks(std::vector<std::shared_ptr<SBook>>&& books) = 0;
		virtual void Finish() = 0;
	};
}