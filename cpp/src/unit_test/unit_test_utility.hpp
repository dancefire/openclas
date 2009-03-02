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
#ifndef _OPENCLAS_UNIT_TEST_UTILITY_HPP_
#define _OPENCLAS_UNIT_TEST_UTILITY_HPP_

#ifdef WIN32
#pragma warning( disable:4566 )
#endif

#include <openclas/utility.hpp>


BOOST_AUTO_TEST_SUITE( utility )

using namespace openclas;

BOOST_AUTO_TEST_CASE( test_get_special_word_string )
{
	BOOST_CHECK( get_special_word_string(WORD_TAG_NX) == std::wstring(L"$nx") );
	BOOST_CHECK( get_special_word_string(WORD_TAG_AG) == std::wstring(L"$ag") );
	BOOST_CHECK( get_special_word_string(WORD_TAG_NR) == std::wstring(L"$nr") );
	BOOST_CHECK( get_special_word_string(WORD_TAG_NZ) == std::wstring(L"$nz") );
	BOOST_CHECK( get_special_word_string(WORD_TAG_NS) == std::wstring(L"$ns") );
	BOOST_CHECK( get_special_word_string(WORD_TAG_UNKNOWN) == std::wstring(L"$") );
	BOOST_CHECK( get_special_word_string(WORD_TAG_BEGIN) == std::wstring(L"$BEGIN") );
	BOOST_CHECK( get_special_word_string(WORD_TAG_END) == std::wstring(L"$END") );
}

BOOST_AUTO_TEST_CASE( test_in_range )
{
	//	integer
	BOOST_CHECK_EQUAL(in_range(5, 1, 10), true);
	BOOST_CHECK_EQUAL(in_range(5, 5, 10), true);
	BOOST_CHECK_EQUAL(in_range(5, 1, 5), true);
	BOOST_CHECK_EQUAL(in_range(5, 1, 4), false);
	BOOST_CHECK_EQUAL(in_range(5, 6, 10), false);
	//	double
	BOOST_CHECK_EQUAL(in_range(5., 1, 10), true);
	BOOST_CHECK_EQUAL(in_range(5., 5, 10), true);
	BOOST_CHECK_EQUAL(in_range(5., 1, 5), true);
	BOOST_CHECK_EQUAL(in_range(5., 1, 4), false);
	BOOST_CHECK_EQUAL(in_range(5., 6, 10), false);
	//	char
	BOOST_CHECK_EQUAL(in_range('f', 'a', 'n'), true);
	BOOST_CHECK_EQUAL(in_range('f', 'f', 'n'), true);
	BOOST_CHECK_EQUAL(in_range('f', 'a', 'f'), true);
	BOOST_CHECK_EQUAL(in_range('f', 'a', 'e'), false);
	BOOST_CHECK_EQUAL(in_range('f', 'g', 'n'), false);
	//	wchar_t
	BOOST_CHECK_EQUAL(in_range(L'f', L'a', L'n'), true);
	BOOST_CHECK_EQUAL(in_range(L'f', L'f', L'n'), true);
	BOOST_CHECK_EQUAL(in_range(L'f', L'a', L'f'), true);
	BOOST_CHECK_EQUAL(in_range(L'f', L'a', L'e'), false);
	BOOST_CHECK_EQUAL(in_range(L'f', L'g', L'n'), false);
}

BOOST_AUTO_TEST_CASE( test_get_symbol_type )
{
	//	SYMBOL_TYPE_CHINESE
	//		CJK Unified Ideographs	[0x4E00, 0x9FFF]
	BOOST_CHECK_EQUAL(get_symbol_type(L'一'), SYMBOL_TYPE_CHINESE);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'鿿'), SYMBOL_TYPE_CHINESE);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'鱀'), SYMBOL_TYPE_CHINESE);
	BOOST_CHECK_EQUAL(get_symbol_type(L'丠'), SYMBOL_TYPE_CHINESE);
	//		CJK Unified Ideographs Extension A	[0x3400, 0x4DBF]
	BOOST_CHECK_EQUAL(get_symbol_type(L'㐀'), SYMBOL_TYPE_CHINESE);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'䶿'), SYMBOL_TYPE_CHINESE);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'㪘'), SYMBOL_TYPE_CHINESE);
	BOOST_CHECK_EQUAL(get_symbol_type(L'䉨'), SYMBOL_TYPE_CHINESE);
	//		CJK Compatibility Ideographs	[0xF900, 0xFAFF]
	BOOST_CHECK_EQUAL(get_symbol_type(L'豈'), SYMBOL_TYPE_CHINESE);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'﫿'), SYMBOL_TYPE_CHINESE);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'樂'), SYMBOL_TYPE_CHINESE);
	BOOST_CHECK_EQUAL(get_symbol_type(L'禍'), SYMBOL_TYPE_CHINESE);
	//		CJK Radicals Supplement	[0x2E80, 0x2EFF]
	BOOST_CHECK_EQUAL(get_symbol_type(L'⺀'), SYMBOL_TYPE_CHINESE);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'⻿'), SYMBOL_TYPE_CHINESE);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'⺮'), SYMBOL_TYPE_CHINESE);
	BOOST_CHECK_EQUAL(get_symbol_type(L'⺓'), SYMBOL_TYPE_CHINESE);
	//		Kangxi Radicals	[0x2F00, 0x2FDF]
	BOOST_CHECK_EQUAL(get_symbol_type(L'⼀'), SYMBOL_TYPE_CHINESE);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'⿟'), SYMBOL_TYPE_CHINESE);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'⿓'), SYMBOL_TYPE_CHINESE);
	BOOST_CHECK_EQUAL(get_symbol_type(L'⽃'), SYMBOL_TYPE_CHINESE);


	//	SYMBOL_TYPE_INDEX
	//		Enclosed Alphanumerics	0x2460, 0x24FF
	BOOST_CHECK_EQUAL(get_symbol_type(L'①'), SYMBOL_TYPE_INDEX);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'⓿'), SYMBOL_TYPE_INDEX);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'⑿'), SYMBOL_TYPE_INDEX);
	BOOST_CHECK_EQUAL(get_symbol_type(L'ⓤ'), SYMBOL_TYPE_INDEX);
	//		Enclosed CJK Letters and Months		0x3200, 0x32FF
	BOOST_CHECK_EQUAL(get_symbol_type(L'㈀'), SYMBOL_TYPE_INDEX);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'㋿'), SYMBOL_TYPE_INDEX);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'㈲'), SYMBOL_TYPE_INDEX);
	BOOST_CHECK_EQUAL(get_symbol_type(L'㊰'), SYMBOL_TYPE_INDEX);
	//		Number Forms (except 0x2150-215F)	0x2160, 0x218F
	BOOST_CHECK_EQUAL(get_symbol_type(L'Ⅰ'), SYMBOL_TYPE_INDEX);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'↏'), SYMBOL_TYPE_INDEX);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'Ⅻ'), SYMBOL_TYPE_INDEX);
	BOOST_CHECK_EQUAL(get_symbol_type(L'ↀ'), SYMBOL_TYPE_INDEX);
	//		Dingbats		0x2776, 0x2793
	BOOST_CHECK_EQUAL(get_symbol_type(L'❶'), SYMBOL_TYPE_INDEX);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'➓'), SYMBOL_TYPE_INDEX);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'❼'), SYMBOL_TYPE_INDEX);
	BOOST_CHECK_EQUAL(get_symbol_type(L'➆'), SYMBOL_TYPE_INDEX);


	//	SYMBOL_TYPE_NUMBER
	//	ASCII DIGITS		0x0030, 0x0039
	BOOST_CHECK_EQUAL(get_symbol_type(L'0'), SYMBOL_TYPE_NUMBER);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'9'), SYMBOL_TYPE_NUMBER);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'6'), SYMBOL_TYPE_NUMBER);
	BOOST_CHECK_EQUAL(get_symbol_type(L'7'), SYMBOL_TYPE_NUMBER);
	//	Fullwidth DIGITS	0xFF10, 0xFF19
	BOOST_CHECK_EQUAL(get_symbol_type(L'０'), SYMBOL_TYPE_NUMBER);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'９'), SYMBOL_TYPE_NUMBER);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'３'), SYMBOL_TYPE_NUMBER);
	BOOST_CHECK_EQUAL(get_symbol_type(L'６'), SYMBOL_TYPE_NUMBER);
	//	Number Forms (Fractions)		0x2150, 0x215F
	BOOST_CHECK_EQUAL(get_symbol_type(L'⅐'), SYMBOL_TYPE_NUMBER);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'⅟'), SYMBOL_TYPE_NUMBER);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'⅓'), SYMBOL_TYPE_NUMBER);
	BOOST_CHECK_EQUAL(get_symbol_type(L'⅛'), SYMBOL_TYPE_NUMBER);
	//	Superscripts and Subscripts	0x2070, 0x2079
	BOOST_CHECK_EQUAL(get_symbol_type(L'⁰'), SYMBOL_TYPE_NUMBER);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'⁹'), SYMBOL_TYPE_NUMBER);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'⁴'), SYMBOL_TYPE_NUMBER);
	BOOST_CHECK_EQUAL(get_symbol_type(L'⁶'), SYMBOL_TYPE_NUMBER);
	//      Superscripts and Subscripts	0x2080, 0x2089
	BOOST_CHECK_EQUAL(get_symbol_type(L'₀'), SYMBOL_TYPE_NUMBER);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'₉'), SYMBOL_TYPE_NUMBER);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'₃'), SYMBOL_TYPE_NUMBER);
	BOOST_CHECK_EQUAL(get_symbol_type(L'₆'), SYMBOL_TYPE_NUMBER);
	
	//	SYMBOL_TYPE_LETTER
	//	ASCII Letters		0x0041, 0x005A
	BOOST_CHECK_EQUAL(get_symbol_type(L'A'), SYMBOL_TYPE_LETTER);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'Z'), SYMBOL_TYPE_LETTER);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'G'), SYMBOL_TYPE_LETTER);
	BOOST_CHECK_EQUAL(get_symbol_type(L'O'), SYMBOL_TYPE_LETTER);
	//	ASCII Letters		0x0061, 0x007A
	BOOST_CHECK_EQUAL(get_symbol_type(L'a'), SYMBOL_TYPE_LETTER);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'z'), SYMBOL_TYPE_LETTER);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'e'), SYMBOL_TYPE_LETTER);
	BOOST_CHECK_EQUAL(get_symbol_type(L't'), SYMBOL_TYPE_LETTER);
	//	Fullwidth Letters	0xFF21, 0xFF3A
	BOOST_CHECK_EQUAL(get_symbol_type(L'Ａ'), SYMBOL_TYPE_LETTER);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'Ｚ'), SYMBOL_TYPE_LETTER);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'Ｈ'), SYMBOL_TYPE_LETTER);
	BOOST_CHECK_EQUAL(get_symbol_type(L'Ｘ'), SYMBOL_TYPE_LETTER);
	//	Fullwidth Letters	0xFF41, 0xFF5A
	BOOST_CHECK_EQUAL(get_symbol_type(L'ａ'), SYMBOL_TYPE_LETTER);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'ｚ'), SYMBOL_TYPE_LETTER);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'ｇ'), SYMBOL_TYPE_LETTER);
	BOOST_CHECK_EQUAL(get_symbol_type(L'ｓ'), SYMBOL_TYPE_LETTER);
	//	Latin-1 Letters		 0x00C0, 0x00D6
	BOOST_CHECK_EQUAL(get_symbol_type(L'À'), SYMBOL_TYPE_LETTER);	//	begin	
	BOOST_CHECK_EQUAL(get_symbol_type(L'Ö'), SYMBOL_TYPE_LETTER);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'É'), SYMBOL_TYPE_LETTER);
	BOOST_CHECK_EQUAL(get_symbol_type(L'Ñ'), SYMBOL_TYPE_LETTER);
	//	Latin-1 Letters		0x00D8, 0x00F6
	BOOST_CHECK_EQUAL(get_symbol_type(L'Ø'), SYMBOL_TYPE_LETTER);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'ö'), SYMBOL_TYPE_LETTER);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'à'), SYMBOL_TYPE_LETTER);
	BOOST_CHECK_EQUAL(get_symbol_type(L'ð'), SYMBOL_TYPE_LETTER);
	//	Latin-1 Letters		0x00F8, 0x00FF
	BOOST_CHECK_EQUAL(get_symbol_type(L'ø'), SYMBOL_TYPE_LETTER);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'ÿ'), SYMBOL_TYPE_LETTER);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'ú'), SYMBOL_TYPE_LETTER);
	BOOST_CHECK_EQUAL(get_symbol_type(L'ý'), SYMBOL_TYPE_LETTER);
	
	
	//	SYMBOL_TYPE_PUNCTUATION
	//	ASCII Punctuationsin_range		0x0020, 0x002F
	BOOST_CHECK_EQUAL(get_symbol_type(L' '), SYMBOL_TYPE_PUNCTUATION);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'/'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'"'), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'\''), SYMBOL_TYPE_PUNCTUATION);
	//	ASCII Punctuations				0x003A, 0x0040
	BOOST_CHECK_EQUAL(get_symbol_type(L':'), SYMBOL_TYPE_PUNCTUATION);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'@'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'='), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'>'), SYMBOL_TYPE_PUNCTUATION);
	//	ASCII Punctuations				0x005B, 0x0060
	BOOST_CHECK_EQUAL(get_symbol_type(L'['), SYMBOL_TYPE_PUNCTUATION);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'`'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'^'), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'_'), SYMBOL_TYPE_PUNCTUATION);
	//	ASCII Punctuations				0x007B, 0x007E
	BOOST_CHECK_EQUAL(get_symbol_type(L'{'), SYMBOL_TYPE_PUNCTUATION);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'~'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'|'), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'}'), SYMBOL_TYPE_PUNCTUATION);
	//	Latin-1 Punctuations			0x0080, 0x00BF
	BOOST_CHECK_EQUAL(get_symbol_type(L'\x0080'), SYMBOL_TYPE_PUNCTUATION);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'\x00BF'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'—'), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'±'), SYMBOL_TYPE_PUNCTUATION);
	//	Latin-1 Punctuations			0x00D7
	BOOST_CHECK_EQUAL(get_symbol_type(L'×'), SYMBOL_TYPE_PUNCTUATION);
	//	Latin-1 Punctuations			0x00F7
	BOOST_CHECK_EQUAL(get_symbol_type(L'÷'), SYMBOL_TYPE_PUNCTUATION);
	//	Spacing Modifier Letters			0x02B0, 0x02FF
    BOOST_CHECK_EQUAL(get_symbol_type(L'ʰ'), SYMBOL_TYPE_PUNCTUATION);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'˿'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'˓'), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'˩'), SYMBOL_TYPE_PUNCTUATION);	
	//	General Punctuation				0x2000, 0x206F
	BOOST_CHECK_EQUAL(get_symbol_type(L' '), SYMBOL_TYPE_PUNCTUATION);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'\x206F'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'‘'), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'‣'), SYMBOL_TYPE_PUNCTUATION);
	//	Letterlike Symbols				0x2100, 0x214F
    BOOST_CHECK_EQUAL(get_symbol_type(L'℀'), SYMBOL_TYPE_PUNCTUATION);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'⅏'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'ℒ'), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'ℳ'), SYMBOL_TYPE_PUNCTUATION);		
	//	Arrows, Mathematical Operators, Miscellaneous Technical 		0x2190, 0x23FF	
	BOOST_CHECK_EQUAL(get_symbol_type(L'←'), SYMBOL_TYPE_PUNCTUATION);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'⏿'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'∊'), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'⍝'), SYMBOL_TYPE_PUNCTUATION);
	//	Box Drawing, Block Elements, Geometric Shapes, Miscellaneous Symbols	0x2500, 0x26FF	
	BOOST_CHECK_EQUAL(get_symbol_type(L'─'), SYMBOL_TYPE_PUNCTUATION);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'⛿'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'╔'), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'☻'), SYMBOL_TYPE_PUNCTUATION);
	//	Dingbats		 		0x2700, 0x2775
	BOOST_CHECK_EQUAL(get_symbol_type(L'✀'), SYMBOL_TYPE_PUNCTUATION);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'❵'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'✈'), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'✫'), SYMBOL_TYPE_PUNCTUATION);
	//	Dingbats				0x2794, 0x27BF
	BOOST_CHECK_EQUAL(get_symbol_type(L'➔'), SYMBOL_TYPE_PUNCTUATION);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'➿'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'➠'), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'➸'), SYMBOL_TYPE_PUNCTUATION);
	//	Miscellaneous Mathematical Symbols-A, Supplemental Arrows-A		0x27C0, 0x27FF
    BOOST_CHECK_EQUAL(get_symbol_type(L'⟀'), SYMBOL_TYPE_PUNCTUATION);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'⟿'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'⟰'), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'⟲'), SYMBOL_TYPE_PUNCTUATION);		
	//	Supplemental Arrows-B, Miscellaneous Mathematical Symbols-B, Supplemental Mathematical Operators, Miscellaneous Symbols and Arrows			0x2900, 0x2BFF
	BOOST_CHECK_EQUAL(get_symbol_type(L'⤀'), SYMBOL_TYPE_PUNCTUATION);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'⯿'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'⦎'), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'⯢'), SYMBOL_TYPE_PUNCTUATION);
	//	Supplemental Punctuation	0x2E00, 0x2E7F
	BOOST_CHECK_EQUAL(get_symbol_type(L'⸀'), SYMBOL_TYPE_PUNCTUATION);  //	begin 2E80
	BOOST_CHECK_EQUAL(get_symbol_type(L'⹿'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'⸛'), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'⹗'), SYMBOL_TYPE_PUNCTUATION);	
	//	CJK Symbols and Punctuation	0x3000, 0x303F
	BOOST_CHECK_EQUAL(get_symbol_type(L'　'), SYMBOL_TYPE_PUNCTUATION);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'〿'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'」'), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'〩'), SYMBOL_TYPE_PUNCTUATION);
	//	Fullwidth Forms				0xFF00, 0xFF0F
	BOOST_CHECK_EQUAL(get_symbol_type(L'＀'), SYMBOL_TYPE_PUNCTUATION);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'／'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'＄'), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'＊'), SYMBOL_TYPE_PUNCTUATION);
	//	Fullwidth Forms				0xFF1A, 0xFF20
	BOOST_CHECK_EQUAL(get_symbol_type(L'：'), SYMBOL_TYPE_PUNCTUATION);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'＠'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'；'), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'＞'), SYMBOL_TYPE_PUNCTUATION);	
	//	Fullwidth Form				0xFF3B, 0xFF40
	BOOST_CHECK_EQUAL(get_symbol_type(L'［'), SYMBOL_TYPE_PUNCTUATION);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'｀'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'＼'), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'＾'), SYMBOL_TYPE_PUNCTUATION);
	//	Fullwidth Forms				0xFF5B, 0xFF65
	BOOST_CHECK_EQUAL(get_symbol_type(L'｛'), SYMBOL_TYPE_PUNCTUATION);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'･'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'～'), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'｢'), SYMBOL_TYPE_PUNCTUATION);
	//	Fullwidth Forms				0xFFE0, 0xFFEF
	BOOST_CHECK_EQUAL(get_symbol_type(L'￠'), SYMBOL_TYPE_PUNCTUATION);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'￯'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'￤'), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'￦'), SYMBOL_TYPE_PUNCTUATION);
	//	Vertical Forms				0xFE00, 0xFE6F
    BOOST_CHECK_EQUAL(get_symbol_type(L'︀'), SYMBOL_TYPE_PUNCTUATION);	//	begin
	BOOST_CHECK_EQUAL(get_symbol_type(L'﹯'), SYMBOL_TYPE_PUNCTUATION);	//	end
	BOOST_CHECK_EQUAL(get_symbol_type(L'︰'), SYMBOL_TYPE_PUNCTUATION);
	BOOST_CHECK_EQUAL(get_symbol_type(L'﹑'), SYMBOL_TYPE_PUNCTUATION);		

	//	SYMBOL_TYPE_OTHER
}

BOOST_AUTO_TEST_CASE( test_exist )
{
	//	char
	BOOST_CHECK_EQUAL( exist('a', "adgmf"), true );
	BOOST_CHECK_EQUAL( exist('f', "adgmf"), true );
	BOOST_CHECK_EQUAL( exist('g', "adgmf"), true );
	BOOST_CHECK_EQUAL( exist('b', "adgmf"), false );
	BOOST_CHECK_EQUAL( exist('q', "adgmf"), false );
	BOOST_CHECK_EQUAL( exist('\0', "adgmf"), false );

	//	wchar_t
	BOOST_CHECK_EQUAL( exist(L'中', L",中文华夏."), true );
	BOOST_CHECK_EQUAL( exist(L'夏', L",中文华夏."), true );
	BOOST_CHECK_EQUAL( exist(L'文', L",中文华夏."), true );
	BOOST_CHECK_EQUAL( exist(L',', L",中文华夏."), true );
	BOOST_CHECK_EQUAL( exist(L'.', L",中文华夏."), true );
	BOOST_CHECK_EQUAL( exist(L'天', L",中文华夏."), false );
	BOOST_CHECK_EQUAL( exist(L'1', L",中文华夏."), false );
	BOOST_CHECK_EQUAL( exist(L'\0', L",中文华夏."), false );
}

BOOST_AUTO_TEST_CASE( test_locale )
{
	BOOST_CHECK_NO_THROW( make_locale(CHARSET_C) );
	BOOST_CHECK_NO_THROW( make_locale(CHARSET_ASCII) );
	BOOST_CHECK_NO_THROW( make_locale(CHARSET_GB2312) );
	BOOST_CHECK_NO_THROW( make_locale(CHARSET_GBK) );
	//BOOST_CHECK_NO_THROW( make_locale(CHARSET_GB18030) );
	BOOST_CHECK_NO_THROW( make_locale(CHARSET_BIG5) );
	BOOST_CHECK_NO_THROW( make_locale(CHARSET_UTF8) );
}

void test_encoding(const std::locale& loc, const wchar_t* wide_string, const char* narrow_string)
{
	BOOST_CHECK_EQUAL( narrow(wide_string, loc), narrow_string );
	BOOST_CHECK( widen(narrow_string, loc) == wide_string );
}

BOOST_AUTO_TEST_CASE( test_locale_gb2312 )
{
	std::locale loc;
	BOOST_REQUIRE_NO_THROW( loc = make_locale(CHARSET_GB2312) );
	test_encoding(loc, L"", "");
	test_encoding(loc, L"a", "a");
	test_encoding(loc, L"std::locale loc 1234567", "std::locale loc 1234567");
	test_encoding(loc, L"朱基,朱基,光,繁体字,codepage", "\xd6\xec\xbb\xf9\x2c\xd6\xec\xbb\xf9\x2c\xb9\xe2\x2c\xb7\xb1\xcc\xe5\xd7\xd6\x2c\x63\x6f\x64\x65\x70\x61\x67\x65");
}

BOOST_AUTO_TEST_CASE( test_locale_gbk )
{
	std::locale loc;
	BOOST_REQUIRE_NO_THROW( loc = make_locale(CHARSET_GBK) );
	test_encoding(loc, L"", "");
	test_encoding(loc, L"a", "a");
	test_encoding(loc, L"std::locale loc 1234567", "std::locale loc 1234567");
	test_encoding(loc, L"朱镕基,朱鎔基,光緒,繁体字,codepage", "\xd6\xec\xe9\x46\xbb\xf9\x2c\xd6\xec\xe6\x67\xbb\xf9\x2c\xb9\xe2\xbe\x77\x2c\xb7\xb1\xcc\xe5\xd7\xd6\x2c\x63\x6f\x64\x65\x70\x61\x67\x65");
}
//
//BOOST_AUTO_TEST_CASE( test_locale_gb18030 )
//{
//	std::locale loc;
//	BOOST_REQUIRE_NO_THROW( loc = make_locale(CHARSET_GB18030) );
//	test_encoding(loc, L"", "");
//	test_encoding(loc, L"a", "a");
//	test_encoding(loc, L"std::locale loc 1234567", "std::locale loc 1234567");
//	test_encoding(loc, L"朱镕基,朱鎔基,光緒,繁体字,codepage", "\xd6\xec\xe9\x46\xbb\xf9\x2c\xd6\xec\xe6\x67\xbb\xf9\x2c\xb9\xe2\xbe\x77\x2c\xb7\xb1\xcc\xe5\xd7\xd6\x2c\x63\x6f\x64\x65\x70\x61\x67\x65");
//}

BOOST_AUTO_TEST_CASE( test_locale_big5 )
{
	std::locale loc;
	BOOST_REQUIRE_NO_THROW( loc = make_locale(CHARSET_BIG5) );
	test_encoding(loc, L"", "");
	test_encoding(loc, L"a", "a");
	test_encoding(loc, L"std::locale loc 1234567", "std::locale loc 1234567");
	test_encoding(loc, L"朱基,朱鎔基,光緒,繁体字,codepage", "\xa6\xb6\xb0\xf2\x2c\xa6\xb6\xc2\xe8\xb0\xf2\x2c\xa5\xfa\xba\xfc\x2c\xc1\x63\xca\x5e\xa6\x72\x2c\x63\x6f\x64\x65\x70\x61\x67\x65");
}

BOOST_AUTO_TEST_CASE( test_locale_utf8 )
{
	std::locale loc;
	BOOST_REQUIRE_NO_THROW( loc = make_locale(CHARSET_UTF8) );
	test_encoding(loc, L"", "");
	test_encoding(loc, L"a", "a");
	test_encoding(loc, L"std::locale loc 1234567", "std::locale loc 1234567");
	test_encoding(loc, L"朱镕基,朱鎔基,光緒,繁体字,codepage", "\xe6\x9c\xb1\xe9\x95\x95\xe5\x9f\xba\x2c\xe6\x9c\xb1\xe9\x8e\x94\xe5\x9f\xba\x2c\xe5\x85\x89\xe7\xb7\x92\x2c\xe7\xb9\x81\xe4\xbd\x93\xe5\xad\x97\x2c\x63\x6f\x64\x65\x70\x61\x67\x65");
}

BOOST_AUTO_TEST_SUITE_END()

//	_OPENCLAS_UNIT_TEST_UTILITY_HPP_
#endif
