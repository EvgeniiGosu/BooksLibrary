#include <iostream>
#include <windows.h>

#include "../CommonLib/Logging/Log.h"
#include "../CommonLib/Logging/LogFactory.h"

#include "MVC/CliView.h"

int main(int argc, char* argv[])
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    LOG_INIT(logging::LogFactory::CreateFileLog("BooksLibrary.log"));
    LOG.Write("Application started");

    try
    {
        CCliView().Run();
    }
    catch (std::exception& ex)
    {
        LOG.Write("Application failed with exception: {}", ex.what());
        std::cout << ex.what();

        throw;
    }

    LOG.Write("Application finished");
    return 0;
}