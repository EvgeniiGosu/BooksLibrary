#pragma once

#include "IViewCallback.h"

class CCliView : public IViewCallBack
{
public:
	void Run();

//IViewCallBack
public:
	void OnBooksResult(const std::vector<std::shared_ptr<bookslibrary::SBook>>& books) override;
	void DisplayMsg(const std::string& msg) override;
};