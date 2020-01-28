
#include <iostream>
#include <fstream>
#include <codecvt>
#include "RouteLogistic.h"

using namespace std;

int main(char** args)
{

	wifstream is("C:\\Users\\pavel.konev\\new 2.txt", ifstream::in|| ifstream::binary);
//	codecvt_utf8<wchar_t> *cd = new codecvt_utf8<wchar_t>;
	is.imbue(locale("ru_RU", locale::all));
	is.seekg(0, is.end);
	auto lenFile = is.tellg();
	is.seekg(0, is.beg);

	wchar_t* buffer = new wchar_t[(unsigned int)lenFile];

	is.read(buffer, lenFile);

	is.close();

	vector<Object1C*> route;

	Object1C* cityOut, * cityIn;

	RouteLogistic rl;

	//for (size_t cn = 1000; cn > 0; --cn)
	rl.Init(buffer);
	//rl.Init(L"");

	//return 0;

	cityOut = rl.seria1C.FindObj(L"jcfg:CatalogRef.МестаХранения", L"e27718c8-7f15-11e2-93c7-000c29c4d504"); //Бузулук
	cityIn = rl.seria1C.FindObj(L"jcfg:CatalogRef.МестаХранения", L"f5415e57-7f15-11e2-93c7-000c29c4d504"); //Нижняя Тура

	route = rl.Route(cityOut, cityIn);

	auto loc2 = locale("rus_rus.866");
	std::wcout.imbue(loc2);

	//wstring wstr = rl.seria1C.GetJSON(route);

	//std::wcout << wstr << '\n';

	for (auto var : route)
	{
		std::wcout << var->guid.c_str() << '\t' << var->type.c_str() << '\n';
	}
	
	std::wcout << rl.debbugOut.GetString();

}
