#include "curl_wrapper.h"
#include "unit_testing.h"
#include <fstream>
#include <json.hpp>
using json = nlohmann::json;

int main(int argc, wchar_t* argv[]) 
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
