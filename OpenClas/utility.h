/*********************************************************************************
Copyright 2007-2009 Dancefire (dancefire@gmail.com).
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


版权所有 2007-2009 Dancefire (dancefire@gmail.com)。
保留所有权利。

在满足下列条件的前提下，允许重新分发修改过或未经修改的，以源代码或已编译形式
存在的本软件：

1. 以源代码形式重新发布必须保留未经修改的上述版权声明、本许可条件以及其后的免
责声明。
2. 以已编译形式重新发布必须在发布版本的文档和/或其它同时发布的资料中重述上述
版权声明、本许可条件以及其后的免责声明。

此软件由作者及贡献者 “按现状形式”(AS-IS) 提供。在此明示不提供任何包括但不限于
关于特定目的的适销性、实用性的明示或暗示的担保。在任何情况下，作者及贡献者或其
他参与者皆不对由于使用此软件造成的直接、间接、连带、特别、惩戒的或因而造成的的
损害(包括但不限定于获得替代物及服务、无法使用、数据丢失、损失盈利或商务中断)承
担义务，无论任何在软件使用时产生损害是如何造成的、基于何种责任推断、是否属于合
同范畴、严格赔偿责任或民事侵权行为(包括疏忽和其它原因)，即使预先被告知此类损害
发生的可能性。

**********************************************************************************/
/*
*	$Id$
*/

#pragma once

#pragma warning( disable:4996 )

#include "common.h"

#include <string>
#include <locale>
#include <ctime>
#include <iostream>


namespace ictclas{

	using namespace std;

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
		enum code_page {
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

	static const wchar_t* SPECIAL_STRING[] = {
		L"",
		L"始##始",
		L"末##末",
		L"未##串",
		L"未##地",
		L"未##人",
		L"未##时",
		L"未##数",
		L"未##它",
		L"未##团",
		L"未##专"
	};

	class Utility{
	public:
		static CharType CharType(wchar_t ch)
		{
			//	FIXME: we should centralize all wide string to avoid encoding
			//	FIXME: we should use unicode value boundary, rather than enum all possible character.
			//	0x22 should in CT_Delimiters
			const wstring CT_Delimiters(L"\x20!,.:;?()[]{}+=　、。·ˉˇ¨〃々-～‖…‘’“”〔〕〈〉《》「」『』〖〗【】±×÷∶∧∨∑∏∪∩∈∷√⊥∥∠⌒⊙∫∮≡≌≈∽∝≠≮≯≤≥∞∵∴♂♀°′″℃＄¤￠￡‰§№☆★○●◎◇◆□■△▲※→←↑↓〓！＂＃￥％＆＇（）＊＋，－．／：；＜＝＞？＠［＼］＾＿｀｛｜｝￣ ");
			const wstring CT_Number(L"0123456789０１２３４５６７８９");
			const wstring CT_Letter(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ");
			const wstring CT_Index(L"⒈⒉⒊⒋⒌⒍⒎⒏⒐⒑⒒⒓⒔⒕⒖⒗⒘⒙⒚⒛⑴⑵⑶⑷⑸⑹⑺⑻⑼⑽⑾⑿⒀⒁⒂⒃⒄⒅⒆⒇①②③④⑤⑥⑦⑧⑨⑩㈠㈡㈢㈣㈤㈥㈦㈧㈨㈩ⅠⅡⅢⅣⅤⅥⅦⅧⅨⅩⅪⅫ");

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

	enum SymbolType get_symbol_type(char_type symbol);
	bool is_inside(char_type symbol, const string_type& collection);

	const string_type NUMBER_PREFIXS(L".-+．－＋");

	const int MAX_FREQUENCE = 2079997;

	enum CodePage {
		CODEPAGE_C,
		CODEPAGE_ANSI,
		CODEPAGE_GB2312,
		CODEPAGE_GBK,
		CODEPAGE_GB18030,
		CODEPAGE_BIG5,
		CODEPAGE_UCS2_LE,
		CODEPAGE_UCS2_BE,
		CODEPAGE_UCS4_LE,
		CODEPAGE_UCS4_BE,
		CODEPAGE_UTF7,
		CODEPAGE_UTF8
	};

	static const char* CodePageString[] = {
#ifdef WIN32
		"C",		//	C(0)
		"",			//	ANSI(1250)
		".20936",	//	GB2312
		".936",		//	GBK
		".54936",	//	GB18030
		".950",		//	BIG5
		".1200",	//	UCS2_LE
		".1201",	//	UCS2_BE
		".12000",	//	UCS4_LE
		".12001",	//	UCS4_BE
		".65000",	//	UTF7
		".65001"	//	UTF8
#else
		/***************************************************
		*	Reference:
		*		http://www.iana.org/assignments/character-sets
		*		http://stdcxx.apache.org/doc/stdlibref/codecvt-byname.html
		*		http://gcc.gnu.org/onlinedocs/libstdc++/manual/codecvt.html
		****************************************************/
		"C",		//	C
		"",			//	ANSI
		"GB2312",	//	GB2312
		"GBK",		//	GBK
		"GB18030",	//	GB18030
		"BIG5",		//	BIG5
		"CP1200",	//	UCS2_LE
		"CP1201",	//	UCS2_BE
		"CP12000",	//	UCS4_LE
		"CP12001",	//	UCS4_BE
		"UTF7",		//	UTF7
		"UTF8"		//	UTF8
#endif
	};

	//std::locale get_locale(enum CodePage codepage)
	//{
	//	return std::locale(CodePageString[codepage]);
	//}

	//std::locale set_locale(enum CodePage codepage)
	//{
	//	return std::locale::global(std::locale(CodePageString[codepage]));
	//}

	//wchar_t* convert_to_wide_char_string(const char* string_ptr, size_t length)
	//{
	//	if (string_ptr != 0 && length != 0)
	//	{
	//		size_t buf_size = mbstowcs(0, string_ptr, length);
	//		if (buf_size > 0)
	//		{
	//			wchar_t* buf = new wchar_t[buf_size+1];
	//			mbstowcs(buf, string_ptr, buf_size);
	//			buf[buf_size] = 0;
	//			return buf;
	//		}
	//	}

	//	return 0;
	//}

	//char* convert_to_char_string(const wchar_t* string_ptr, size_t length)
	//{
	//	size_t buf_size = wcstombs(0, string_ptr, length);
	//	if (buf_size > 0)
	//	{
	//		char* buf = new char[size+1];
	//		wcstombs(buf, string_ptr, buf_size);
	//		buf[buf_size] = 0;
	//		return buf;
	//	}

	//	return 0;
	//}

	typedef std::codecvt<wchar_t, char, mbstate_t> code_converter_type;

	std::string narrow(const std::wstring& str, const std::locale& loc = std::locale());
	std::wstring widen(const std::string& str, const std::locale& loc = std::locale());

	namespace pku {
		//	Reference:
		//		北京大学现代汉语语料库基本加工规范
		//			俞士汶 段慧明 朱学锋 孙斌
		//			http://www.chineseldc.org/EN/doc/CLDC-LAC-2003-002/label.htm
		enum WordTag{
			WORD_TAG_UNKNOWN,
			WORD_TAG_AG,	//	形语素	形容词性语素。形容词代码为a，语素代码g前面置以a。
			WORD_TAG_A,	//	形容词	取英语形容词adjective的第1个字母。
			WORD_TAG_AD,	//	副形词	直接作状语的形容词。形容词代码a和副词代码d并在一起。
			WORD_TAG_AN,	//	名形词	具有名词功能的形容词。形容词代码a和名词代码N并在一起。
			WORD_TAG_B,	//	区别词	取汉字“别”的声母。
			WORD_TAG_C,	//	连词		取英语连词conjunction的第1个字母。
			WORD_TAG_DG,	//	副语素	副词性语素。副词代码为d，语素代码g前面置以d。
			WORD_TAG_D,	//	副词		取adverb的第2个字母，因其第1个字母已用于形容词。
			WORD_TAG_E,	//	叹词		取英语叹词exclamation的第1个字母。
			WORD_TAG_F,	//	方位词	取汉字“方” 的声母。
			WORD_TAG_G,	//	语素		绝大多数语素都能作为合成词的“词根”，取汉字“根”的声母。
			WORD_TAG_H,	//	前接成分	取英语head的第1个字母。
			WORD_TAG_I,	//	成语		取英语成语idiom的第1个字母。
			WORD_TAG_J,	//	简称略语	取汉字“简”的声母。
			WORD_TAG_K,	//	后接成分
			WORD_TAG_L,	//	习用语	习用语尚未成为成语，有点“临时性”，取“临”的声母。
			WORD_TAG_M,	//	数词		取英语numeral的第3个字母，n，u已有他用。
			WORD_TAG_NG,	//	名语素	名词性语素。名词代码为n，语素代码g前面置以n。
			WORD_TAG_N,	//	名词		取英语名词noun的第1个字母。
			WORD_TAG_NR,	//	人名		名词代码n和“人(ren)”的声母并在一起。
			WORD_TAG_NS,	//	地名		名词代码n和处所词代码s并在一起。
			WORD_TAG_NT,	//	机构团体	“团”的声母为t，名词代码n和t并在一起。
			WORD_TAG_NX,	//	非汉字串
			WORD_TAG_NZ,	//	其他专名	“专”的声母的第1个字母为z，名词代码n和z并在一起。 
			WORD_TAG_O,	//	 拟声词	取英语拟声词onomatopoeia的第1个字母。
			WORD_TAG_P,	//	介词		取英语介词prepositional的第1个字母。
			WORD_TAG_Q,	//	量词		取英语quantity的第1个字母。
			WORD_TAG_R,	//	代词		取英语代词pronoun的第2个字母,因p已用于介词。
			WORD_TAG_S,	//	处所词	取英语space的第1个字母。
			WORD_TAG_TG,	//	时语素	时间词性语素。时间词代码为t,在语素的代码g前面置以t。
			WORD_TAG_T,	//	时间词	取英语time的第1个字母。
			WORD_TAG_U,	//	助词		取英语助词auxiliary 的第2个字母,因a已用于形容词。
			WORD_TAG_VG,	//	动语素	动词性语素。动词代码为v。在语素的代码g前面置以v。
			WORD_TAG_V,	//	动词		取英语动词verb的第一个字母。
			WORD_TAG_VD,	//	副动词	直接作状语的动词。动词和副词的代码并在一起。
			WORD_TAG_VN,	//	名动词	指具有名词功能的动词。动词和名词的代码并在一起。
			WORD_TAG_W,	//	标点符号   
			WORD_TAG_X,	//	非语素字	非语素字只是一个符号，字母x通常用于代表未知数、符号。
			WORD_TAG_Y,	//	语气词	取汉字“语”的声母。
			WORD_TAG_Z,	//	状态词	取汉字“状”的声母的前一个字母。

			WORD_TAG_BEGIN,	//	句子开始(特殊标记)
			WORD_TAG_END	//	句子结束(特殊标记)
		};

		const size_t WORD_TAG_COUNT = static_cast<size_t>(WORD_TAG_Z + 1);	//	Get the count of TagTypePKU

		static const char_type* WORD_TAG_NAME[] = 
		{
			L"",
			L"Ag",	//	形语素	形容词性语素。形容词代码为a，语素代码g前面置以A。
			L"a",	//	形容词	取英语形容词adjective的第1个字母。
			L"ad",	//	副形词	直接作状语的形容词。形容词代码a和副词代码d并在一起。
			L"an",	//	名形词	具有名词功能的形容词。形容词代码a和名词代码n并在一起。
			L"b",	//	区别词	取汉字“别”的声母。
			L"c",	//	连词		取英语连词conjunction的第1个字母。
			L"Dg",	//	副语素	副词性语素。副词代码为d，语素代码ｇ前面置以D。
			L"d",	//	副词		取adverb的第2个字母，因其第1个字母已用于形容词。
			L"e",	//	叹词		取英语叹词exclamation的第1个字母。
			L"f",	//	方位词	取汉字“方” 的声母。
			L"g",	//	语素		绝大多数语素都能作为合成词的“词根”，取汉字“根”的声母。
			L"h",	//	前接成分	取英语head的第1个字母。
			L"i",	//	成语		取英语成语idiom的第1个字母。
			L"j",	//	简称略语	取汉字“简”的声母。
			L"k",	//	后接成分
			L"l",	//	习用语	习用语尚未成为成语，有点“临时性”，取“临”的声母。
			L"m",	//	数词		取英语numeral的第3个字母，n，u已有他用。
			L"Ng",	//	名语素	名词性语素。名词代码为n，语素代码ｇ前面置以N。
			L"n",	//	名词		取英语名词noun的第1个字母。
			L"nr",	//	人名		名词代码n和“人(ren)”的声母并在一起。
			L"ns",	//	地名		名词代码n和处所词代码s并在一起。
			L"nt",	//	机构团体	“团”的声母为t，名词代码n和t并在一起。
			L"nx",	//	非汉字串
			L"nz",	//	其他专名	“专”的声母的第1个字母为z，名词代码n和z并在一起。 
			L"o",	//	 拟声词	取英语拟声词onomatopoeia的第1个字母。
			L"p",	//	介词		取英语介词prepositional的第1个字母。
			L"q",	//	量词		取英语quantity的第1个字母。
			L"r",	//	代词		取英语代词pronoun的第2个字母,因p已用于介词。
			L"s",	//	处所词	取英语space的第1个字母。
			L"Tg",	//	时语素	时间词性语素。时间词代码为t,在语素的代码g前面置以T。
			L"t",	//	时间词	取英语time的第1个字母。
			L"u",	//	助词		取英语助词auxiliary 的第2个字母,因a已用于形容词。
			L"Vg",	//	动语素	动词性语素。动词代码为v。在语素的代码g前面置以V。
			L"v",	//	动词		取英语动词verb的第一个字母。
			L"vd",	//	副动词	直接作状语的动词。动词和副词的代码并在一起。
			L"vn",	//	名动词	指具有名词功能的动词。动词和名词的代码并在一起。
			L"w",	//	标点符号   
			L"x",	//	非语素字	非语素字只是一个符号，字母x通常用于代表未知数、符号。
			L"y",	//	语气词	取汉字“语”的声母。
			L"z",	//	状态词	取汉字“状”的声母的前一个字母。

			L"BEGIN",	//	句子开始
			L"END",		//	句子结束
		};

		string_type get_special_word_string(enum WordTag tag);
	}	//	namespace pku
}