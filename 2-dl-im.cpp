#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <string>

// Fonction de rappel pour écrire les données téléchargées dans un fichier
size_t WriteData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

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

    CURL *curl;
    FILE *file;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, imageUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        // curl_easy_setopt(curl, CURLOPT_SERVER_RESPONSE_TIMEOUT, 5L);
        curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 20L);

        // Configuration du proxy
        const char *proxyUrl = "http://92.53.100.208:8080";
        // de votre proxy
        curl_easy_setopt(curl, CURLOPT_PROXY, proxyUrl);
        // curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_HTTPS);
        // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        file = fopen(outputFileName.c_str(), "wb");
        if (file)
        {
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);

            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK)
            {
                std::cerr << "Erreur lors du téléchargement de l'image: "
                          << curl_easy_strerror(res) << std::endl;
            }
            else
            {
                std::cout << "Image Téléchargée ✅" << std::endl;
            }

            fclose(file);
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return 0;
}
