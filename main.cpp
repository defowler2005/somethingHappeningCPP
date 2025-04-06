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
            char *arg = argv[i];

            if (std::strcmp(arg, "--help") == 0 || std::strcmp(arg, "-h") == 0)
            {
                std::cout << "Usage: " << argv[0] << " [options]\n";
                std::cout << "Options:\n";
                std::cout << "  --help, -h       Show this help message\n";
                std::cout << "  --version, -v    Show version information\n";
                std::cout << "  --svr, -s    Start the silly server stuff\n";
                std::cout << "  --posttlementrydata, -ptd    Try post crappy data to microsoft backend telementry servers\n"; // telemetryJibberJabber.

                return 0;
            }
            else if (std::strcmp(arg, "--version") == 0 || std::strcmp(arg, "-v") == 0)
            {
                std::cout << "Version: 1.0.0\n";
                return 0;
            }
            else if (std::strcmp(arg, "--svr") == 0 || std::strcmp(arg, "-s") == 0)
            {
                somethingHappening::somethingHappeningHttpServer::startServer();
                return 0;
            }
            else if (std::strcmp(arg, "--posttlementrydata") == 0 || std::strcmp(arg, "-ptd") == 0)
            {
                if (somethingHappening::somethingHappeningHttpServer::telemetryJibberJabber())
                {
                    std::cout << "Telemetry data posted successfully.\n";
                }
                else
                {
                    std::cerr << "Failed to post telemetry data.\n";
                };
                return 0;
            }
            else if (std::strcmp(arg, "--mLeak") == 0 || std::strcmp(arg, "-ml") == 0)
            {
                somethingHappening::somethingHappeningHttpServer::mLeak();
                return 0;
            }
            else
            {
                std::cout << "Unknown option: " << arg << "\n";
                return 1;
            };
        };
    };
    return 0;
};