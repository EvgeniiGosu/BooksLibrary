#pragma once

#include "../../BooksLibraryApi/Data/Book.h"

class IViewCallBack
{
public:
	~IViewCallBack() = default;

	virtual void OnBooksResult(const std::vector<std::shared_ptr<bookslibrary::SBook>>& books) = 0;
	virtual void DisplayMsg(const std::string& msg) = 0;
};