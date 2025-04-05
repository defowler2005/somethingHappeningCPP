#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "./httplib.h"
#include "./json.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
namespace fs = std::filesystem;
using json = nlohmann::json;

/*
 * Filename: somethingHappening.h
 * Description: Something happening is nothing exciting! just a messy codebase of many c++ code snippets and tests allong side with bad practices an good ones too :3
 * Author: undefined
 * Date: 03-04-2025 7:39 AM +0000
 * Notes:
 *  - Have fun trying to manage it all as its filthy and messy but works!
 */

namespace somethingHappening
{
    namespace somethingHappeningHttpServer
    {
        httplib::Server svr;
        std::string custom404Page = "<h3 style='color: red;'>404 - Not found: That file not found on this server.</h3>";
        std::string configFile = (fs::current_path() / "CppServerConfig.json").string();
        std::vector<std::string> blackListedPaths;
        std::string ip = "0.0.0.0";
        int port = 6432;
        std::map<std::string, std::string, std::less<std::string>, std::allocator<std::pair<const std::string, std::string>>> mime_types = {
            {".html", "text/html"},
            {".css", "text/css"},
            {".js", "text/javascript"},
            {".json", "application/json"},
            {".xml", "application/xml"},
            {".svg", "image/svg+xml"},
            {".png", "image/png"},
            {".ico", "image/x-icon"},
            {".gif", "image/gif"},
            {".jpg", "image/jpeg"},
            {".jpeg", "image/jpeg"},
            {".zip", "application/zip"},
            {".tar", "application/x-tar"},
            {".log", "text/plain"},
            {".gz", "application/gzip"},
            {".cpp", "text/plain"},
            {".h", "text/plain"},
        };
        static std::string readFile(const std::string &filePath)
        {
            std::ostringstream contents;
            std::ifstream file(filePath, std::ios::in | std::ios::binary);

            if (!file)
                return "Error retrieving file contents";

            contents << file.rdbuf();
            return contents.str();
        };

        static void log(const std::string &logType, const std::string &message)
        {
            static const std::unordered_map<std::string, std::string> validTypes = {
                {"INFO", "\033[32m"},
                {"WARNING", "\033[33m"},
                {"ERROR", "\033[31m"},
                {"DEBUG", "\033[36m"}};

            std::time_t now_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::ofstream logFile("./CppServerLog.log", std::ios::app);
            std::tm now_tm;
            localtime_r(&now_time_t, &now_tm);
            std::ostringstream currentTime;
            currentTime << std::put_time(&now_tm, "%d-%m-%y %H:%M:%S");

#ifdef _DEBUGGING_
            if (validTypes.find(logType) == validTypes.end())
            {
                std::cerr << "\033[33m[WARNING] - Invalid log type used: " << logType << "\033[0m" << std::endl;
                return;
            };
#endif

            std::string formattedLog = "[" + currentTime.str() + "] [" + logType + "] - " + message + "\n";
            std::cout << validTypes.at(logType) << formattedLog << "\033[0m";
            logFile << formattedLog;

            logFile.close();
        };

        void startServer()
        {
            svr.Get("/", [&](const httplib::Request &req, httplib::Response &res) // Root path (./index.html).
                    {
			std::string clientIp = req.remote_addr;
			std::string filePath = (fs::absolute(fs::current_path() / "index.html").string());

			if (fs::exists(filePath))
			{
				res.set_content(readFile(filePath), "text/html");
				log("INFO" , "Client " + clientIp + " " + req.method + " " + req.path + " (200 OK)");
			}
			else
			{
				res.status = httplib::StatusCode::NotFound_404;
				log("INFO" , "Client " + clientIp + " " + req.method + " " + req.path + "index.html (404 Not Found)");
				res.set_content("<h3 style='color: red;'>404 - The main index.html doesn't exist on this server.</h3>", "text/html");
			}; });

            svr.Get(".*", [&](const httplib::Request &req, httplib::Response &res)
                    {
    std::string clientIp = req.remote_addr;
    std::string filePath = fs::current_path().string() + req.path;
	std::string content_type = httplib::detail::find_content_type(filePath, mime_types, "application/octet-stream");
	
    if (fs::exists(filePath))
    {
        res.set_content(readFile(filePath), content_type);
        log("INFO", "Client " + clientIp + " " + req.method + " " + req.path + " (200 OK)");
    }
    else
    {
        res.status = httplib::StatusCode::NotFound_404;
        log("INFO", "Client " + clientIp + " " + req.method + " " + req.path + " (404 Not Found)");
        res.set_content(custom404Page, "text/html");
    }; });

            try
            {
                log("INFO", "Server listening on " + ip + ":" + std::to_string(port) + (ip == "0.0.0.0" ? " All network interfaces." : ""));
                svr.listen(ip, port);
            }
            catch (std::exception &error)
            {
                log("ERROR", "The provided port is in use! choose another one");
            }
        };

        bool checkURL(std::string url)
        {
            if (url.empty())
                somethingHappening::somethingHappeningHttpServer::log("WARNING", "Given checkURL string is not a valid string");

            httplib::Client client(url);

            auto res = client.Get("/");

            if (res && !res->body.empty())
            {
                return true;
            }
            else
            {
                return false;
            }
        };

        bool telemetryJibberJabber(const std::string &url = "https://dc.services.visualstudio.com/v2/track")
        {
            const std::string prefix = "https://";
            if (url.substr(0, prefix.size()) != prefix)
            {
                std::cerr << "Invalid URL scheme.\n";
                return false;
            }

            size_t slash_pos = url.find('/', prefix.size());
            if (slash_pos == std::string::npos)
            {
                std::cerr << "Invalid URL: no path specified.\n";
                return false;
            }

            std::string host = url.substr(prefix.size(), slash_pos - prefix.size());
            std::string path = url.substr(slash_pos);

            httplib::SSLClient cli(host);
            cli.set_follow_location(true);

            httplib::Headers headers = {
                {"Content-Type", "application/json"},
                {"User-Agent", "Cpp-Jabber/0.1"}};

            std::string payload = R"(
        {
  "name": "Microsoft.ApplicationInsights.0c6ae279ed8443289764825290e4f9e2-1a736e7c-1324-4338-be46-fc2a58ae4d14-7255.Event",
  "time": "2025-04-04T20:00:00.000Z",
  "iKey": "0c6ae279ed8443289764825290e4f9e2-1a736e7c-1324-4338-be46-fc2a58ae4d14-7255",
  "tags": {
    "ai.application.ver": "1.17.5",
    "ai.device.id": "d3v1c3-1d",
    "ai.device.osVersion": "Ubuntu 22.04",
    "ai.session.id": "r4nd0m-550n-1d",
    "ai.user.id": "user-1234",
    "ai.cloud.roleInstance": "vscode"
  },
  "data": {
    "baseType": "EventData",
    "baseData": {
      "ver": 2,
      "name": "C_Cpp/LanguageServer/activate",
      "properties": {
        "extensionName": "ms-vscode.cpptools",
        "version": "1.17.5",
        "osPlatform": "linux",
        "activationMode": "standard",
        "workspaceType": "multi-root",
        "featureFlag1": "true"
      },
      "measurements": {
        "startupTimeMs": 1234,
        "numCppFiles": 12003,
        "numHeaderFiles": 5000,
        "numSourceFiles": 7000,
        "numOtherFiles": 2000,
        "numFilesWithErrors": 100,
        "numFilesWithWarnings": 200,
        "numFilesWithInfo": 300,
        "numFilesWithHints": 400,
        "numFilesWithMessages": 500,
        "numFilesWithDiagnostics": 600,
        "numFilesWithCodeActions": 700,
        "numFilesWithCodeLens": 800,
        "numFilesWithCodeLensCodeActions": 900,
        "numFilesWithCodeLensReferences": 1000,
        "numFilesWithCodeLensReferencesCodeActions": 1100,
        "numFilesWithCodeLensReferencesCodeActionsReferences": 1200,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLens": 1300,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLensCodeActions": 1400,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLensCodeActionsReferences": 1500,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLensCodeActionsReferencesCodeLens": 1600,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLensCodeActionsReferencesCodeLensCodeActions": 1700,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLensCodeActionsReferencesCodeLensCodeActionsReferences": 1800,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLensCodeActionsReferencesCodeLensCodeActionsReferencesCodeLens": 1900,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLensCodeActionsReferencesCodeLensCodeActionsReferencesCodeLensCodeActions": 2000,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLensCodeActionsReferencesCodeLensCodeActionsReferencesCodeLensCodeActions": 2100,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLensCodeActionsReferencesCodeLensCodeActionsReferencesCodeLensCodeActions": 2200,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLensCodeActionsReferencesCodeLensCodeActionsReferencesCodeLensCodeActions": 2300,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLensCodeActionsReferencesCodeLensCodeActionsReferencesCodeLensCodeActions": 2400,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLensCodeActionsReferencesCodeLensCodeActionsReferencesCodeLensCodeActions": 2500,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLensCodeActionsReferencesCodeLensCodeActionsReferencesCodeLensCodeActions": 2600,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLensCodeActionsReferencesCodeLensReferences": 2700,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLensReferences": 2800,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLens": 2900,
        "numFilesWithCodeLensReferencesCodeLens": 3000,
        "numFilesWithCodeLens": 3100,
        "numFilesWithCodeActions": 3200,
        "numFilesWithCodeLensCodeActions": 3300,
        "numFilesWithCodeLensReferencesCodeActions": 3400,
        "numFilesWithCodeLensReferencesCodeActionsReferences": 3500,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLens": 3600,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLensCodeActions": 3700,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLensCodeActionsReferences": 3800,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLensCodeActionsReferencesCodeLens": 3900,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLensCodeActionsReferencesCodeLensCodeActions": 4000,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLensCodeActionsReferencesCodeLensCodeActions": 4100,
        "numFilesWithCodeLensReferencesCodeActionsReferencesCodeLensCodeActionsReferencesCodeLensCodeActions": 4200,
      }
    }
  }
}

    })";

            auto res = cli.Post(path.c_str(), headers, payload, "application/json");

            if (res)
            {
                std::cout << "Status: " << res->status << "\n";
                std::cout << "Response: " << res->body << "\n";
                return true;
            }
            else
            {
                std::cerr << "Request failed: " << res.error() << "\n";
                return false;
            };
        };

        void mLeak()
        {
            std::cout << "Memory leak started: \n";

            while (true)
            {
                int *someLeak = new int[10000];
            };
            //delete someLeak;
        }
    };
};