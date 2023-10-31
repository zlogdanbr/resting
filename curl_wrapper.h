
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


protected:
	sheaders* headers = nullptr;
	std::string response;

	static int writer(char* data, size_t size, size_t nmemb, std::string* writerData)
	{
		if (writerData == nullptr)
			return 0;

		writerData->append(data, size * nmemb);

		return static_cast<int>(size * nmemb);
	}

private:
	RestHandler(RestHandler&) = delete;
	RestHandler operator=(RestHandler&) = delete;
	const char* pPassphrase = nullptr;
	const char* pCertFile = "testcert.pem";
	const char* pCACertFile = "cacert.pem";
	const char* pHeaderFile = "dumpit";
	const char* pKeyName = "testkey.pem";
	const char* pKeyType = "PEM";

	void setSecurity(CURL* curl) const;
	virtual void setHeaders();
	virtual void setOptions(CURL* curl, const char* url, const char* method);
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

class CQueryWithJson : public RestHandler
{
public:
	virtual ~CQueryWithJson() {};
	virtual void setOptions(CURL* curl, const char* url, const char* method) override;
	bool getResponsePost(std::string& url, std::string& json);
	bool getResponseGet(std::string& url);
	void setjson_string(std::string j) { json_string = j; };
	std::string getjson_string() const { return json_string;};
private:
	std::string json_string;
};




