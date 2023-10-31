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
        setOptions(curl, url, method);

        if (secureit != NO_HEADERS_HTTPS)
        {
            setSecurity(curl);
        }

        CURLcode code = curl_easy_perform(curl);

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

void RestHandler::setOptions(CURL* curl, const char* url, const char* method )
{
    std::cout << "\nCalling base setOptions" << std::endl;
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
}

void RestHandler::setHeaders()
{
    std::cout << "\nCalling base setHeaders" << std::endl;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charset: utf-8");
}


void RestHandler::setSecurity(CURL* curl) const
{
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
}

void CDeezer::setHeaders()
{
    std::cout << "\nCalling override setHeaders" << std::endl;
    headers = curl_slist_append(headers, "Content-Type: application/json");
}

bool CDeezer::getArtistInfo()
{   
    std::stringstream os;
    os << "https://api.deezer.com/search?q=";
    os << getArtist();
    os << "&output=json";
    std::cout << os.str();
    return send(os.str().c_str(), 1);    
}


void CQueryWithJson::setOptions(    CURL* curl, 
                                    const char* url, 
                                    const char* method)
{
    std::cout << "\nCalling override setOptions" << std::endl;
    std::string smeth = method;

    if (smeth == "POST")
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, getjson_string().c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, getjson_string().length());
        curl_easy_setopt(curl, CURLOPT_POST, 1);
    }
    else
    if (smeth == "GET")
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
    }
}


bool CQueryWithJson::getResponsePost(std::string& url, std::string& json)
{
    setjson_string(json);
    std::stringstream os;
    os << url;
    os << "&output=json";
    return send(os.str().c_str(), 1, 0, "POST");
}


bool CQueryWithJson::getResponseGet(std::string& url)
{
    std::stringstream os;
    os << url;
    os << "&output=json";
    return send(os.str().c_str());
}