#pragma once

#include <memory>
#include <string>

#include "BooksStorage/IBooksStorageReader.h"

namespace bookslibrary
{
	class CBooksLibraryApi
	{
	public:
		static std::shared_ptr<CBooksLibraryApi> Get();

		void ImportNewLib(const std::wstring& inpxPath, const std::wstring& booksDirPath, const std::wstring& storagePath);

		void OpenExistingLib(const std::wstring& booksDirPath, const std::wstring& storagePath);

		std::vector<std::shared_ptr<SBook>> SearchByPartialTitle(const std::wstring& title) const;

		void ExportBook(uint64_t id, const std::wstring& dirPath) const;

		std::vector<std::shared_ptr<bookslibrary::SBook>> GetAllBooks() const;

	private:
		CBooksLibraryApi();

	private:
		std::shared_ptr<IBooksStorageReader> m_ptrBooksStorage;
		std::wstring m_booksDir;
	};
}
