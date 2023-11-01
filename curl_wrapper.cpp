#include "curl_wrapper.h"

bool CBaseHttpHandler::send(const char* url, int secureit, const char* method)
{
    clearBuffer();
  
    if ( nullptr == curl )
    {
        curl = curl_easy_init();
        curl_global_init(CURL_GLOBAL_ALL);
    }

    if (curl)
    {
        setOptions(url, method);

        if (secureit == _HTTPS)
        {
            setSecurity();
        }

        CURLcode code = curl_easy_perform(curl);

        if (code != CURLE_OK)
        {
            return false;
        }
       
    }
    
    return true;
}

void CBaseHttpHandler::setHeaders(std::vector< std::string >& _headers)
{
    for (auto& h : _headers)
    {
        headers = curl_slist_append(headers, h.c_str());
    }
}

void CBaseHttpHandler::setSecurity() const
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


void CHttpClient::setOptions(  const char* url, const char* method)
{

    std::string smeth = method;

    if (smeth == "POST")
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, getParamsString().c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, getParamsString().length());
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


bool CHttpClient::getResponsePost(std::string& url, std::string& params)
{
    setParamsString(params);
    std::stringstream os;
    os << url;
    return send(os.str().c_str(), 0, "POST");
}


bool CHttpClient::getResponseGet(std::string& url)
{
    std::stringstream os;
    os << url;
    return send(os.str().c_str());
}
