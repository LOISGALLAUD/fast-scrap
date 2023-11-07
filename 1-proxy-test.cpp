#include <cpr/cpr.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <regex>
#include <vector>

#define TIMEOUT_MS 5000
#define IP_URL "http://www.httpbin.org/ip"
#define PROXY_FILE "free-proxies.txt"
#define OUTPUT_FILE "safe-proxies.txt"
#define LIMIT 5

std::string getMyIP();
std::string getOriginIPString(const std::string &jsonString);
void writeProtectedProxies(const std::vector<std::string> &proxies);

std::string getMyIP()
{
    cpr::Response response =
        cpr::Get(cpr::Url{IP_URL}, cpr::Timeout{TIMEOUT_MS});
    if (response.status_code == 200)
    {
        // Request succeeded
        return getOriginIPString(response.text);
    }
    else
    {
        // Request failed
        std::cerr << "Request failed: " << response.status_code << std::endl;
        return "";
    }
}

std::string getOriginIPString(const std::string &jsonString)
{
    nlohmann::json jsonResponse = nlohmann::json::parse(jsonString);
    if (jsonResponse.find("origin") != jsonResponse.end())
    {
        std::string ipAddress = jsonResponse["origin"];

        std::regex regexPattern(R"(\d+\.\d+\.\d+\.\d+)");
        std::smatch match;
        if (std::regex_search(ipAddress, match, regexPattern))
        {
            return match.str();
        }
    }
    return "";
}

void writeProtectedProxies(const std::vector<std::string> &proxies)
{
    std::ofstream protectedProxiesFile(OUTPUT_FILE);
    if (!protectedProxiesFile.is_open())
    {
        std::cerr << "Unable to open OUTPUT_FILE" << std::endl;
        return;
    }

    for (const std::string &proxy : proxies)
    {
        protectedProxiesFile << proxy << std::endl;
    }
}

int main(void)
{
    std::ifstream proxyFile(PROXY_FILE);
    if (!proxyFile.is_open())
    {
        std::cerr << "Unable to open the proxy file: " << PROXY_FILE
                  << std::endl;
        return 1;
    }

    std::string myIP = getMyIP();
    std::vector<std::string> protectedProxies;
    std::vector<std::string> proxies;
    std::string line;
    int count = 0;
    while (std::getline(proxyFile, line))
    {
        proxies.push_back(line);
    }
    proxyFile.close();

    for (const std::string &proxy : proxies)
    {
        cpr::Proxies proxy_config{{"http", proxy}};
        cpr::Response response =
            cpr::Get(cpr::Url{IP_URL}, proxy_config, cpr::Timeout{TIMEOUT_MS});

        if (response.status_code == 200)
        {
            // Request succeeded
            if (myIP != getOriginIPString(response.text))
            {
                // Proxy is working
                std::cout << "Proxy " << proxy << " ✅" << std::endl;
                protectedProxies.push_back(proxy);
                count++;
            }
            else
            {
                // Proxy isn't safe
                std::cerr << "Proxy " << proxy << " ❌" << std::endl;
            }
            if (count == LIMIT)
            {
                break;
            }
        }
        else
        {
            // Request failed
            std::cerr << "Proxy " << proxy
                      << " - Request failed: " << response.status_code
                      << std::endl;
        }
    }

    writeProtectedProxies(protectedProxies); // Write the protected proxies

    return 0;
}
