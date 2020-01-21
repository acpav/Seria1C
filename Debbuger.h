#pragma once
#include <string>

	class Debbuger
	{
	private:
		std::wstring strDebug;
		size_t lineN;
		bool endLine;
	public:

		friend Debbuger& operator<<(Debbuger& d, const std::wstring aStr)
		{
			#ifdef _DEBUG
			if (d.endLine)
			{
				++d.lineN;
				wchar_t buff[15];
				_ultow_s(d.lineN, buff, 14, 10);
				d.strDebug.append(buff);
				d.strDebug.append(L") ");
			}
			d.strDebug.append(aStr);
			d.endLine = (aStr.back() == L'\n');
			#endif
			return d;
		}		

		Debbuger()
		{
			endLine = true;
			lineN = 0;
		};

		~Debbuger() {};

		std::wstring GetString()
		{
			return strDebug;
		}

		void clear()
		{
			strDebug.clear();
			endLine = true;
			lineN = 0;
		}
	};
