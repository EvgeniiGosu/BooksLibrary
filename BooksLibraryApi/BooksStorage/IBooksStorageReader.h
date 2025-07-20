#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../Data/Book.h"

namespace bookslibrary
{
	class IBooksStorageReader
	{
	public:
		~IBooksStorageReader() = default;

		virtual std::vector<std::shared_ptr<SBook>> GetAllBooks() const = 0;

		virtual std::vector<std::shared_ptr<SBook>> SearchByPartialTitle(const std::wstring& title) const = 0;

		virtual std::shared_ptr<SBook> GetBook(uint64_t id) const = 0;

	};
}