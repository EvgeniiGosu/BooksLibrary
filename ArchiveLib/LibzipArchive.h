#pragma once

#include <memory>
#include <string>
#include <mutex>

#include <zip.h>

#include "IArchive.h"

namespace archive
{
	class CLibzipArchive : public IArchive
	{
	public:
		~CLibzipArchive();

		static std::shared_ptr<IArchive> Open4Read(const std::string& path);
		static std::shared_ptr<IArchive> Open4Read(const std::wstring& path);
		static std::shared_ptr<IArchive> Open4Write(const std::string& path);

		std::vector<std::string> EnumEntries() const override;
		std::string ReadFileToStr(const std::string& fileName) const override;
		uint64_t GetFileSize(const std::string& fileName) const override;
		void ExtractFile(const std::string& fileName, const std::wstring& targetPath) const override;
		void ExtractFile(const std::string& fileName, const std::string& targetPath) const override;
		void AddFileToArchive(const std::string& sourceFilePath) override;

	private:
		CLibzipArchive(zip_t* pArchive, const std::string& path);

		void ReadFileToStream(const std::string& fileName, std::ostream& stream) const;

	private:
		zip_t* m_pArchive;
		std::string m_archivePath;
		mutable std::recursive_mutex m_mutex;
	};
}