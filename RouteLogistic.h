#pragma once
#include "Seria1C.h"
#include "Debbuger.h"
#include <deque>

using namespace std;

class RouteLogistic
{
private:

	unordered_map<Object1C*, Object1C*> DependencyList;
	map<std::pair<Object1C*, Object1C*>, Object1C*> DirectRouteList;
	unordered_multimap<Object1C*, Object1C*> ExceptionRouteList;
	unordered_multimap<Object1C*, CostRoute> RelatedList;

	unordered_map<Object1C*, Object1C*> GetAllRoutes(Object1C* cityOut);

	wstring error;

public:

	RouteLogistic(wstring jsonStr);
	RouteLogistic() { };
	~RouteLogistic();
	void Init(wstring jsonStr);
	vector<Object1C*> Route(Object1C* cityOut, Object1C* cityIn);
	bool IsInit();

	const wstring GetErrorString()
	{
		return error;
	}

	const wstring GetDebugString()
	{
		return debbugOut.GetString();
	}

	Seria1C seria1C;

	Debbuger debbugOut;

};
