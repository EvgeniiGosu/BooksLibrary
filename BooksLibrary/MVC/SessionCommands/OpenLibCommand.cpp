#include "../../../CommonLib/Str.h"

#include "../../Util/CommandlineUtil.h"

#include "OpenLibCommand.h"

std::string COpenLibCommand::GetName()
{
	return "open_lib";
}

std::shared_ptr<boost::program_options::options_description> COpenLibCommand::GetOptions()
{
	auto ptrOptions = std::make_shared<boost::program_options::options_description>(GetName());

	ptrOptions->add_options()
		("inpx", boost::program_options::wvalue<std::wstring>()->required(), "Path to inpx file")
		("books", boost::program_options::wvalue<std::string>()->required(), "Path to dir with books arhives")
		("storage", boost::program_options::wvalue<std::string>()->required(), "Path to existing storage");

	return ptrOptions;
}

void COpenLibCommand::Run(const boost::program_options::variables_map& vm, const std::shared_ptr<CController>& ptrController, IViewCallBack* pCallBack)
{
	const std::wstring inpxPath = CCommandlineUtil::Get<std::wstring>("inpx", vm);
	const std::wstring booksDirPath = CCommandlineUtil::Get<std::wstring>("books", vm);
	const std::wstring storagePath = CCommandlineUtil::Get<std::wstring>("storage", vm);

	ptrController->OpenExistingLib(inpxPath, booksDirPath, storagePath, pCallBack);
}
