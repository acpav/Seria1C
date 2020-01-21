#pragma once
#include <unordered_map>
#include <map>
#include <vector>
#include "HeaderStructSeria1C.h"

using namespace structNSseria1C;

class TabReader1C
{
protected:
	TypeStruct1C type;
public:
	TabReader1C()
	{
		type = TypeStruct1C::table;
	};

	TypeStruct1C GetType()
	{
		return type;
	};

	virtual void ReadRow(std::vector<Var1C> cols) = 0;
	virtual bool CheckRow(std::vector<Var1C> cols) = 0;

	std::wstring nameStruct;
};

class RelatedListReader : public TabReader1C
{
private:
	std::unordered_multimap<Object1C*, CostRoute>* mmap;

public:

	RelatedListReader(std::wstring nameS, std::unordered_multimap<Object1C*, CostRoute>* m)
	{
		nameStruct = nameS;
		mmap = m;
	}

	void ReadRow(std::vector<Var1C> cols)
	{
		if (CheckRow(cols))
		{
			CostRoute costD(cols[1].obj, cols[2].num);
			mmap->emplace( cols[0].obj, costD );
		}
	}

	bool CheckRow(std::vector<Var1C> cols)
	{
		return
			cols.size() == 3 &&
			cols[0].tag == Var1C::Type1C::pObj &&
			cols[1].tag == Var1C::Type1C::pObj &&
			cols[2].tag == Var1C::Type1C::Int;
	}

};

class ExceptionRouteListReader : public TabReader1C
{
private:
	std::unordered_multimap<Object1C*, Object1C*>* mmap;

public:

	ExceptionRouteListReader(std::wstring nameS, std::unordered_multimap<Object1C*, Object1C*>* m)
	{
		nameStruct = nameS;
		mmap = m;
	}

	void ReadRow(std::vector<Var1C> cols)
	{
		if (CheckRow(cols))
		{
			mmap->emplace(cols[0].obj, cols[1].obj);
		}
	}

	bool CheckRow(std::vector<Var1C> cols)
	{
		return
			cols.size() == 2 &&
			cols[0].tag == Var1C::Type1C::pObj &&
			cols[1].tag == Var1C::Type1C::pObj;
	}

};

class DirectRouteListReader : public TabReader1C
{
private:
	std::map<std::pair<Object1C*, Object1C*>, Object1C*>* mmap;

public:

	DirectRouteListReader(std::wstring nameS, std::map<std::pair<Object1C*, Object1C*>, Object1C*>* m)
	{
		nameStruct = nameS;
		mmap = m;
	}

	void ReadRow(std::vector<Var1C> cols)
	{
		if (CheckRow(cols))
		{
			mmap->emplace(std::make_pair(cols[0].obj, cols[1].obj), cols[2].obj);
		}
	}

	bool CheckRow(std::vector<Var1C> cols)
	{
		return
			cols.size() == 3 &&
			cols[0].tag == Var1C::Type1C::pObj &&
			cols[1].tag == Var1C::Type1C::pObj &&
			cols[2].tag == Var1C::Type1C::pObj;
	}

};

class DependencyListReader : public TabReader1C
{
private:
	std::unordered_map<Object1C*, Object1C*>* mmap;

public:

	DependencyListReader(std::wstring nameS, std::unordered_map<Object1C*, Object1C*>* m)
	{
		nameStruct = nameS;
		mmap = m;
		TabReader1C::type = TypeStruct1C::map;
	}

	void ReadRow(std::vector<Var1C> cols)
	{
		if (CheckRow(cols))
		{
			mmap->emplace(cols[0].obj, cols[1].obj);
		}
	}

	bool CheckRow(std::vector<Var1C> cols)
	{
		return
			cols.size() == 2 &&
			cols[0].tag == Var1C::Type1C::pObj &&
			cols[1].tag == Var1C::Type1C::pObj;
	}

};
