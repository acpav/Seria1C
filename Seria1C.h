#pragma once
#include <string>
#include <rapidjson/reader.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "TabReader1C.h"
#include "HeaderStructSeria1C.h"

using namespace structNSseria1C;

class Seria1C
{
private:
	std::unordered_multimap<std::wstring, Object1C*> Objects;
public:
	Seria1C();
	~Seria1C();
	void GetTabs(wchar_t const* jsonStr, std::vector<TabReader1C*> tabReader);
	Object1C* FindObj(std::wstring t, std::wstring g);
	std::wstring GetJSON(std::vector<Object1C*> m);
	std::wstring GetArrayJSON(std::vector<Object1C*> m);
};
