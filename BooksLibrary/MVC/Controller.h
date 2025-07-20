#pragma once

#include <string>

#include "IViewCallback.h"

class CController
{
public:
	CController(IViewCallBack* pCallBack);

	void ImportNewLib(const std::wstring& inpxPath, const std::wstring& booksDirPath, const std::wstring& storagePath, IViewCallBack* pCallBack);
	void OpenExistingLib(const std::wstring& booksDirPath, const std::wstring& storagePath, IViewCallBack* pCallBack);
	void FindBookByTitle(const std::wstring& title, IViewCallBack* pCallBack);
	void ExportBook(uint64_t id, const std::wstring& path, IViewCallBack* pCallBack);

private:
	void OpenExistingLibImpl(bool saveConfig, const std::wstring& booksDirPath, const std::wstring& storagePath, IViewCallBack* pCallBack);

};