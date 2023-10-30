#include "curl_wrapper.h"

bool RestHandler::send(const char* url, int opt , int secureit )
{
    clearBuffer();
    struct curl_slist* headers = nullptr; // init to NULL is important 
    curl_global_init(CURL_GLOBAL_ALL);

    if (opt != NO_HEADERS_HTTP)
    {
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charset: utf-8");
    }

    curl_global_init(CURL_GLOBAL_ALL);
    CURL* curl = nullptr;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);

        CURLcode code = curl_easy_perform(curl);

        if (secureit != NO_HEADERS_HTTPS)
        {
            setSecurity(curl);
        }

        if (code != CURLE_OK)
        {
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return false;
        }
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return true;
}

