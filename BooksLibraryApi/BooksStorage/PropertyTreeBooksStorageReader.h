#pragma once

#include <string>
#include <memory>

#include "IBooksStorageReader.h"

namespace bookslibrary
{
	class CPropertyTreeBooksStorageReader : public IBooksStorageReader
	{
	public:
		static std::shared_ptr<IBooksStorageReader> CreateNew(const std::wstring& storagePath);

		std::vector<std::shared_ptr<SBook>> GetAllBooks() const override;

		std::vector<std::shared_ptr<SBook>> SearchByPartialTitle(const std::wstring& title) const override;

		std::shared_ptr<SBook> GetBook(uint64_t id) const override;

	private:
		CPropertyTreeBooksStorageReader(std::vector<std::shared_ptr<SBook>>&& books);

	private:
		std::vector<std::shared_ptr<SBook>> m_books;
	};
}