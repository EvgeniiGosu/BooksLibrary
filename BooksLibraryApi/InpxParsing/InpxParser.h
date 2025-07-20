#pragma once

#include <memory>
#include <string>

#include "../Data/Book.h"
#include "../BooksStorage/IBooksStorageWriter.h"

namespace bookslibrary
{
	class CInpxParser
	{
	public:
		CInpxParser(const std::wstring& inpxPath, const std::wstring& booksDirPath);

		uint64_t Parse(const std::shared_ptr<IBooksStorageWriter>& ptrBooksStorage);

	private:
		const std::wstring m_inpxPath;
		const std::wstring m_booksDirPath;
	};
}