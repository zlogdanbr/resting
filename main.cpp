#include "curl_wrapper.h"
#include "unit_testing.h"
#include <fstream>
#include <json.hpp>
#include <map>
using json = nlohmann::json;

//----------------------------------------------------------------
// https://dadosabertos.camara.leg.br/swagger/api.html
// "{ \"happy\": true, \"pi\": 3.141 }" or
// std::string s = "{ \"happy\": true, \"pi\": 3.141 
//----------------------------------------------------------------

struct Deputado
{
	std::string id;
	std::string nome;
	std::string partido;
	std::string email;

	friend std::ostream& operator<<(std::ostream& os, const Deputado& d);


};

std::ostream& operator<<(std::ostream& os, const Deputado& d)
{
	os << "Nome: " << d.nome << std::endl;
	os << "ID: " << d.id << std::endl;
	os << "Partido: " << d.partido << std::endl;
	os << "email: " << d.email << std::endl;
	return os;
}

bool getIDDeputado(Deputado& d, std::string&& name_to_query)
{
	std::vector < std::string> headers = { "Accept: application/json"
										   "Content-Type: application/json"
										 };

	std::string url_base = "https://dadosabertos.camara.leg.br/api/v2/";
	std::stringstream os;

	std::string format = "ASC";
	std::string order = "nome";
	std::string name = name_to_query;

	os << url_base;
	os << "deputados?";
	os << "nome=" << name;
	os << "&ordem=" << format;
	os << "&ordenarPor=" << order;
	CHttpClient rest_client;
	auto q = os.str();
	rest_client.setHeaders(headers);


	if (rest_client.getResponseGet(q))
	{
		auto buffer = rest_client.getBuffer();

		if (buffer.size() == 0)
		{
			return false;
		}
		auto response = json::parse(buffer);

		if (response.size() == 0)
		{
			return false;
		}

		try
		{
			for (auto& [key, value] : response.items())
			{
				if (key == "dados")
				{
					if (value.size() == 0)
					{
						return false;
					}
					for (auto& [key2, value2] : value.begin()->items())
					{
						if (key2 == "id")
						{
							std::stringstream dep;
							dep << value2;
							d.id = dep.str();
						}
						if (key2 == "email")
						{
							std::stringstream dep;
							dep << value2;
							d.email = dep.str();
						}
						if (key2 == "nome")
						{
							std::stringstream dep;
							dep << value2;
							d.nome = dep.str();
						}
						if (key2 == "siglaPartido")
						{
							std::stringstream dep;
							dep << value2;
							d.partido = dep.str();
						}
					}
				}
			}
			return true;
		}
		catch(...)
		{
			return false;
		}

	}


	return false;
}

bool ObterGastosDeputado(std::string id)
{
	std::vector < std::string> headers = { "Accept: application/json"
										   "Content-Type: application/json"
	};

	std::string url_base = "https://dadosabertos.camara.leg.br/api/v2/";
	std::stringstream os;

	std::string format = "ASC";
	std::string order = "ano";

	os << url_base;
	os << "deputados/" << id << "/despesas";
	os << "?ordem=" << format;
	os << "&ordenarPor=" << order;

	CHttpClient rest_client;
	auto q = os.str();
	rest_client.setHeaders(headers);


	if (rest_client.getResponseGet(q))
	{
		auto buffer = rest_client.getBuffer();


		auto response = json::parse(buffer);
		std::cout << response.dump(4) << std::endl;

		return true;

	}


	return false;
}


int main(int argc, wchar_t* argv[]) 
{
	Deputado d;
	if (getIDDeputado(d, "samia"))
	{
		std::cout << d;
		ObterGastosDeputado(d.id);
	}
	else
	{
		std::cout << "Error" << std::endl;
	}
}
