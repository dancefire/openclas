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
#include "common.h"


namespace ictclas{

	const wchar_t* PKU_TAG_STRING[] = 
	{
		L"ag",	//	形语素	形容词性语素。形容词代码为a，语素代码ｇ前面置以A。
		L"a",	//	形容词	取英语形容词adjective的第1个字母。
		L"ad",	//	副形词	直接作状语的形容词。形容词代码a和副词代码d并在一起。
		L"an",	//	名形词	具有名词功能的形容词。形容词代码a和名词代码n并在一起。
		L"b",	//	区别词	取汉字“别”的声母。
		L"c",	//	连词		取英语连词conjunction的第1个字母。
		L"dg",	//	副语素	副词性语素。副词代码为d，语素代码ｇ前面置以D。
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
		L"z"		//	状态词	取汉字“状”的声母的前一个字母。
	};
	class TaggingConstants{
		enum PKU_Tag_Index{
			ag,	//	形语素	形容词性语素。形容词代码为a，语素代码ｇ前面置以A。
			a,	//	形容词	取英语形容词adjective的第1个字母。
			ad,	//	副形词	直接作状语的形容词。形容词代码a和副词代码d并在一起。
			an,	//	名形词	具有名词功能的形容词。形容词代码a和名词代码n并在一起。
			b,	//	区别词	取汉字“别”的声母。
			c,	//	连词		取英语连词conjunction的第1个字母。
			dg,	//	副语素	副词性语素。副词代码为d，语素代码ｇ前面置以D。
			d,	//	副词		取adverb的第2个字母，因其第1个字母已用于形容词。
			e,	//	叹词		取英语叹词exclamation的第1个字母。
			f,	//	方位词	取汉字“方” 的声母。
			g,	//	语素		绝大多数语素都能作为合成词的“词根”，取汉字“根”的声母。
			h,	//	前接成分	取英语head的第1个字母。
			i,	//	成语		取英语成语idiom的第1个字母。
			j,	//	简称略语	取汉字“简”的声母。
			k,	//	后接成分
			l,	//	习用语	习用语尚未成为成语，有点“临时性”，取“临”的声母。
			m,	//	数词		取英语numeral的第3个字母，n，u已有他用。
			ng,	//	名语素	名词性语素。名词代码为n，语素代码ｇ前面置以N。
			n,	//	名词		取英语名词noun的第1个字母。
			nr,	//	人名		名词代码n和“人(ren)”的声母并在一起。
			ns,	//	地名		名词代码n和处所词代码s并在一起。
			nt,	//	机构团体	“团”的声母为t，名词代码n和t并在一起。
			nz,	//	其他专名	“专”的声母的第1个字母为z，名词代码n和z并在一起。 
			o,	//	 拟声词	取英语拟声词onomatopoeia的第1个字母。
			p,	//	介词		取英语介词prepositional的第1个字母。
			q,	//	量词		取英语quantity的第1个字母。
			r,	//	代词		取英语代词pronoun的第2个字母,因p已用于介词。
			s,	//	处所词	取英语space的第1个字母。
			tg,	//	时语素	时间词性语素。时间词代码为t,在语素的代码g前面置以T。
			t,	//	时间词	取英语time的第1个字母。
			u,	//	助词		取英语助词auxiliary 的第2个字母,因a已用于形容词。
			vg,	//	动语素	动词性语素。动词代码为v。在语素的代码g前面置以V。
			v,	//	动词		取英语动词verb的第一个字母。
			vd,	//	副动词	直接作状语的动词。动词和副词的代码并在一起。
			vn,	//	名动词	指具有名词功能的动词。动词和名词的代码并在一起。
			w,	//	标点符号   
			x,	//	非语素字	非语素字只是一个符号，字母x通常用于代表未知数、符号。
			y,	//	语气词	取汉字“语”的声母。
			z,	//	状态词	取汉字“状”的声母的前一个字母。
			PKU_Tag_Index_Count
		};
	};


}

namespace openclas {
	enum TagTypePKU{
		TAG_PKU_UNKNOWN,
		TAG_PKU_AG,	//	形语素	形容词性语素。形容词代码为a，语素代码g前面置以a。
		TAG_PKU_A,	//	形容词	取英语形容词adjective的第1个字母。
		TAG_PKU_AD,	//	副形词	直接作状语的形容词。形容词代码a和副词代码d并在一起。
		TAG_PKU_AN,	//	名形词	具有名词功能的形容词。形容词代码a和名词代码N并在一起。
		TAG_PKU_B,	//	区别词	取汉字“别”的声母。
		TAG_PKU_C,	//	连词		取英语连词conjunction的第1个字母。
		TAG_PKU_DG,	//	副语素	副词性语素。副词代码为d，语素代码g前面置以d。
		TAG_PKU_D,	//	副词		取adverb的第2个字母，因其第1个字母已用于形容词。
		TAG_PKU_E,	//	叹词		取英语叹词exclamation的第1个字母。
		TAG_PKU_F,	//	方位词	取汉字“方” 的声母。
		TAG_PKU_G,	//	语素		绝大多数语素都能作为合成词的“词根”，取汉字“根”的声母。
		TAG_PKU_H,	//	前接成分	取英语head的第1个字母。
		TAG_PKU_I,	//	成语		取英语成语idiom的第1个字母。
		TAG_PKU_J,	//	简称略语	取汉字“简”的声母。
		TAG_PKU_K,	//	后接成分
		TAG_PKU_L,	//	习用语	习用语尚未成为成语，有点“临时性”，取“临”的声母。
		TAG_PKU_M,	//	数词		取英语numeral的第3个字母，n，u已有他用。
		TAG_PKU_NG,	//	名语素	名词性语素。名词代码为n，语素代码g前面置以n。
		TAG_PKU_N,	//	名词		取英语名词noun的第1个字母。
		TAG_PKU_NR,	//	人名		名词代码n和“人(ren)”的声母并在一起。
		TAG_PKU_NS,	//	地名		名词代码n和处所词代码s并在一起。
		TAG_PKU_NT,	//	机构团体	“团”的声母为t，名词代码n和t并在一起。
		TAG_PKU_NZ,	//	其他专名	“专”的声母的第1个字母为z，名词代码n和z并在一起。 
		TAG_PKU_O,	//	 拟声词	取英语拟声词onomatopoeia的第1个字母。
		TAG_PKU_P,	//	介词		取英语介词prepositional的第1个字母。
		TAG_PKU_Q,	//	量词		取英语quantity的第1个字母。
		TAG_PKU_R,	//	代词		取英语代词pronoun的第2个字母,因p已用于介词。
		TAG_PKU_S,	//	处所词	取英语space的第1个字母。
		TAG_PKU_TG,	//	时语素	时间词性语素。时间词代码为t,在语素的代码g前面置以t。
		TAG_PKU_T,	//	时间词	取英语time的第1个字母。
		TAG_PKU_U,	//	助词		取英语助词auxiliary 的第2个字母,因a已用于形容词。
		TAG_PKU_VG,	//	动语素	动词性语素。动词代码为v。在语素的代码g前面置以v。
		TAG_PKU_V,	//	动词		取英语动词verb的第一个字母。
		TAG_PKU_VD,	//	副动词	直接作状语的动词。动词和副词的代码并在一起。
		TAG_PKU_VN,	//	名动词	指具有名词功能的动词。动词和名词的代码并在一起。
		TAG_PKU_W,	//	标点符号   
		TAG_PKU_X,	//	非语素字	非语素字只是一个符号，字母x通常用于代表未知数、符号。
		TAG_PKU_Y,	//	语气词	取汉字“语”的声母。
		TAG_PKU_Z	//	状态词	取汉字“状”的声母的前一个字母。
	};

	const size_t TAG_PKU_COUNT = static_cast<size_t>(TAG_PKU_Z + 1);	//	Get the count of TagTypePKU

	const char_type* TAG_PKU_NAME[] = 
	{
		L"",
		L"ag",	//	形语素	形容词性语素。形容词代码为a，语素代码ｇ前面置以A。
		L"a",	//	形容词	取英语形容词adjective的第1个字母。
		L"ad",	//	副形词	直接作状语的形容词。形容词代码a和副词代码d并在一起。
		L"an",	//	名形词	具有名词功能的形容词。形容词代码a和名词代码n并在一起。
		L"b",	//	区别词	取汉字“别”的声母。
		L"c",	//	连词		取英语连词conjunction的第1个字母。
		L"dg",	//	副语素	副词性语素。副词代码为d，语素代码ｇ前面置以D。
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
		L"z"		//	状态词	取汉字“状”的声母的前一个字母。
	};
}
