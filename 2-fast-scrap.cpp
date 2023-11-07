#include <cpr/cpr.h>
#include <iostream>

#define PROXY_FILE "safe-proxies.txt"
#define TIMEOUT_MS 10000

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Utilisation: " << argv[0]
                  << " <URL de l'image> <nom du fichier de sortie>"
                  << std::endl;
        return 1;
    }

    std::string imageUrl = argv[1];
    std::string outputFileName = argv[2];

    std::ifstream proxyFile(PROXY_FILE);

    if (!proxyFile.is_open())
    {
        std::cerr << "Impossible d'ouvrir le fichier " << PROXY_FILE
                  << std::endl;
        return 1;
    }

    std::vector<std::string> proxies;
    std::string line;
    while (std::getline(proxyFile, line))
    {
        proxies.push_back(line);
    }
    proxyFile.close();

    std::string proxy = proxies[rand() % proxies.size()];

    std::cout << "Utilisation du proxy " << proxy << std::endl;

    cpr::Proxies proxy_config{{"http", proxy}};
    cpr::Response response = cpr::Get(cpr::Url{imageUrl}, proxy_config);

    while (response.status_code != 200)
    {
        std::cout << "ERROR " << response.status_code << std::endl;
        proxy = proxies[rand() % proxies.size()];
        std::cout << "Proxy " << proxy << std::endl;
        proxy_config = cpr::Proxies{{"http", proxy}};
        response = cpr::Get(cpr::Url{imageUrl}, proxy_config);
    }

    std::ofstream imageFile(outputFileName, std::ios::binary);
    imageFile.write(response.text.c_str(), response.text.size());
    std::cout << "Image Téléchargée ✅" << std::endl;
    imageFile.close();

    return 0;
}
