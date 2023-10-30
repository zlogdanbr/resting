
#pragma once
// Started using the example from
// https://juhilee.hashnode.dev/configuring-libcurl-in-visual-studio-2022


#include <iostream>
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


std::string send(const char* url);

std::string sendWithHeaders(const char* url);

std::string sendWithHeadersHttps(const char* url);