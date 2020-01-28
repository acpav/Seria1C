#include "RouteLogistic.h"

unordered_map<Object1C*, Object1C*> RouteLogistic::GetAllRoutes(Object1C* cityOut)
{
	const int maxCost = 99999999;

	unordered_map<Object1C*, Object1C*> RouteList;

	unordered_map<Object1C*, CostNode> costNodeList;
	unordered_set<Object1C*> nodesInWork;

	for (auto var = RelatedList.begin(); var != RelatedList.end(); ++var)
	{
		var->second.end = false;
		costNodeList.insert({ var->first, CostNode(maxCost) });
		costNodeList.insert({ var->second.Node2(), CostNode(maxCost) });
	}

	auto curNode = costNodeList.find(cityOut);

	if (curNode == costNodeList.end()) return RouteList;

	curNode->second.cost = 0;

	while (curNode != costNodeList.end())
	{
		curNode->second.end = true;
		int curCostNode = curNode->second.cost;

		auto RelCitysRange = RelatedList.equal_range(curNode->first);

		for (auto RelCitys = RelCitysRange.first; RelCitys != RelCitysRange.second; ++RelCitys)
		{
			if (RelCitys->second.end) continue;

			auto costCity = costNodeList.find(RelCitys->second.Node2());
			int cost = RelCitys->second.Cost();

			if (costCity->second.cost > curCostNode + cost)
			{
				costCity->second.cost = curCostNode + cost;
				if (!costCity->second.end)
					nodesInWork.insert(costCity->first);
				RouteList[RelCitys->second.Node2()] = curNode->first;
			}
			RelCitys->second.end = true;
		}

		if (nodesInWork.empty())
			curNode = costNodeList.end();
		else
		{
			curNode = costNodeList.find(*nodesInWork.begin());
			nodesInWork.erase(nodesInWork.begin());
		}
	}

	return RouteList;
}

RouteLogistic::RouteLogistic(wstring jsonStr)
{
	Init(jsonStr);
}

RouteLogistic::~RouteLogistic()
{
}

void RouteLogistic::Init(wstring jsonStr)
{
	DependencyList.clear();
	DirectRouteList.clear();
	ExceptionRouteList.clear();
	RelatedList.clear();

	vector<TabReader1C*> tabReader1C;
	tabReader1C.push_back(new DependencyListReader(L"СписокПодчиненных", &DependencyList));
	tabReader1C.push_back(new DirectRouteListReader(L"ТабПрямыхУчастков", &DirectRouteList));
	tabReader1C.push_back(new ExceptionRouteListReader(L"МаршрутыИсключения", &ExceptionRouteList));
	tabReader1C.push_back(new RelatedListReader(L"СписокСмежности", &RelatedList));

	seria1C.GetTabs(jsonStr.c_str(), tabReader1C);
}

vector<Object1C*> RouteLogistic::Route(Object1C* cityOut, Object1C* cityIn)
{

	Object1C *curCityOut, *curCityIn;

	curCityOut = cityOut;
	curCityIn = cityIn;

	deque<Object1C*> tmpRouteIn, tmpRoute, tmpRouteOut;

	const int limitIn = 4;
	const int limitCn = 15;
	int cn = 0;

	debbugOut.clear();
	debbugOut << L"Начало: " << cityOut->guid << L" - " << cityIn->guid << L"\n";

	while (!curCityOut->empty())
	{
		if (++cn > limitCn)
		{
			error = L"Зацикливание маршрута из " + cityIn->guid + L" до " + cityOut->guid;
			break;
		}

		bool isFind = false;
		int cnIn = 0;

		while (!curCityIn->empty() && ++cnIn <= limitIn && curCityOut != curCityIn)
		{

			tmpRouteIn.push_back(curCityIn);
			tmpRoute.clear();

			auto val = DirectRouteList.find(make_pair(curCityOut, curCityIn));

			if (val != DirectRouteList.end())
			{
				tmpRoute.push_front(curCityOut);
				if (!val->second->empty())
				{
					tmpRoute.push_front(val->second);
				}
				isFind = true;
				break;
			}

			auto mD = DependencyList.find(curCityIn);

			if (mD != DependencyList.end())
				curCityIn = mD->second;
			else
				curCityIn = new Object1C();
		}

		if (isFind || curCityIn == curCityOut)
		{
			if (curCityIn == curCityOut)
			{
				tmpRoute.push_back(curCityIn);
			}
			break;
		}

		tmpRoute.clear();
		tmpRouteIn.clear();
		curCityIn = cityIn;

		tmpRouteOut.push_front(curCityOut);

		auto mD = DependencyList.find(curCityOut);

		if (mD != DependencyList.end())
			curCityOut = mD->second;
		else
			curCityOut = new Object1C();

	}

	vector<Object1C*> route;

	while (!tmpRouteOut.empty())
	{
		route.push_back(tmpRouteOut.back());
		tmpRouteOut.pop_back();
	}

	while (!tmpRoute.empty())
	{
		route.push_back(tmpRoute.back());
		tmpRoute.pop_back();
	}

	while (!tmpRouteIn.empty())
	{
		route.push_back(tmpRouteIn.back());
		tmpRouteIn.pop_back();
	}

	debbugOut << L"Найдено:" << L"\n";
	for (auto var : route)
	{
		debbugOut << var->guid << L"\n";
	}

	size_t ctR = route.size();
	if (ctR > 2)
	{
		size_t b = ctR - 2;
		for (size_t a = 0; a <= ctR - 2; a++)
		{
			while (b > a + 1)
			{
				auto mDir = DirectRouteList.find(make_pair(route.at(a), route.at(b)));
				if (mDir != DirectRouteList.end())
				{
					debbugOut << L"Спрямлен маршрут: " << route.at(a)->guid << L" - " << route.at(b)->guid << L"\n";
					route.erase(route.begin() + a + 1, route.begin() + b);
					if (!mDir->second->empty())
					{
						route.insert(route.begin() + a + 1, mDir->second);
					}
					a = ctR;
					break;
				}
				b--;
			}
			b = ctR - 1;
		}
	}

	if (route.front() != cityOut || route.back() != cityIn)
	{
		route.clear();
	}

	if (route.empty()) //Считаем по старому
	{
		debbugOut << L"Считаем по старому" << L"\n";

		auto RouteList = GetAllRoutes(cityOut);

		auto curCity = cityIn;

		route.push_back(curCity);

		while (curCity != cityOut)
		{
			auto tR = RouteList.find(curCity);
			if (tR == RouteList.end())
			{
				route.clear();
				break;
			}
			route.insert(route.begin(), tR->second);
			curCity = tR->second;
		}
	}

	//+Маршруты исключения
	for (auto it = route.begin(); it != route.end(); ++it)
	{
		if ((*it)->type == L"jcfg:CatalogRef.Маршруты")
		{
			debbugOut << L"Найдено исключение: " << (*it)->guid << L"\n";

			auto pairER = ExceptionRouteList.equal_range(*it);
			--pairER.second;
			route.erase(it--);
			for (auto mER = pairER.first; mER != ExceptionRouteList.end() && mER != pairER.second; ++mER)
				it = route.insert(it+1, mER->second);
		}
	}
	//-Маршруты исключения

	return route;
}

bool RouteLogistic::IsInit()
{
	return !RelatedList.empty() && !DependencyList.empty() && !DirectRouteList.empty() && !ExceptionRouteList.empty();
}
