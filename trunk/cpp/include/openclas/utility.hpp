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
#ifndef _OPENCLAS_UTILITY_HPP_
#define _OPENCLAS_UTILITY_HPP_

#if defined(_MSC_VER)
#pragma warning( disable:4996 )
#endif

#include "common.hpp"

#include <string>
#include <locale>

#if defined(_MSC_VER)
//	Microsoft Visual C++ will need utf8_codecvt_facet.hpp for utf8 encoding.
#define BOOST_UTF8_BEGIN_NAMESPACE namespace openclas {
#define BOOST_UTF8_END_NAMESPACE }
#define BOOST_UTF8_DECL

//#include <boost/detail/utf8_codecvt_facet.hpp>
#include "utf8_codecvt_facet.hpp"

#undef BOOST_UTF8_BEGIN_NAMESPACE
#undef BOOST_UTF8_END_NAMESPACE
#undef BOOST_UTF8_DECL

#endif

namespace openclas {
	/** Word part-of-speech tag
	*
	*  Use PKU Tag definition with minor modification.
	*	Add WORD_TAG_NX for english words
	*	Add WORD_TAG_UNKNOWN, WORD_TAG_BEGIN, WORD_TAG_END for system.
	*
	*  See reference:
	*		北京大学现代汉语语料库基本加工规范, 俞士汶 段慧明 朱学锋 孙斌
	*		http://www.chineseldc.org/EN/doc/CLDC-LAC-2003-002/label.htm
	*/
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

	const size_t WORD_TAG_COUNT = static_cast<size_t>(WORD_TAG_Z + 1);	//	Get the count of WordTag

	/** Word part-of-speech tag name
	*
	*  Use PKU Tag definition with minor modification.
	*	Add L"nx"(WORD_TAG_NX) for english words
	*	Add L""(WORD_TAG_UNKNOWN), L"BEGIN"(WORD_TAG_BEGIN), L"END"(WORD_TAG_END) for system.
	*
	*  See reference:
	*		北京大学现代汉语语料库基本加工规范, 俞士汶 段慧明 朱学锋 孙斌
	*		http://www.chineseldc.org/EN/doc/CLDC-LAC-2003-002/label.htm
	*/
	static const wchar_t* WORD_TAG_NAME[] =
	{
		L"",
		L"ag",	//	形语素	形容词性语素。形容词代码为a，语素代码g前面置以A。
		L"a",	//	形容词	取英语形容词adjective的第1个字母。
		L"ad",	//	副形词	直接作状语的形容词。形容词代码a和副词代码d并在一起。
		L"an",	//	名形词	具有名词功能的形容词。形容词代码a和名词代码n并在一起。
		L"b",	//	区别词	取汉字“别”的声母。
		L"c",	//	连词		取英语连词conjunction的第1个字母。
		L"dg",	//	副语素	副词性语素。副词代码为d，语素代码g前面置以D。
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
		L"ng",	//	名语素	名词性语素。名词代码为n，语素代码ｇ前面置以N。
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
		L"tg",	//	时语素	时间词性语素。时间词代码为t,在语素的代码g前面置以T。
		L"t",	//	时间词	取英语time的第1个字母。
		L"u",	//	助词		取英语助词auxiliary 的第2个字母,因a已用于形容词。
		L"vg",	//	动语素	动词性语素。动词代码为v。在语素的代码g前面置以V。
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
	static const wchar_t* NUMBER_PREFIXS = L".-+．－＋";
	const int MAX_FREQUENCE = 2079997;

	/** Enumerate several useful character set in this library
	 *	see reference:
	 *		http://www.iana.org/assignments/character-sets
	 *		http://stdcxx.apache.org/doc/stdlibref/codecvt-byname.html#sec7
	 */
	enum Charset {
		CHARSET_ASCII,
		CHARSET_C,
		CHARSET_GB2312,
		CHARSET_GBK,
		CHARSET_GB18030,
		CHARSET_BIG5,
		CHARSET_UTF8
	};

	static const char* CharsetName[] = {
#if defined(_MSC_VER)
		/***************************************************
		 *	Reference:
		 *		http://msdn.microsoft.com/en-us/library/dd317756(VS.85).aspx
		 *
		 ****************************************************/
		"",			//	ASCII(1250)
		"C",		//	C(0)
		".20936",	//	GB2312
		".936",		//	GBK
		".54936",	//	GB18030
		".950",		//	BIG5
		".65001"	//	UTF8 (not supported by VC STL)
#else
		/***************************************************
		 *	Reference:
		 *		http://www.iana.org/assignments/character-sets
		 *		http://stdcxx.apache.org/doc/stdlibref/codecvt-byname.html
		 *		http://gcc.gnu.org/onlinedocs/libstdc++/manual/codecvt.html
		 ****************************************************/
		"",				//	ASCII
		"C",			//	C
		"zh_CN.GB2312",	//	GB2312
		"zh_CN.GBK",	//	GBK
		"zh_CN.GB18030",//	GB18030
		"zh_TW.BIG5",	//	BIG5
		"zh_CN.UTF8"	//	UTF8
#endif
	};

	typedef std::codecvt<wchar_t, char, mbstate_t> codecvt_t;

	/** Generate special word name
	*	Just add prefix "$" to WORD_TAG_NAME[WordTag]
	* @returns the concated string.
	*/
	inline std::wstring get_special_word_string(enum WordTag tag)
	{
		std::wostringstream oss;
		oss << L'$' << WORD_TAG_NAME[tag];
		return oss.str();
	}

	/**	Check the value whether is in the given range or not
	*
	*  Notice: the boundary is included in the range. That is the expression is as following:
	*		(lower_bound <= value) && (value <= higher_bound)
	*
	* @returns true for is in the given range, otherwise false.
	*/
	template<typename T, typename A, typename B>
	bool in_range(const T& value, const A& lower_bound, const B& higher_bound){
		return (static_cast<T>(lower_bound) <= value && value <= static_cast<T>(higher_bound));
	}

	/** Detect the symbol type by unicode range
	*	the range is defined by unicode range.
	*	see reference: http://orwell.ru/test/Unicode/
	* @returns the symbol type (enum SymbolType)
	*/
	static enum SymbolType get_symbol_type(wchar_t symbol) {
		//	the range might be able to merge to increase the performance a little,
		//	however, the seperated ranges make defination of the ranges more clear.
		if (in_range(symbol, 0x4E00, 0x9FFF)	//	CJK Unified Ideographs
		|| in_range(symbol, 0x3400, 0x4DBF)	//	CJK Unified Ideographs Extension A
		|| in_range(symbol, 0xF900, 0xFAFF)	//	CJK Compatibility Ideographs
		|| in_range(symbol, 0x2E80, 0x2EFF)	//	CJK Radicals Supplement
		|| in_range(symbol, 0x2F00, 0x2FDF)	//	Kangxi Radicals
		)
		{
			return SYMBOL_TYPE_CHINESE;
		}

		if (in_range(symbol, 0x2460, 0x24FF)	//	Enclosed Alphanumerics
			|| in_range(symbol, 0x3200, 0x32FF)	//	Enclosed CJK Letters and Months
			|| in_range(symbol, 0x2160, 0x218F)	//	Number Forms (except 0x2150-215F)
			|| in_range(symbol, 0x2776, 0x2793)	//	Dingbats
			)
		{
			return SYMBOL_TYPE_INDEX;
		}

		if (in_range(symbol, 0x0030, 0x0039)	//	ASCII DIGITS
			|| in_range(symbol, 0xFF10, 0xFF19)	//	Fullwidth DIGITS
			|| in_range(symbol, 0x2150, 0x215F)	//	Number Forms (Fractions)
			|| in_range(symbol, 0x2070, 0x2079)	//	Superscripts and Subscripts
			|| in_range(symbol, 0x2080, 0x2089)
			)
		{
			return SYMBOL_TYPE_NUMBER;
		}

		if (in_range(symbol, 0x0041, 0x005A)	//	ASCII Letters
			|| in_range(symbol, 0x0061, 0x007A)
			|| in_range(symbol, 0xFF21, 0xFF3A)	//	Fullwidth Letters
			|| in_range(symbol, 0xFF41, 0xFF5A)
			|| in_range(symbol, 0x00C0, 0x00D6)	//	Latin-1 Letters
			|| in_range(symbol, 0x00D8, 0x00F6)
			|| in_range(symbol, 0x00F8, 0x00FF)
			)
		{
			return SYMBOL_TYPE_LETTER;
		}

		if (in_range(symbol, 0x0020, 0x002F)	//	ASCII Punctuations
			|| in_range(symbol, 0x003A, 0x0040)
			|| in_range(symbol, 0x005B, 0x0060)
			|| in_range(symbol, 0x007B, 0x007E)
			|| in_range(symbol, 0x0080, 0x00BF)	//	Latin-1 Punctuations
			|| symbol == 0x00D7
			|| symbol == 0x00F7
			|| in_range(symbol, 0x02B0, 0x02FF)	//	Spacing Modifier Letters
			|| in_range(symbol, 0x2000, 0x206F)	//	General Punctuation
			|| in_range(symbol, 0x2100, 0x214F)	//	Letterlike Symbols
			|| in_range(symbol, 0x2190, 0x23FF)	//	Arrows, Mathematical Operators, Miscellaneous Technical
			|| in_range(symbol, 0x2500, 0x26FF)	//	Box Drawing, Block Elements, Geometric Shapes, Miscellaneous Symbols
			|| in_range(symbol, 0x2700, 0x2775)	//	Dingbats
			|| in_range(symbol, 0x2794, 0x27BF)
			|| in_range(symbol, 0x27C0, 0x27FF)	//	Miscellaneous Mathematical Symbols-A, Supplemental Arrows-A
			|| in_range(symbol, 0x2900, 0x2BFF)	//	Supplemental Arrows-B, Miscellaneous Mathematical Symbols-B, Supplemental Mathematical Operators, Miscellaneous Symbols and Arrows
			|| in_range(symbol, 0x2E00, 0x2E7F)	//	Supplemental Punctuation
			|| in_range(symbol, 0x3000, 0x303F)	//	CJK Symbols and Punctuation
			|| in_range(symbol, 0xFF00, 0xFF0F)	//	Fullwidth Forms
			|| in_range(symbol, 0xFF1A, 0xFF20)
			|| in_range(symbol, 0xFF3B, 0xFF40)
			|| in_range(symbol, 0xFF5B, 0xFF65)
			|| in_range(symbol, 0xFFE0, 0xFFEF)
			|| in_range(symbol, 0xFE00, 0xFE6F)	//	Vertical Forms
			)
		{
			return SYMBOL_TYPE_PUNCTUATION;
		}

		//	others
		return SYMBOL_TYPE_OTHER;
	}

	/** Check whether the given symbol exist in a given collection
	*	The given input has to be either char or wchar_t
	*	the given collection should be an \0 terminated string.
	* @returns true for is exist in the collection.
	*/
	template <typename CharType>
	inline bool exist(CharType symbol, const CharType* collection)
	{
		while(*collection)
			if (*(collection++) == symbol)
				return true;
		return false;
	}

	const std::locale make_locale(enum Charset charset)
	{
#if defined(_MSC_VER)
		if (charset != CHARSET_UTF8){
			return std::locale(CharsetName[charset]);
		}else{
			return std::locale(std::locale::classic(), new utf8_codecvt_facet);
		}
#else
		return std::locale::locale(CharsetName[charset]);
#endif
	}

	/** Convert given wide string to narrow string by given locale object.
	*  The function is used for converting from wide char string to multibyte string.
	*	Notice: Visual C++ current cannot support utf-8 locale in STL. So, for alternative
	*          way, user can use system-specified way to do the conversion.
	*	see also: openclas::widen();
	* @returns the converted narrow multibyte string.
	*/
	static std::string narrow(const std::wstring& str, const std::locale& loc)
	{
		const codecvt_t& cc = std::use_facet<codecvt_t>(loc);

		int buf_size = static_cast<int>(cc.max_length() * (str.length() + 1));
		scoped_array<char> buf(new char[buf_size]);

		mbstate_t state = mbstate_t();
		const wchar_t* from_next = 0;
		char* to_next = 0;

		std::codecvt_base::result result = cc.out(state,
			str.c_str(), str.c_str() + str.length(), from_next,
			buf.get(), buf.get() + buf_size, to_next);

		if (result == std::codecvt_base::ok)
		{
			std::string narrow_string(buf.get(), to_next);
			return narrow_string;
		}else{
			return std::string(buf.get(), to_next);
		}
	}

	/** Convert given narrow string to wide string by given locale object.
	*  The function is used for converting from multi-byte string to wide char string.
	*	Notice: Visual C++ current cannot support utf-8 locale in STL. So, for alternative
	*          way, user can use system-specified way to do the conversion.
	*	see also: openclas::widen();
	* @returns the converted wide string.
	*/
	static std::wstring widen(const std::string& str, const std::locale& loc)
	{
		const codecvt_t& cc = std::use_facet<codecvt_t>(loc);

		size_t buf_size = str.length() + 1;
		scoped_array<wchar_t> buf(new wchar_t[buf_size]);

		mbstate_t state = mbstate_t();
		const char* from_next = 0;
		wchar_t* to_next = 0;

		std::codecvt_base::result result = cc.in(state,
			str.c_str(), str.c_str() + str.length(), from_next,
			buf.get(), buf.get() + buf_size, to_next);

		if (result == std::codecvt_base::ok)
		{
			std::wstring widen_string(buf.get(), to_next);
			return widen_string;
		}else{
			return std::wstring();
		}
	}

}	//	namespace openclas

//	 _OPENCLAS_UTILITY_HPP_
#endif
