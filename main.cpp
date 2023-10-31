#include "curl_wrapper.h"
#include "unit_testing.h"
#include <fstream>
#include <json.hpp>
using json = nlohmann::json;


// https://dadosabertos.camara.leg.br/swagger/api.html
void getIDDeputado()
{
	std::vector < std::string> headers = { "Accept: application/json"
											"Content-Type: application/json"
										 };

	std::string url_base = "https://dadosabertos.camara.leg.br/api/v2/";
	std::stringstream os;

	std::string format = "ASC";
	std::string order = "nome";
	std::string name = "Zambelli";
	os << url_base;
	os << "deputados?";
	os << "nome=" << name;
	os << "&ordem=" << format;
	os << "&ordenarPor=" << order;
	CQueryWithJson jquery;
	auto q = os.str();
	jquery.setHeaders(headers);
	jquery.getResponseGet(q);
	auto response = json::parse(jquery.getBuffer());
	std::cout << response.dump(4) << std::endl;
}


int main(int argc, wchar_t* argv[]) 
{
	getIDDeputado();
}
