#include "curl_wrapper.h"
#include "unit_testing.h"
#include <fstream>
#include <json.hpp>
using json = nlohmann::json;

void doDeezer()
{
	// create an instance of a Deezer object
	CDeezer deezer;

	// choose the artist
	std::string artist = "King%20Crimson";
	deezer.setArtist(artist);

	// do the actual query
	deezer.getArtistInfo();

	// use lib to parse the response
	auto response = json::parse(deezer.getBuffer());

	// pretty print it
	std::cout << response.dump(4) << std::endl;
}


void doQueryWithJson()
{
	std::string url = "";
	// "{ \"happy\": true, \"pi\": 3.141 }" or
	// std::string s = "{ \"happy\": true, \"pi\": 3.141 }";
	std::string sjson = "";
	CQueryWithJson jquery;
	jquery.getResponsePost(url, sjson);
	auto response = json::parse(jquery.getBuffer());

	// pretty print it
	std::cout << response.dump(4) << std::endl;

}


int main(int argc, wchar_t* argv[]) 
{
	doDeezer();
}
