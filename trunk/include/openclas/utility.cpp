/*********************************************************************************
Copyright 2009 Dancefire (dancefire@gmail.com).
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


版权所有 2009 Dancefire (dancefire@gmail.com)。
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
#include "utility.h"

namespace openclas {
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

	bool is_inside(char_type symbol, const string_type& collection)
	{
		for(string_type::const_iterator it = collection.begin(); it != collection.end(); ++it)
		{
			if (*it == symbol) { return true; }
		}
		return false;
	}

	std::string narrow(const std::wstring& str, const std::locale& loc)
	{
		const code_converter_type& cc = std::use_facet<code_converter_type>(loc);

		int buf_size = static_cast<int>(cc.max_length() * (str.length() + 1));
		char* buf = new char[buf_size];

		mbstate_t state = mbstate_t();
		const wchar_t* from_next = 0;
		char* to_next = 0;

		std::codecvt_base::result result = cc.out(state,
			str.c_str(), str.c_str() + str.length(), from_next,
			buf, buf + buf_size, to_next);

		if (result == std::codecvt_base::ok)
		{
			std::string narrow_string(buf, to_next);
			delete buf;
			return narrow_string;
		}else{
			delete buf;
			return std::string();
		}
	}

	std::wstring widen(const std::string& str, const std::locale& loc)
	{
		const code_converter_type& cc = std::use_facet<code_converter_type>(loc);

		size_t buf_size = str.length() + 1;
		wchar_t* buf = new wchar_t[buf_size];

		mbstate_t state = mbstate_t();
		const char* from_next = 0;
		wchar_t* to_next = 0;

		std::codecvt_base::result result = cc.in(state,
			str.c_str(), str.c_str() + str.length(), from_next,
			buf, buf + buf_size, to_next);

		if (result == std::codecvt_base::ok)
		{
			std::wstring widen_string(buf, to_next);
			delete buf;
			return widen_string;
		}else{
			delete buf;
			return std::wstring();
		}
	}

	namespace pku {
		string_type get_special_word_string(enum WordTag tag)
		{
			string_type str;
			str.push_back(L'$');
			str.append(WORD_TAG_NAME[tag]);
			return str;
		}
	}
}