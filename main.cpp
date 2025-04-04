#define _DEBUGGING_
#include <iostream>
#include "./somethingHappening.h"

int main(int argc, char *argv[])
{
    somethingHappening::somethingHappeningHttpServer::log("INFO", "Hey worldy! :333");

    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            if (std::strcmp(argv[i], "--help") == 0 || std::strcmp(argv[i], "-h") == 0)
            {
                std::cout << "Usage: " << argv[0] << " [options]\n";
                std::cout << "Options:\n";
                std::cout << "  --help, -h       Show this help message\n";
                std::cout << "  --version, -v    Show version information\n";
                std::cout << "  --svr, -s    STart the silly server stuff\n";
                return 0;
            }
            else if (std::strcmp(argv[i], "--version") == 0 || std::strcmp(argv[i], "-v") == 0)
            {
                std::cout << "Version: 1.0.0\n";
                return 0;
            }
            else if (std::strcmp(argv[i], "--svr") == 0 || std::strcmp(argv[i], "-s") == 0)
            {
                somethingHappening::somethingHappeningHttpServer::startServer();
                return 0;
            }
            else
            {
                std::cout << "Unknown option: " << argv[i] << "\n";
                return 1;
            };
        };
    }; return 0;
};