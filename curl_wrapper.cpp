#include "curl_wrapper.h"

bool RestHandler::send(const char* url, int opt , int secureit, const char* method)
{
    clearBuffer();

    curl_global_init(CURL_GLOBAL_ALL);

    if (opt != NO_HEADERS_HTTP)
    {
        setHeaders();
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
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);

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

void CDeezer::setHeaders()
{
    //headers = curl_slist_append(headers, "X-RapidAPI-Key: SIGN-UP-FOR-KEY");
    headers = curl_slist_append(headers, "X-RapidAPI-Host: deezerdevs-deezer.p.rapidapi.com");
    headers = curl_slist_append(headers, "Content-Type: application/json");
}

bool CDeezer::getArtistInfo()
{   
    // https://api.deezer.com/search?q=eminem

    std::stringstream os;
    os << "https://api.deezer.com/search?q=";
    os << getArtist();
    os << "&output=json";
    std::cout << os.str();
    send(os.str().c_str(), 1);    
    return true;
}

