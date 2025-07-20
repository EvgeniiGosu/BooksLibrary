#include "../../CommonLib/Str.h"
#include "../../BooksLibraryApi/BooksLibraryApi.h"

#include "../Config/Config.h"

#include "Controller.h"

CController::CController(IViewCallBack* pCallBack)
{
	const std::shared_ptr<CConfig> ptrConfig = CConfig::FindConfig();

	if (!ptrConfig)
		return;

	pCallBack->DisplayMsg(str::Format("Config file found, opening library: books:\"{}\", storage:\"{}\"",
		str::WstringToUtf8String(ptrConfig->GetBooksDir()),
		str::WstringToUtf8String(ptrConfig->GetStoragePath())));

	OpenExistingLibImpl(false, ptrConfig->GetBooksDir(), ptrConfig->GetStoragePath(), pCallBack);
}

void CController::ImportNewLib(const std::wstring& inpxPath, const std::wstring& booksDirPath, const std::wstring& storagePath, IViewCallBack* pCallBack)
{
	bookslibrary::CBooksLibraryApi::Get()->ImportNewLib(inpxPath, booksDirPath, storagePath);
	pCallBack->DisplayMsg("The library was imported successfully.");

	CConfig::SaveConfig(booksDirPath, storagePath);
}

void CController::OpenExistingLib(const std::wstring& booksDirPath, const std::wstring& storagePath, IViewCallBack* pCallBack)
{
	OpenExistingLibImpl(true, booksDirPath, storagePath, pCallBack);
}

void CController::FindBookByTitle(const std::wstring& title, IViewCallBack* pCallBack)
{
	const std::vector<std::shared_ptr<bookslibrary::SBook>> result = bookslibrary::CBooksLibraryApi::Get()->SearchByPartialTitle(title);
	pCallBack->OnBooksResult(result);
}

void CController::ExportBook(uint64_t id, const std::wstring& path, IViewCallBack* pCallBack)
{
	bookslibrary::CBooksLibraryApi::Get()->ExportBook(id, path);
	pCallBack->DisplayMsg("The book has been successfully exported");
}

void CController::OpenExistingLibImpl(bool saveConfig, const std::wstring& booksDirPath, const std::wstring& storagePath, IViewCallBack* pCallBack)
{
	bookslibrary::CBooksLibraryApi::Get()->OpenExistingLib(booksDirPath, storagePath);
	pCallBack->DisplayMsg("The library has been opened successfully..");

	if (saveConfig)
		CConfig::SaveConfig(booksDirPath, storagePath);
}
