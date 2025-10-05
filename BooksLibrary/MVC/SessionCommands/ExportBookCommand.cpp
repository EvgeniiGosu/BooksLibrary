#include "../../../CommonLib/Str.h"

#include "../../Util/CommandlineUtil.h"

#include "ExportBookCommand.h"

std::string CExportBookCommand::GetName()
{
	return "export_book";
}

std::shared_ptr<boost::program_options::options_description> CExportBookCommand::GetOptions()
{
	auto ptrOptions = std::make_shared<boost::program_options::options_description>(GetName());

	ptrOptions->add_options()
		("id", boost::program_options::value<std::uint64_t>(), "Book id")
		("path", boost::program_options::wvalue<std::wstring>()->required(), "Destination path (dir)");

	return ptrOptions;
}

void CExportBookCommand::Run(const boost::program_options::variables_map& vm, const std::shared_ptr<CController>& ptrController, IViewCallBack* pCallBack)
{
	const std::wstring path = CCommandlineUtil::Get<std::wstring>("path", vm);

	if (uint64_t id; CCommandlineUtil::Find<uint64_t>("id", vm, id))
		ptrController->ExportBook(id, path, pCallBack);
	else
		ptrController->ExportBooks(path, pCallBack);
}
