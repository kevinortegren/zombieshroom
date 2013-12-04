#pragma once

#include<windows.h>
#include<iosfwd>

namespace ColorCMD
{
	enum ConsoleColor
	{
		BLACK			= 0,
		DARK_BLUE		= 1,
		DARK_GREEN		= 2,
		DARK_AQUA		= 3, 
		DARK_CYAN		= 3,
		DARK_RED		= 4,
		DARK_PURPLE		= 5,
		DARK_PINK		= 5,
		DARK_MANGENTA	= 5,
		DARK_YELLOW		= 6,
		DARK_WHITE		= 7,
		GRAY			= 8,
		BLUE			= 9,
		GREEN			= 10,
		AQUA			= 11,
		CYAN			= 11,
		RED				= 12,
		PURPLE			= 13,
		PINK			= 13,
		MANGENTA		= 13,
		YELLOW			= 14,
		WHITE			= 15
	};

	HANDLE std_con_out;

	//Standard Output Handle
	bool colorprotect = false;

	ConsoleColor textcol, backcol, deftextcol, defbackcol;

	inline void UpdateColors()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(std_con_out,&csbi);
		textcol = ConsoleColor(csbi.wAttributes & 15);
		backcol = ConsoleColor((csbi.wAttributes & 0xf0)>>4);
	}

	inline void SetColor(ConsoleColor p_textcolor ,ConsoleColor p_backcolor)
	{
		if(colorprotect && p_textcolor == p_backcolor)return;
		textcol = p_textcolor;backcol = p_backcolor;
		unsigned short wAttributes = ((unsigned int)backcol<<4) | (unsigned int)textcol;
		SetConsoleTextAttribute(std_con_out,wAttributes);
	}

	inline void SetTextColor(ConsoleColor p_textColor)
	{
		if(colorprotect && p_textColor == backcol)return;
		textcol = p_textColor;
		unsigned short wAttributes=((unsigned int)backcol<<4) | (unsigned int)textcol;
		SetConsoleTextAttribute(std_con_out,wAttributes);
	}

	inline void SetBackColor(ConsoleColor p_backColor)
	{
		if(colorprotect && textcol == p_backColor)return;
		backcol = p_backColor;
		unsigned short wAttributes=((unsigned int)backcol<<4) | (unsigned int)textcol;
		SetConsoleTextAttribute(std_con_out,wAttributes);
	}

	inline void ConsoleColorInit()
	{
		std_con_out = GetStdHandle(STD_OUTPUT_HANDLE);
		ShowWindow( GetConsoleWindow() , SW_MAXIMIZE);
		UpdateColors();
		deftextcol = textcol;
		defbackcol = backcol;
	}

	template<class elem,class traits>
	inline std::basic_ostream<elem,traits>& operator<<(std::basic_ostream<elem,traits>& os, ConsoleColor p_col)
	{
		os.flush();
		SetTextColor(p_col);
		return os;
	}

	template<class elem,class traits>
	inline std::basic_istream<elem,traits>& operator>>(std::basic_istream<elem,traits>& is, ConsoleColor p_col)
	{
		std::basic_ostream<elem,traits>* p=is.tie();
		if(p!=NULL)p->flush();
		SetTextColor(p_col);
		return is;
	}
}

