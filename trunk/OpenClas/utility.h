/*********************************************************************************
	Copyright 2007 Dancefire (dancefire@gmail.com).
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright
	   notice, this list of conditions and the following disclaimer in the
	   documentation and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
	ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
	OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
	HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
	OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
	SUCH DAMAGE.


	��Ȩ���� 2007 Dancefire (dancefire@gmail.com)��
	��������Ȩ����

	����������������ǰ���£��������·ַ��޸Ĺ���δ���޸ĵģ���Դ������ѱ�����ʽ
	���ڵı������

	1. ��Դ������ʽ���·������뱣��δ���޸ĵ�������Ȩ����������������Լ�������
	   ��������
	2. ���ѱ�����ʽ���·��������ڷ����汾���ĵ���/������ͬʱ��������������������
	   ��Ȩ����������������Լ���������������

	����������߼������� ������״��ʽ��(AS-IS) �ṩ���ڴ���ʾ���ṩ�κΰ�����������
	�����ض�Ŀ�ĵ������ԡ�ʵ���Ե���ʾ��ʾ�ĵ��������κ�����£����߼������߻���
	�������߽Բ�������ʹ�ô������ɵ�ֱ�ӡ���ӡ��������ر𡢳ͽ�Ļ������ɵĵ�
	��(���������޶��ڻ������Ｐ�����޷�ʹ�á����ݶ�ʧ����ʧӯ���������ж�)��
	�����������κ������ʹ��ʱ�������������ɵġ����ں��������ƶϡ��Ƿ����ں�
	ͬ���롢�ϸ��⳥���λ�������Ȩ��Ϊ(�������������ԭ��)����ʹԤ�ȱ���֪������
	�����Ŀ����ԡ�
	
**********************************************************************************/
/*
*	$Date$
*	$Revision$
*	$Author$
*	$Id$
*/

#pragma once

#pragma warning( disable:4996 )

#include <string>
#include <locale>
#include <ctime>
#include <iostream>


namespace ictclas{

	static clock_t tick_load_locale;
	static clock_t tick_set_global;
	static clock_t tick_convert;
	static clock_t tick_pre_convert;
	static clock_t tick_new;
	static clock_t tick_create_wstring;
	static clock_t tick_delete;
	static clock_t tick_set_global_prev;

	class Encoding{
	public:
		enum code_page : unsigned int {
			C = 0,
			ANSI = 1250,
			GB2312 = 20936,
			GBK = 936,
			GB18030 = 54936,
			BIG5 = 950,
			UCS2_LE = 1200,
			UCS2_BE = 1201,
			UCS4_LE = 12000,
			UCS4_BE = 12001,
			UTF7 = 65000,
			UTF8 = 65001,
		};

		static const char* get_codepage_string(code_page cp)
		{
#ifdef WIN32
			const char* CP_STR_C = "C";
			const char* CP_STR_ANSI = "";
			const char* CP_STR_GB2312 = ".20936";
			const char* CP_STR_GBK = ".936";
			const char* CP_STR_GB18030 = ".54936";
			const char* CP_STR_BIG5 = ".950";
			const char* CP_STR_UCS2_LE = ".1200";
			const char* CP_STR_UCS2_BE = ".1201";
			const char* CP_STR_UCS4_LE = ".12000";
			const char* CP_STR_UCS4_BE = ".12001";
			const char* CP_STR_UTF7 = ".65000";
			const char* CP_STR_UTF8 = ".65001";
#else
			const char* CP_STR_C = "C";
			const char* CP_STR_ANSI = "";
			const char* CP_STR_GB2312 = "GB2312";
			const char* CP_STR_GBK = "GBK";
			const char* CP_STR_GB18030 = "GB18030";
			const char* CP_STR_BIG5 = "BIG5";
			const char* CP_STR_UCS2_LE = "CP1200";
			const char* CP_STR_UCS2_BE = "CP1201";
			const char* CP_STR_UCS4_LE = "CP12000";
			const char* CP_STR_UCS4_BE = "CP12001";
			const char* CP_STR_UTF7 = "UTF7";
			const char* CP_STR_UTF8 = "UTF8";
#endif
			switch(cp)
			{
				case ANSI:
					return CP_STR_ANSI;
				case GB2312:
					return CP_STR_GB2312;
				case GBK:
					return CP_STR_GBK;
				case GB18030:
					return CP_STR_GB18030;
				case BIG5:
					return CP_STR_BIG5;
				case UCS2_LE:
					return CP_STR_UCS2_LE;
				case UCS2_BE:
					return CP_STR_UCS2_BE;
				case UCS4_LE:
					return CP_STR_UCS4_LE;
				case UCS4_BE:
					return CP_STR_UCS4_BE;
				case UTF7:
					return CP_STR_UTF7;
				case UTF8:
					return CP_STR_UTF8;
				default:
					return CP_STR_C;
			}
		}

		static locale get_locale(code_page cp)
		{
			return locale(get_codepage_string(cp));
		}
		static locale setlocale(code_page cp)
		{
			return locale::global( get_locale(cp) );
		}
		static locale setlocale(const locale& loc)
		{
			return locale::global(loc);
		}
		static wstring to_wstring(const char* lpstr, size_t len)
		{
			if(lpstr == NULL)
			{
				return wstring();
			}
			clock_t tick;
			//DWORD tick = GetTickCount();
			//locale loc(get_codepage_string(cp));
			//tick_load_locale += (GetTickCount() - tick);

			//tick = GetTickCount();
			//locale prev_loc = locale::global(loc);
			//tick_set_global += (GetTickCount() - tick);

			tick = clock();
			size_t size = mbstowcs(0, lpstr, len);
			tick_pre_convert = (clock() - tick);


			if (size == 0){
				//locale::global(prev_loc);
				return wstring();
			}else{

				tick = clock();
				wchar_t* buf = new wchar_t[size+1];
				tick_new += (clock() - tick);

				tick = clock();
				mbstowcs(buf, lpstr, size);
				tick_convert += (clock() - tick);

				buf[size] = 0;

				tick = clock();
				wstring ret(buf);
				tick_create_wstring += (clock() - tick);

				tick = clock();
				delete buf;
				tick_delete += (clock() - tick);

				//tick = clock();
				//locale::global(prev_loc);
				//tick_set_global_prev += (clock() - tick);

				return ret;
			}
		}
		static wstring to_wstring(const string& str)
		{
			const char* lpstr = str.c_str();
			size_t len = str.length();
			//if(cp == UTF8 && str[0] == (char)0xEF){
			//	//	remove the leading endian order char 0xEF
			//	++lpstr;
			//	--len;
			//}
			return to_wstring(lpstr, len);
		}
		static string to_string(const wstring& str, const locale& loc)
		{
			typedef codecvt<wchar_t, char, mbstate_t> codecvt_t;
			const codecvt_t& cc = use_facet<codecvt_t>(loc);
			mbstate_t state = mbstate_t();
			
			int buf_size = static_cast<int>(str.length() * 2);
			char* buf = new char[buf_size+1];	//	FIXME: it's hardcode : "* 2"

			const wchar_t* char_next;
			char* byte_next;
			
			int res = cc.out(state,
				str.c_str(), str.c_str() + str.length(), char_next,
				&buf[0], &buf[buf_size], byte_next);

			if (res == codecvt_base::error) {
				cerr << "codecvt convert fail. locale=" << loc.name() << endl;
				return string();
			}
			
			*byte_next = 0;
			string result(buf);
			delete buf;
			
			return result;
		}
		static wstring to_wstring(const string& str, const locale& loc)
		{
			typedef codecvt<wchar_t, char, mbstate_t> codecvt_t;
			const codecvt_t& cc = use_facet<codecvt_t>(loc);
			mbstate_t state = mbstate_t();
			
			int buf_size = cc.length(state, str.c_str(), str.c_str() + str.length(), str.length());
			wchar_t* buf = new wchar_t[buf_size+1];
			
			wchar_t* char_next;
			const char* byte_next;
			
			int res = cc.in(state,
				str.c_str(), str.c_str() + str.length(), byte_next,
				&buf[0], &buf[buf_size], char_next);

			if (res == codecvt_base::error) {
				cerr << "codecvt convert fail. locale=" << loc.name() << endl;
				return wstring();
			}

			*char_next = 0;
			wstring result(buf);
			delete buf;
			
			return result;
		}

		static string to_string(const wstring& str)
		{
			const wchar_t* lpwstr = str.c_str();
			size_t len = str.length();
			if(str[0] == (wchar_t)0xFEFF)
			{
				//	remove the leading endian decision chars 0xFE 0xFF
				++lpwstr;
				--len;
			}
			//locale loc(get_codepage_string(cp));
			//locale prev_loc = locale::global(loc);
			size_t size = wcstombs(0, lpwstr, len);
			if (size == 0){
				//locale::global(prev_loc);
				return string();
			}else{
				char* buf = new char[size+1];
				wcstombs(buf, lpwstr, size);
				buf[size] = 0;
				string ret(buf);
				delete buf;
				//locale::global(prev_loc);
				return ret;
			}
		}

		static void output_tick()
		{
			cout << "tick_load_locale = " << tick_load_locale << endl;
			cout << "tick_set_global = " << tick_set_global << endl;
			cout << "tick_pre_convert = " << tick_pre_convert << endl;
			cout << "tick_convert = " << tick_convert << endl;
			cout << "tick_new = " << tick_new << endl;
			cout << "tick_create_wstring = " << tick_create_wstring << endl;
			cout << "tick_delete = " << tick_delete << endl;
			cout << "tick_set_global_prev = " << tick_set_global_prev << endl;
		}

		static void clear_tick()
		{
			tick_load_locale = 0;
			tick_set_global = 0;
			tick_pre_convert = 0;
			tick_convert = 0;
			tick_new = 0;
			tick_create_wstring = 0;
			tick_delete = 0;
			tick_set_global_prev = 0;
		}
	};

	enum CharType{
		CT_BEGIN,
		CT_END,
		CT_SINGLE,
		CT_DELIMITER,
		CT_CHINESE,
		CT_LETTER,
		CT_NUMBER,
		CT_INDEX,
		CT_OTHER,
	};

	enum SPECIAL_STRING_INDEX{
		PRESERVED_ZERO,
		SENTENCE_BEGIN,
		SENTENCE_END,
		UNKNOWN_STRING,
		UNKNOWN_LOCATION,
		UNKNOWN_PERSON,
		UNKNOWN_TIME,
		UNKNOWN_NUMBER,
		UNKNOWN_ITEM,
		UNKNOWN_ORG,
		UNKNOWN_SPECIAL,
		SPECIAL_STRING_COUNT
	};

	const wchar_t* SPECIAL_STRING[] = {
		L"",
		L"ʼ##ʼ",
		L"ĩ##ĩ",
		L"δ##��",
		L"δ##��",
		L"δ##��",
		L"δ##ʱ",
		L"δ##��",
		L"δ##��",
		L"δ##��",
		L"δ##ר"
	};

	class Utility{
	public:
		static CharType CharType(wchar_t ch)
		{
			//	FIXME: we should centralize all wide string to avoid encoding
			//	FIXME: we should use unicode value boundary, rather than enum all possible character.
			//	0x22 should in CT_Delimiters
			const wstring CT_Delimiters(L"\x20!,.:;?()[]{}+=������������������-�����������������������������������������������¡áġšơǡȡɡʡˡ̡͡ΡϡСѡҡӡԡա֡סء١ڡۡܡݡޡߡ��������������������������������������������������������������������������������������ۣܣݣޣߣ�������� ");
			const wstring CT_Number(L"0123456789��������������������");
			const wstring CT_Letter(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz���£ãģţƣǣȣɣʣˣ̣ͣΣϣУѣңӣԣգ֣ףأ٣��������������������������������");
			const wstring CT_Index(L"�����������������������������������¢âĢŢƢǢȢɢʢˢ̢͢΢ϢТѢҢӢԢբ֢עآ٢ڢۢܢݢޢߢ���������������������������������");
			
			//	ASCII
			if( CT_Index.find(ch) != string::npos ){
				return CT_INDEX;
			}else if( CT_Number.find(ch) != string::npos ){
				return CT_NUMBER;
			}else if( CT_Letter.find(ch) != string::npos ){
				return CT_LETTER;
			}else if( CT_Delimiters.find(ch) != string::npos ){
				return CT_DELIMITER;
			}else if (ch < 0x80){
				return CT_SINGLE;
			}else {
				return CT_CHINESE;
			}
		}
		static SPECIAL_STRING_INDEX GetSpecialStringIndex(const wstring& str)
		{
			for(int i = 0; i < SPECIAL_STRING_COUNT; i++)
			{
				if(str == SPECIAL_STRING[i]){
					return (SPECIAL_STRING_INDEX) i;
				}
			}
			return (SPECIAL_STRING_INDEX) -1;
		}
	};

	enum Constans{
		MAX_FREQUENCE = 2079997
	};
}

namespace openclas {
	enum SymbolType{
		SYMBOL_TYPE_UNKNOWN,
		SYMBOL_TYPE_BEGIN,
		SYMBOL_TYPE_END,
		SYMBOL_TYPE_SINGLE,
		SYMBOL_TYPE_PUNCTUATION,
		SYMBOL_TYPE_CHINESE,
		SYMBOL_TYPE_LETTER,
		SYMBOL_TYPE_NUMBER,
		SYMBOL_TYPE_INDEX,
		SYMBOL_TYPE_OTHER,
	};

	enum WordType{
		WORD_TYPE_UNKNOWN,
		WORD_TYPE_SENTENCE_BEGIN,
		WORD_TYPE_SENTENCE_END,
		WORD_TYPE_UNKNOWN_STRING,
		WORD_TYPE_UNKNOWN_LOCATION,
		WORD_TYPE_UNKNOWN_PERSON,
		WORD_TYPE_UNKNOWN_TIME,
		WORD_TYPE_UNKNOWN_NUMBER,
		WORD_TYPE_UNKNOWN_ITEM,
		WORD_TYPE_UNKNOWN_ORG,
		WORD_TYPE_UNKNOWN_SPECIAL
	};

	const size_t WORD_TYPE_COUNT = static_cast<size_t>(WORD_TYPE_UNKNOWN_SPECIAL + 1);

	const char_type* WordTypeName[] = {
		L"",
		L"[���ӿ�ʼ]",		//L"ʼ##ʼ",
		L"[���ӽ���]",		//L"ĩ##ĩ",
		L"[δ��¼�ַ���]",	//L"δ##��",
		L"[δ��¼�ص�]",	//L"δ##��",
		L"[δ��¼����]",	//L"δ##��",
		L"[δ��¼ʱ��]",	//L"δ##ʱ",
		L"[δ��¼����]",	//L"δ##��",
		L"[δ��¼Item]",	//L"δ##��",
		L"[δ��¼��֯]",	//L"δ##��",
		L"[δ��¼ר�ô�]",	//L"δ##ר"
	};

	template<class T>
	class ValueCheck{
	public:
		ValueCheck(T value) : m_value(value) {}
		bool within(T lower_bound, T higher_bound)
		{
			return (m_value >= lower_bound && m_value <= higher_bound);
		}
		bool operator == (T value)
		{
			return m_value == value;
		}
	protected:
		T m_value;
	};

	template<class T>
	bool within(T value, T lower_bound, T higher_bound){
		return (value >= lower_bound && value <= higher_bound);
	}

	//	Detect the symbol type by unicode range
	//	reference: http://orwell.ru/test/Unicode/
	enum SymbolType get_symbol_type(char_type symbol) {
		ValueCheck<char_type> val(symbol);
		if (val.within(0x4E00, 0x9FFF)	//	CJK Unified Ideographs 
			|| val.within(0x3400, 0x4DBF)	//	CJK Unified Ideographs Extension A
			|| val.within(0xF900, 0xFAFF)	//	CJK Compatibility Ideographs
			|| val.within(0x2E80, 0x2EFF)	//	CJK Radicals Supplement
			|| val.within(0x2F00, 0x2FDF)	//	Kangxi Radicals
			)
		{
			return SYMBOL_TYPE_CHINESE;
		}

		if (val.within(0x2460, 0x24FF)	//	Enclosed Alphanumerics
			|| val.within(0x3200, 0x32FF)	//	Enclosed CJK Letters and Months 
			|| val.within(0x2160, 0x218F)	//	Number Forms (except 0x2150-215F)
			|| val.within(0x2776, 0x2793)	//	Dingbats
			)
		{
			return SYMBOL_TYPE_INDEX;
		}

		if (val.within(0x0030, 0x0039)	//	ASCII DIGITS
			|| val.within(0xFF10, 0xFF19)	//	Fullwidth DIGITS
			|| val.within(0x2150, 0x215F)	//	Number Forms (Fractions)
			|| val.within(0x2070, 0x2079)	//	Superscripts and Subscripts
			|| val.within(0x2080, 0x2089)
			)
		{
			return SYMBOL_TYPE_NUMBER;
		}

		if (val.within(0x0041, 0x005A)	//	ASCII Letters
			|| val.within(0x0061, 0x007A)
			|| val.within(0xFF21, 0xFF3A)	//	Fullwidth Letters
			|| val.within(0xFF41, 0xFF5A)
			|| val.within(0x00C0, 0x00D6)	//	Latin-1 Letters
			|| val.within(0x00D8, 0x00F6)
			|| val.within(0x00F8, 0x00FF)
			)
		{
			return SYMBOL_TYPE_LETTER;
		}

		if (val.within(0x0020, 0x002F)	//	ASCII Punctuations
			|| val.within(0x003A, 0x0040)
			|| val.within(0x005B, 0x0060)
			|| val.within(0x007B, 0x007E)
			|| val.within(0x0080, 0x00BF)	//	Latin-1 Punctuations
			|| val == 0x00D7
			|| val == 0x00F7
			|| val.within(0x02B0, 0x02FF)	//	Spacing Modifier Letters
			|| val.within(0x2000, 0x206F)	//	General Punctuation
			|| val.within(0x2100, 0x214F)	//	Letterlike Symbols
			|| val.within(0x2190, 0x23FF)	//	Arrows, Mathematical Operators, Miscellaneous Technical
			|| val.within(0x2500, 0x26FF)	//	Box Drawing, Block Elements, Geometric Shapes, Miscellaneous Symbols
			|| val.within(0x2700, 0x2775)	//	Dingbats
			|| val.within(0x2794, 0x27BF)
			|| val.within(0x27C0, 0x27FF)	//	Miscellaneous Mathematical Symbols-A, Supplemental Arrows-A
			|| val.within(0x2900, 0x2BFF)	//	Supplemental Arrows-B, Miscellaneous Mathematical Symbols-B, Supplemental Mathematical Operators, Miscellaneous Symbols and Arrows
			|| val.within(0x2E00, 0x2E7F)	//	Supplemental Punctuation
			|| val.within(0x3000, 0x303F)	//	CJK Symbols and Punctuation
			|| val.within(0xFF00, 0xFF0F)	//	Fullwidth Forms
			|| val.within(0xFF1A, 0xFF20)
			|| val.within(0xFF3B, 0xFF40)
			|| val.within(0xFF5B, 0xFF65)
			|| val.within(0xFFE0, 0xFFEF)
			|| val.within(0xFE00, 0xFE6F)	//	Vertical Forms
			)
		{
			return SYMBOL_TYPE_PUNCTUATION;
		}

		//	others
		return SYMBOL_TYPE_OTHER;
	}

	enum Constans{
		MAX_FREQUENCE = 2079997
	};

}