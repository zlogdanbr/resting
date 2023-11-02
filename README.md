# resting

##Introduction

Why I did this? A friend of mine and I were asked to come up with an app for testing a REST API.
This was for a job position. Also, although REST is not usually used with C++, there are some companies that required this knowledge.

##Details

This project implements an Httpclient, either using or not HTTPs.

I have used this excellent tutorial to start with libcurl:

https://juhilee.hashnode.dev/configuring-libcurl-in-visual-studio-2022

It explains in details how to build libcurl and also how to use with Visual Studio 2022.

I just wrapped everything inside a virtual class and implemented a new one.

The reason for that is to allow people to customize

virtual void setOptions(const char* url, const char* method) = 0;

to set libcurl options.

For parsing the Json response I have used

https://github.com/nlohmann/json:

Another great resources for libcurl are:

https://curl.se/libcurl/
https://curl.se/libcurl/c/libcurl-tutorial.html
https://everything.curl.dev/libcurl/getinfo

I used the api:

https://dadosabertos.camara.leg.br/swagger/api.html#api

to calculate the expenses for a specific Brazilian federal congressman.

The only drawback I haven't been yet able to figure out is that the API uses UTF-16 and the json library only handles UTF-8.
As Portuguese contains non UTF-8 characters, there might be some problems but I try to handle them in code.



_________________________________________________
**2023 Daniel Vasconcelos Gomes**
**You can use this freely but I don't take responsibility for whatever problems this will likely cause.**

