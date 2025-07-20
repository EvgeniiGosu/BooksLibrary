#include "../../../CommonLib/Str.h"

#include "../../Util/CommandlineUtil.h"

#include "FindBookByTitleCommand.h"

std::string CFindBookByTitleCommand::GetName()
{
	return "find_book_by_title";
}

std::shared_ptr<boost::program_options::options_description> CFindBookByTitleCommand::GetOptions()
{
	auto ptrOptions = std::make_shared<boost::program_options::options_description>(GetName());

	ptrOptions->add_options()
		("title", boost::program_options::wvalue<std::wstring>()->required(), "Full or partial title of the book");

	return ptrOptions;
}

void CFindBookByTitleCommand::Run(const boost::program_options::variables_map& vm, const std::shared_ptr<CController>& ptrController, IViewCallBack* pCallBack)
{
	const std::wstring title = CCommandlineUtil::Get<std::wstring>("title", vm);

	ptrController->FindBookByTitle(title, pCallBack);
}
