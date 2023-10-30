
#pragma once
// Started using the example from
// https://juhilee.hashnode.dev/configuring-libcurl-in-visual-studio-2022
// I recommend starting from there


#include <iostream>
#include <sstream>
#include <string>
#define CURL_STATICLIB
#include "curl/curl.h"

#ifdef _DEBUG
#    pragma comment (lib,"libcurl_a_debug.lib")
#else
#    pragma comment (lib,"libcurl_a.lib")
#endif // _DEBUG

/*Windows Specific Additional Depenedencies*/
#pragma comment (lib,"Normaliz.lib")
#pragma comment (lib,"Ws2_32.lib")
#pragma comment (lib,"Wldap32.lib")
#pragma comment (lib,"Crypt32.lib")

const int NO_HEADERS_HTTP = 0;
const int HEADERS_HTTP = 1;
const int NO_HEADERS_HTTPS = 0;
const int HEADERS_HTTPS = 1;

using sheaders = struct curl_slist;

class RestHandler 
{
public:
	RestHandler() {};
	virtual ~RestHandler() {};

	void clearBuffer() { response.clear(); };
	std::string getBuffer()const { return response; };

	virtual bool send(	const char* url, 
						int opt = NO_HEADERS_HTTP, 
						int secureit = NO_HEADERS_HTTPS,
						const char* method="GET");

	virtual void setHeaders()
	{
		headers = curl_slist_append(headers, "Accept: application/json");
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "charset: utf-8");
	}

protected:
	sheaders* headers = nullptr;
private:
	RestHandler(RestHandler&) = delete;
	RestHandler operator=(RestHandler&) = delete;
	std::string response;


	const char* pPassphrase = nullptr;
	const char* pCertFile = "testcert.pem";
	const char* pCACertFile = "cacert.pem";
	const char* pHeaderFile = "dumpit";
	const char* pKeyName = "testkey.pem";
	const char* pKeyType = "PEM";

	static int writer(char* data, size_t size, size_t nmemb, std::string* writerData)
	{
		if (writerData == nullptr)
			return 0;

		writerData->append(data, size * nmemb);

		return (size * nmemb);
	}

	void setSecurity(CURL* curl) const
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
};


class CDeezer final : public RestHandler
{
public:
	virtual ~CDeezer() {};
	bool getArtistInfo();
	void setArtist(std::string& art) { artist = art; };
	std::string getArtist()const { return artist; };
	virtual void setHeaders() override;
private:
	std::string artist;
};