#include "curl_wrapper.h"


static int writer(char* data, size_t size, size_t nmemb, std::string* writerData)
{
    if (writerData == nullptr)
        return 0;

    writerData->append(data, size * nmemb);

    return (size * nmemb);
}

std::string send(const char* url)
{
    std::string response;

    curl_global_init(CURL_GLOBAL_ALL);
    CURL* curl = nullptr;

    curl = curl_easy_init();
    if (curl) 
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);

        CURLcode code = curl_easy_perform(curl);        

        if (code != CURLE_OK)
        {
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return "";
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return response;
}

std::string sendWithHeaders(const char* url)
{
    std::string response;
    struct curl_slist* headers = nullptr; // init to NULL is important 
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charset: utf-8");
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
        if (code != CURLE_OK)
        {
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return "";
        }
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return response;

}

std::string sendWithHeadersHttps(const char* url)
{
    std::string response;
    const char* pPassphrase = nullptr;
    const char* pCertFile = "testcert.pem";
    const char* pCACertFile = "cacert.pem";
    const char* pHeaderFile = "dumpit";
    const char*  pKeyName = "testkey.pem";
    const char*  pKeyType = "PEM";
    const char* pEngine = nullptr;

    struct curl_slist* headers = nullptr; // init to NULL is important 
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charset: utf-8");
    curl_global_init(CURL_GLOBAL_ALL);
    CURL* curl = nullptr;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
        /* cert is stored PEM coded in file... */
        /* since PEM is default, we needn't set it for PEM */
        curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");

        /* set the cert for client authentication */
        curl_easy_setopt(curl, CURLOPT_SSLCERT, pCertFile);

        /* sorry, for engine we must set the passphrase
           (if the key has one...) */
        if (pPassphrase)
            curl_easy_setopt(curl, CURLOPT_KEYPASSWD, pPassphrase);

        /* if we use a key stored in a crypto engine,
           we must set the key type to "ENG" */
        curl_easy_setopt(curl, CURLOPT_SSLKEYTYPE, pKeyType);

        /* set the private key (file or ID in engine) */
        curl_easy_setopt(curl, CURLOPT_SSLKEY, pKeyName);

        /* set the file with the certs validating the server */
        curl_easy_setopt(curl, CURLOPT_CAINFO, pCACertFile);

        /* disconnect if we cannot validate server's cert */
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);

        CURLcode code = curl_easy_perform(curl);
        if (code != CURLE_OK)
        {
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return "";
        }
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return response;
    
}

