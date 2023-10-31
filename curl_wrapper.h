
#pragma once
// Started using the example from
// https://juhilee.hashnode.dev/configuring-libcurl-in-visual-studio-2022
// I recommend starting from there


#include <iostream>
#include <sstream>
#include <string>
#include <vector>
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

const int _HTTP = 0;
const int _HTTPS = 1;

using sheaders = struct curl_slist;

class RestHandler 
{
public:
	RestHandler() {};
	virtual ~RestHandler() {};

	void clearBuffer() { response.clear(); };
	std::string getBuffer()const { return response; };

	virtual bool send(	const char* url, 
						int secureit = _HTTP,
						const char* method="GET");

	void setHeaders(std::vector< std::string > headers );

protected:
	sheaders* headers = nullptr;
	CURL* curl = nullptr;
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

	void setSecurity() const;
	virtual void setOptions(const char* url, const char* method);
};


// "{ \"happy\": true, \"pi\": 3.141 }" or
// std::string s = "{ \"happy\": true, \"pi\": 3.141 

class CQueryWithJson final : public RestHandler
{
public:
	virtual ~CQueryWithJson() {};
	virtual void setOptions(const char* url, const char* method) override;
	bool getResponsePost(std::string& url, std::string& json);
	bool getResponseGet(std::string& url);
	void setjson_string(std::string j) { json_string = j; };
	std::string getjson_string() const { return json_string;};
private:
	std::string json_string;
};




