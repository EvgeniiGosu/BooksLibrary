#include <iostream>

#include "../../CommonLib/Str.h"

#include "SessionCommands/FindBookByTitleCommand.h"
#include "SessionCommands/ExportBookCommand.h"
#include "SessionCommands/ImportLibCommand.h"
#include "SessionCommands/OpenLibCommand.h"
#include "SessionCommands/UpdateLibCommand.h"

#include "SessionCommandDispatcher.h"
#include "TablePrinter.h"

namespace {

	template<class T>
	void AddCommand(
		std::map<std::string, std::shared_ptr<boost::program_options::options_description>>& options,
		std::map<std::string, std::function<void(const boost::program_options::variables_map&, const std::shared_ptr<CController>&, IViewCallBack*)>>& commands)
	{
		const std::string name = T::GetName();
		const auto ptrCommandOptions = T::GetOptions();

		options.emplace(name, ptrCommandOptions);

		commands.emplace(name, [](
			const boost::program_options::variables_map& vm,
			const std::shared_ptr<CController>& ptrController,
			IViewCallBack* pCallBack)
			{
				T::Run(vm, ptrController, pCallBack);
			});
	}
}

CSessionCommandDispatcher::CSessionCommandDispatcher()
{
	AddCommand<CImportLibCommand>(m_commandsOptions, m_commands);
	AddCommand<COpenLibCommand>(m_commandsOptions, m_commands);
	AddCommand<CFindBookByTitleCommand>(m_commandsOptions, m_commands);
	AddCommand<CExportBookCommand>(m_commandsOptions, m_commands);
	AddCommand<CUpdateLibCommand>(m_commandsOptions, m_commands);
}

bool CSessionCommandDispatcher::Run(const std::string& commandName, const std::wstring& args, const std::shared_ptr<CController>& ptrController, IViewCallBack* pCallBack)
{
	if (commandName.empty())
		return true;

	if (commandName == "exit")
		return false;

	boost::program_options::options_description effectiveOptions;

	const auto itOptions = m_commandsOptions.find(commandName);

	if (itOptions != m_commandsOptions.cend())
		effectiveOptions.add(*(itOptions->second));

	boost::program_options::variables_map vm;

	boost::program_options::store(
		boost::program_options::wcommand_line_parser(
			boost::program_options::split_winmain(args)).
		options(effectiveOptions).run(),
		vm);

	vm.notify();

	const auto itCommand = m_commands.find(commandName);

	if (itCommand == m_commands.cend())
	{
		pCallBack->DisplayMsg(str::Format("Unknown command: {}", commandName).c_str());
		return true;
	}

	itCommand->second(vm, ptrController, pCallBack);

	return true;
}

void CSessionCommandDispatcher::PrintHelp()
{
	CTablePrinter table;
	table.SetHeaders({ "Command", "Args", "Desc" });

	table.AddRow({ "exit", "", "Exit the application" });

	for (const auto& [name, options] : m_commandsOptions)
	{
		std::vector<std::string> args;

		for (const auto& ptrArg : options->options())
			args.push_back(ptrArg->format_name());

		std::vector<std::string> descs;
		for (const auto& ptrArg : options->options())
			descs.push_back(ptrArg->description());

		table.AddRow({ name, args,  descs });
	}

	table.Print();
}
