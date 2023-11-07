#include "camara.h"


using json = nlohmann::json;

std::ostream& operator<<(std::ostream& os, const Deputado& d)
{
	os << "Nome: " << d.nome << std::endl;
	os << "ID: " << d.id << std::endl;
	os << "Partido: " << d.partido << std::endl;
	os << "email: " << d.email << std::endl;
	return os;
}


bool getIDDeputado(Deputado& d, std::string& name_to_query, CHttpClient& rest_client)
{
	std::vector < std::string> headers = { "Accept: application/json",
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
		catch (...)
		{
			return false;
		}

	}


	return false;
}

bool updateCounters(std::map<std::string, float>& expenses, std::string& key, std::string& v)
{
	if (expenses.empty() == false)
	{
		auto _currentit = expenses.find(key);
		if (_currentit != expenses.end())
		{
			_currentit->second = _currentit->second + std::stof(v);
			return true;
		}
	}
	else
	{
		expenses[key] = std::stof(v);
		return true;
	}


	return false;
}

bool ObterGastosDeputado(std::string id, std::map<std::string, float>& expenses, CHttpClient& rest_client)
{
	std::vector < std::string> headers = { "Accept: application/json",
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
			auto dados = response["dados"];
			for (auto& [key, value] : dados.items())
			{
				std::string ano = "";
				std::string fvalue = "";

				for (json::iterator it = value.begin(); it != value.end(); ++it)
				{
					if (it.key() == "ano")
					{
						std::stringstream y;
						y << it.value();
						ano = y.str();
					}
					if (it.key() == "valorLiquido")
					{
						std::stringstream v;
						v << it.value();
						fvalue = v.str();
					}
				}
				updateCounters(expenses, ano, fvalue);
			}
		}
		catch (...)
		{
			return false;
		}

		return true;
	}


	return false;
}


void Denuncia()
{
	std::string dep;
	std::cout << "Digite o nome do deputado: ";
	std::getline(std::cin, dep);
	Deputado d;
	CHttpClient rest_client;
	if (getIDDeputado(d, dep, rest_client))
	{
		std::cout << d;

		std::map<std::string, float> expenses;
		ObterGastosDeputado(d.id, expenses, rest_client);
		for (const auto& gasto_ano : expenses)
		{
			std::cout << "Gastos no ano de " << gasto_ano.first << " : " << gasto_ano.second << " reais" << std::endl;
		}
	}
	else
	{
		std::cout << "Error" << std::endl;
	}
}