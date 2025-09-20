#pragma once

#include <string>
#include <memory>
#include <fstream>

#include "IBooksStorageWriter.h"

namespace bookslibrary
{
	class CPropertyTreeBooksStorageWriter : public IBooksStorageWriter
	{
	public:
		static std::shared_ptr<IBooksStorageWriter> CreateNew(const std::wstring& storagePath);

		~CPropertyTreeBooksStorageWriter();

		void AddNewBooks(std::vector<std::shared_ptr<SBook>>&& books) override;
		void Finish() override;

	private:
		CPropertyTreeBooksStorageWriter(const std::wstring& path);

	private:
		std::ofstream m_stream;
		std::wstring m_storagePath;
		bool m_finished = false;
	};
}