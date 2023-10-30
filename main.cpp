#include "curl_wrapper.h"
#include "unit_testing.h"

int main(int argc, wchar_t* argv[]) 
{
	CDeezer deezer;
	std::string artist = "ac/dc";
	deezer.setArtist(artist);
	deezer.getArtistInfo();
	std::cout << deezer.getBuffer();
}
