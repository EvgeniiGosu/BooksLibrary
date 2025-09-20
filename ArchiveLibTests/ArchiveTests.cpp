#include <map>
#include <set>
#include <fstream>

#include <catch2/catch.hpp>
#include <zip.h>

#include "../ArchiveLib/ArchiveFactory.h"
#include "../CommonLib/TempDir.h"
#include "../CommonLib/Str.h"

namespace {

	void CreateTestArchive(const std::string& path, const std::map<std::string, std::string>& content)
	{
		int errCode;

		zip_t* pArchive = zip_open(path.c_str(), ZIP_CREATE| ZIP_TRUNCATE, &errCode);
		REQUIRE(pArchive != NULL);

		for (const auto& [fileName, fileContent] : content)
		{
			zip_source_t* pSource = zip_source_buffer(pArchive, fileContent.c_str(), strlen(fileContent.c_str()), 0);
			REQUIRE(pSource != NULL);

			REQUIRE(zip_file_add(pArchive, fileName.c_str(), pSource, ZIP_FL_OVERWRITE) != -1);
		}

		REQUIRE(zip_close(pArchive) == 0);
	}

	void CreateTestFile(const std::wstring& path, const std::string& content)
	{
		std::ofstream file(path);
		REQUIRE(file.is_open());
		file << content;
		file.close();
	}
}

TEST_CASE("CArchiveFactory.AddFileToArchive", "[archive]")
{
	util::CTempDir tmpDir;

	const std::string arhivePath = tmpDir.GetFilePath("new_archive.zip");
	const std::wstring filePath = tmpDir.GetWFilePath(L"test_file.txt");

	std::shared_ptr<archive::IArchive> ptrArchive = archive::CArchiveFactory::Open4Write(arhivePath);

	CreateTestFile(filePath, "file content");

	ptrArchive->AddFileToArchive(str::WstringToUtf8String(filePath));

	ptrArchive.reset(); // for flush

	ptrArchive = archive::CArchiveFactory::Open4Read(arhivePath);

	const auto fileContent = ptrArchive->ReadFileToStr("test_file.txt");

	REQUIRE(fileContent == "file content");
}

TEST_CASE("CArchiveFactory.Open4Read existing archive", "[archive]")
{
	util::CTempDir tmpDir;
	const std::string arhivePath = tmpDir.GetFilePath("existig_archive.zip");

	CreateTestArchive(arhivePath, { { "file.txt", "file content" } });

	REQUIRE_NOTHROW(archive::CArchiveFactory::Open4Read(arhivePath));
}

TEST_CASE("CArchiveFactory.Open4Read not existing archive", "[archive]")
{
	REQUIRE_THROWS_WITH(archive::CArchiveFactory::Open4Read("not_existing.zip"), Catch::Contains("Failed to open archive"));
}

TEST_CASE("CArchiveFactory.EnumEntries", "[archive]")
{
	util::CTempDir tmpDir;
	const std::string arhivePath = tmpDir.GetFilePath("existig_archive.zip");

	const std::map<std::string, std::string> content =
	{
		{ "file1.txt", "file1 content" },
		{ "file2.txt", "file2 content" },
		{ "file3.txt", "file3 content" },
	};

	CreateTestArchive(arhivePath, content);

	const std::shared_ptr<archive::IArchive> ptrArchive = archive::CArchiveFactory::Open4Read(arhivePath);

	std::vector<std::string> fileNames = ptrArchive->EnumEntries();
	std::sort(fileNames.begin(), fileNames.end());

	const std::set<std::string> expectedFileNames = { "file1.txt", "file2.txt", "file3.txt" };

	REQUIRE(std::equal(expectedFileNames.cbegin(), expectedFileNames.cend(), fileNames.cbegin(), fileNames.cend()));
}

TEST_CASE("CArchiveFactory.ReadFileToStr", "[archive]")
{
	util::CTempDir tmpDir;
	const std::string arhivePath = tmpDir.GetFilePath("existig_archive.zip");

	const std::map<std::string, std::string> content =
	{
		{ "file1.txt", "file1 content" },
		{ "file2.txt", "file2 content" },
		{ "file3.txt", "file3 content" },
	};

	CreateTestArchive(arhivePath, content);

	const std::shared_ptr<archive::IArchive> ptrArchive = archive::CArchiveFactory::Open4Read(arhivePath);

	const std::vector<std::string> fileNames = ptrArchive->EnumEntries();

	const auto getExpectedConent = [&content](const std::string& fileName)
		{
			const auto it = content.find(fileName);
			REQUIRE(it != content.cend());

			return it->second;
		};

	for (const auto& fileName : fileNames)
	{
		const auto expectedContent = getExpectedConent(fileName);
		const auto fileContent = ptrArchive->ReadFileToStr(fileName);

		REQUIRE(expectedContent == fileContent);
	}
}

TEST_CASE("CArchiveFactory.ExtractFile", "[archive]")
{
	util::CTempDir tmpDir;
	const std::string arhivePath = tmpDir.GetFilePath("existig_archive.zip");

	const std::map<std::string, std::string> content =
	{
		{ "file1.txt", "file1 content" },
		{ "file2.txt", "file2 content" },
		{ "file3.txt", "file3 content" },
	};

	CreateTestArchive(arhivePath, content);

	const std::shared_ptr<archive::IArchive> ptrArchive = archive::CArchiveFactory::Open4Read(arhivePath);

	const std::vector<std::string> fileNames = ptrArchive->EnumEntries();

	const auto getExpectedConent = [&content](const std::string& fileName)
		{
			const auto it = content.find(fileName);
			REQUIRE(it != content.cend());

			return it->second;
		};

	for (const std::string& fileName : fileNames)
	{
		ptrArchive->ExtractFile(fileName, tmpDir.GetFilePath(fileName));

		std::ifstream file(tmpDir.GetFilePath(fileName));

		std::string fileContent;

		std::copy(
			std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>(),
			std::back_inserter(fileContent)
		);

		const std::string expectedContent = getExpectedConent(fileName);

		REQUIRE(expectedContent == fileContent);
	}
}