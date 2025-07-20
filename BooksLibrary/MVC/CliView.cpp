#include <iostream>
#include <string>
#include <memory>

#include <boost/program_options.hpp>

#include "../../CommonLib/Logging/Log.h"
#include "../../CommonLib/Str.h"

#include "Controller.h"
#include "SessionCommandDispatcher.h"
#include "TablePrinter.h"

#include "CliView.h"

void CCliView::Run()
{
	CSessionCommandDispatcher dispatcher;
	std::shared_ptr<CController> ptrController = std::make_shared<CController>(this);

	dispatcher.PrintHelp();

	while (true)
	{
		std::string line;
		if (std::getline(std::cin, line))
		{

			std::string commandName;
			std::wstring args;

			if (size_t posOfFirstSpace = line.find_first_of(" "); posOfFirstSpace == std::string::npos)
			{
				commandName = line;
			}
			else
			{
				commandName = line.substr(0, posOfFirstSpace);
				args = str::Utf8StringToWString(line.substr(posOfFirstSpace + 1, line.size() - posOfFirstSpace - 1));
			}

			if (!dispatcher.Run(commandName, args, ptrController, this))
				break;

			dispatcher.PrintHelp();
		}
		else
		{
			break;
		}

		if (std::cin.eof())
			break;
	}
}

void CCliView::OnBooksResult(const std::vector<std::shared_ptr<bookslibrary::SBook>>& books)
{
	CTablePrinter table;

	table.SetHeaders({ "Book id", "Title", "Lang", "Authors" });

	for (const std::shared_ptr<bookslibrary::SBook>& ptrBook : books)
	{
		std::vector<std::wstring> authors;
		authors.reserve(ptrBook->m_authors.size());

		for (const bookslibrary::SAuthor& author : ptrBook->m_authors)
			authors.push_back(author.m_author);

		table.AddRow(
			{
				std::to_string(ptrBook->m_id),
				ptrBook->m_title,
				bookslibrary::ToString(ptrBook->m_lang),
				authors
			});
	}

	table.Print();
}

void CCliView::DisplayMsg(const std::string& msg)
{
	std::cout << msg << std::endl;
}
