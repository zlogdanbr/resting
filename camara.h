#pragma once
#include "curl_wrapper.h"
#include "unit_testing.h"
#include <fstream>
#include <json.hpp>
#include <map>

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


bool getIDDeputado(Deputado& d, std::string& name_to_query, CHttpClient& rest_client);
bool updateCounters(std::map<std::string, float>& expenses, std::string& key, std::string& v);
bool ObterGastosDeputado(std::string id, std::map<std::string, float>& expenses, CHttpClient& rest_client);
void Denuncia();
