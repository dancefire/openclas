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
*	$Date$
*	$Revision$
*	$Author$
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

	static const char_type* WordTypeName[] = {
		L"",
		L"[句子开始]",		//L"始##始",
		L"[句子结束]",		//L"末##末",
		L"[未登录字符串]",	//L"未##串",
		L"[未登录地点]",	//L"未##地",
		L"[未登录人名]",	//L"未##人",
		L"[未登录时间]",	//L"未##时",
		L"[未登录数字]",	//L"未##数",
		L"[未登录Item]",	//L"未##它",
		L"[未登录组织]",	//L"未##团",
		L"[未登录专用词]",	//L"未##专"
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

	enum Constans{
		MAX_FREQUENCE = 2079997
	};

}