#pragma once
#include <string>
#include <variant>

namespace structNSseria1C
{
	typedef std::variant<bool, int, unsigned, int64_t, uint64_t, double> SimpleVar;

	struct Object1C
	{
		Object1C()
		{
		};

		Object1C(std::wstring t, std::wstring g) : type(t), guid(g)
		{
		};

		std::wstring type;
		std::wstring guid;

		bool empty()
		{
			return type.empty() || guid.empty() || guid == L"00000000-0000-0000-0000-000000000000";
		}

	};

	struct CostRoute
	{
	private:
		Object1C* node2;
		int cost;
	public:


		CostRoute(Object1C* n, int c) : node2(n), cost(c), end(false)
		{
		}

		bool end{ false};

		Object1C* Node2()
		{
			return node2;
		}

		int Cost()
		{
			return cost;
		}

	};

	struct CostNode
	{
		CostNode(int c) : cost(c), end(false)
		{
		}

		int cost;
		bool end{false};

	};

	struct Var1C {

		Var1C(const Var1C& p)
		{
			tag = p.tag;
			switch (p.tag)
			{
			case Type1C::Int:
				num = p.num;
				break;
			case Type1C::pObj:
				obj = p.obj;
				break;
			case Type1C::wStr:
				str = p.str;
				break;
			case Type1C::Bool:
				b = p.b;
				break;
			default:
				break;
			}
		}

		Var1C(Object1C* v)
		{
			obj = v;
			tag = Type1C::pObj;
		}

		Var1C(int v)
		{
			num = v;
			tag = Type1C::Int;
		}

		Var1C(std::wstring v)
		{
			str = v;
			tag = Type1C::wStr;
		}

		Var1C(bool v)
		{
			b = v;
			tag = Type1C::Bool;
		}

		Var1C(double v)
		{
			d = v;
			tag = Type1C::Double;
		}

		Var1C(int64_t v)
		{
			l = v;
			tag = Type1C::Long;
		}

		Var1C()
		{
			tag = Type1C::Null;
		}

		~Var1C()
		{

		}

		union {
			Object1C* obj;
			std::wstring str;
			int num;
			double d;
			bool b;
			int64_t l;
		};

		enum class Type1C { pObj, wStr, Int, Long, Double, Bool, Null};
		Type1C tag;

	};

	enum class NoteType
	{
		Null,
		Bool,
		Int,
		Uint,
		Int64,
		Uint64,
		Double,
		Number,
		String,
		StartObject,
		Key,
		EndObject,
		StartArray,
		EndArray
	};

	struct MyHandler {
		NoteType type;
		std::wstring data;
		SimpleVar var;
		MyHandler() : type(), data() {}

		bool Null() { type = NoteType::Null; data.clear(); return true; }
		bool Bool(bool b) { type = NoteType::Bool; var = b; return true; }
		bool Int(int i) { type = NoteType::Int; var = i; return true; }
		bool Uint(unsigned u) { type = NoteType::Uint; var = u; return true; }
		bool Int64(int64_t i) { type = NoteType::Int64; var = i; return true; }
		bool Uint64(uint64_t u) { type = NoteType::Uint64; var = u; return true; }
		bool Double(double d) { type = NoteType::Double; var = d; return true; }
		bool RawNumber(const wchar_t* str, size_t length, bool) { type = NoteType::Number; data = std::wstring(str, length); return true; }
		bool String(const wchar_t* str, size_t length, bool) { type = NoteType::String; data = std::wstring(str, length); return true; }
		bool StartObject() { type = NoteType::StartObject; data.clear(); return true; }
		bool Key(const wchar_t* str, size_t length, bool) { type = NoteType::Key; data = std::wstring(str, length); return true; }
		bool EndObject(size_t memberCount) { type = NoteType::EndObject; data = std::to_wstring(memberCount); return true; }
		bool StartArray() { type = NoteType::StartArray; data.clear(); return true; }
		bool EndArray(size_t elementCount) { type = NoteType::EndArray; data = std::to_wstring(elementCount); return true; }

	};

	enum class ModeParsing
	{
		Begin,
		FindRows,
		FindMaps,
		ReadRow,
		ReadType,
		ReadValue,
		NextRow,
		NextMap,
		ReadObject,
		End
	};

	enum class TypeStruct1C
	{
		table,
		map
	};

}