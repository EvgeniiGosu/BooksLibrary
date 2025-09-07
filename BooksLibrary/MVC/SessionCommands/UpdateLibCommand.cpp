#include "../../../CommonLib/Str.h"

#include "../../Util/CommandlineUtil.h"

#include "UpdateLibCommand.h"

std::string CUpdateLibCommand::GetName()
{
	return "update_lib";
}

std::shared_ptr<boost::program_options::options_description> CUpdateLibCommand::GetOptions()
{
	auto ptrOptions = std::make_shared<boost::program_options::options_description>(GetName());
	return ptrOptions;
}

void CUpdateLibCommand::Run(const boost::program_options::variables_map& vm, const std::shared_ptr<CController>& ptrController, IViewCallBack* pCallBack)
{
	ptrController->RebuildLib(pCallBack);
}
