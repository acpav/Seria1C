#include "Seria1C.h"
#include <clocale>

Seria1C::Seria1C()
{
}

Seria1C::~Seria1C()
{
}

void Seria1C::GetTabs(wchar_t const* jsonStr, std::vector<TabReader1C*> tabReader)
{
	MyHandler handler;
	rapidjson::GenericReader<rapidjson::UTF16<wchar_t>, rapidjson::UTF16<wchar_t>> reader;
	rapidjson::GenericStringStream<rapidjson::UTF16<wchar_t>> ss(jsonStr);
	reader.IterativeParseInit();

	std::wstring curType;

	std::vector<Var1C> cols;

	TabReader1C* curTabReader = nullptr;

	ModeParsing mode = ModeParsing::Begin;
	while (!reader.IterativeParseComplete() && mode != ModeParsing::End)
	{
		reader.IterativeParseNext<rapidjson::kParseDefaultFlags>(ss, handler);

		switch (mode)
		{
		case ModeParsing::Begin:

			curTabReader = nullptr;

			if (handler.type == NoteType::String)
			{
				for (auto var : tabReader)
				{
					if (var->nameStruct == handler.data)
					{
						curTabReader = var;
						if (curTabReader->GetType() == TypeStruct1C::table)
							mode = ModeParsing::FindRows;
						else if (curTabReader->GetType() == TypeStruct1C::map)
							mode = ModeParsing::FindMaps;

						break;
					}
				}
			}
			break;
		case ModeParsing::FindMaps:
			if (handler.type == NoteType::String && handler.data == L"jv8:Map")
			{
				mode = ModeParsing::NextMap;
			}
			break;
		case ModeParsing::FindRows:
			if (handler.type == NoteType::Key && handler.data == L"row")
			{
				mode = ModeParsing::NextRow;
			}
			break;
		case ModeParsing::NextRow:
			if (handler.type == NoteType::StartArray)
			{
				mode = ModeParsing::ReadRow;
			}
			else if (handler.type == NoteType::EndArray)
			{
				mode = ModeParsing::Begin;
			}
			break;
		case ModeParsing::ReadRow:
			if (handler.type == NoteType::StartObject)
			{
				mode = ModeParsing::ReadObject;
			}
			else if (curTabReader != nullptr)
			{
				if (curTabReader->GetType() == TypeStruct1C::table)
				{
					if (handler.type == NoteType::EndArray)
					{
						curTabReader->ReadRow(cols);
						cols.clear();
						mode = ModeParsing::NextRow;
					}
				}
				else if (curTabReader->GetType() == TypeStruct1C::map)
					if (handler.type == NoteType::EndObject)
					{
						curTabReader->ReadRow(cols);
						cols.clear();
						mode = ModeParsing::NextMap;
					}
			} else
				mode = ModeParsing::End;
			break;
		case ModeParsing::NextMap:
			if (handler.type == NoteType::StartObject)
			{
				mode = ModeParsing::ReadRow;
			}
			else if (handler.type == NoteType::EndArray)
			{
				mode = ModeParsing::Begin;
			}
			break;
		case ModeParsing::ReadObject:
			if (handler.type == NoteType::Key && handler.data == L"#type")
			{
				mode = ModeParsing::ReadType;
			}
			else if (handler.type == NoteType::Key && handler.data == L"#value")
			{
				mode = ModeParsing::ReadValue;
			}
			else if (handler.type == NoteType::EndObject)
			{
				mode = ModeParsing::ReadRow;
			}
			break;
		case ModeParsing::ReadType:
			if (handler.type == NoteType::String)
			{
				curType = handler.data;
			}
			mode = ModeParsing::ReadObject;
			break;
		case ModeParsing::ReadValue:
			if (handler.type == NoteType::String)
			{
				cols.push_back((Var1C)FindObj(curType, handler.data));
			}
			else if (handler.type == NoteType::Int || handler.type == NoteType::Uint || handler.type == NoteType::Uint64)
			{
				cols.push_back((Var1C)_wtoi(handler.data.c_str()));
			}
			mode = ModeParsing::ReadObject;
			break;
		case ModeParsing::End:

			break;
		default:
			break;
		}
	}
}

Object1C* Seria1C::FindObj(std::wstring t, std::wstring g)
{
	if (t.empty())
		return new Object1C;

	auto pairIt = Objects.equal_range(g);
	for (auto var = pairIt.first; var != pairIt.second; ++var)
	{
		if (var->second->type == t)
		{
			return var->second;
		}
	}

	Object1C* newObj = new Object1C(t, g);
	this->Objects.emplace(g, newObj);

	return newObj;
}

std::wstring Seria1C::GetJSON(std::vector<Object1C*> m)
{
	rapidjson::GenericStringBuffer<rapidjson::UTF16<wchar_t>> s;
	rapidjson::Writer<rapidjson::GenericStringBuffer<rapidjson::UTF16<wchar_t>>, rapidjson::UTF16<wchar_t>, rapidjson::UTF16<wchar_t>> json(s);

	json.StartObject();

	json.Key(L"#type");
	json.String(L"jv8:ValueListType");

		json.Key(L"#value");
		json.StartObject();		
			json.Key(L"valueType");
			json.StartObject();
			json.EndObject();

			json.Key(L"lastId");
			json.StartObject();
				json.Key(L"#type");
				json.String(L"jxs:decimal");
				json.Key(L"#value");
				json.Int(m.size() - 1);
			json.EndObject();

			json.Key(L"item");
			json.StartArray();

			int cn = 0;
			for (auto var : m)
			{
				json.StartObject();
					json.Key(L"value");
					json.StartObject();
						json.Key(L"#type");
						json.String(var->type.c_str());
						json.Key(L"#value");
						json.String(var->guid.c_str());
					json.EndObject();

					json.Key(L"id");
					json.StartObject();
						json.Key(L"#type");
						json.String(L"jxs:decimal");
						json.Key(L"#value");
						json.Int(cn++);
					json.EndObject();

				json.EndObject();
			}

			json.EndArray();
		json.EndObject();
	json.EndObject();

	return s.GetString();
}

std::wstring Seria1C::GetArrayJSON(std::vector<Object1C*> m)
{
	rapidjson::GenericStringBuffer<rapidjson::UTF16<wchar_t>> s;
	rapidjson::Writer<rapidjson::GenericStringBuffer<rapidjson::UTF16<wchar_t>>, rapidjson::UTF16<wchar_t>, rapidjson::UTF16<wchar_t>> json(s);

	json.StartObject();

	json.Key(L"#type");
	json.String(L"jv8:Array");

	json.Key(L"#value");
	json.StartArray();

	int cn = 0;
	for (auto var : m)
	{
		json.StartObject();
		json.Key(L"#type");
		json.String(var->type.c_str());
		json.Key(L"#value");
		json.String(var->guid.c_str());
		json.EndObject();
	}

	json.EndArray();
	json.EndObject();

	return s.GetString();
}
