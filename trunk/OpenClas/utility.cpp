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
*	$Date: 2009-01-25 00:47:56 +1100 (Sun, 25 Jan 2009) $
*	$Revision: 8 $
*	$Author: Dancefire $
*	$Id: graph.h 8 2009-01-24 13:47:56Z Dancefire $
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

}