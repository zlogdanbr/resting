#include "curl_wrapper.h"
#include "unit_testing.h"

int main(int argc, wchar_t* argv[]) 
{
	RestHandler r;
	r.send("https://reqres.in/api/users?page=2");
	std::cout << r.getBuffer();
}
