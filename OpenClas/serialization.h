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

#pragma once
#ifndef _OPENCLAS_SERIALIZATION_H_
#define _OPENCLAS_SERIALIZATION_H_

#include "dictionary.h"
#include <string>

namespace openclas {

	namespace ict {

		struct WordHeader {
			int weight;
			int length;
			int pos;
		};

		static const enum pku::WordTag SpecialWordTagIndex[] = {
			pku::WORD_TAG_BEGIN,
			pku::WORD_TAG_END,
			pku::WORD_TAG_NX,
			pku::WORD_TAG_NS,
			pku::WORD_TAG_NR,
			pku::WORD_TAG_T,
			pku::WORD_TAG_M,
			pku::WORD_TAG_N,
			pku::WORD_TAG_NT,
			pku::WORD_TAG_NZ
		};

		static const char_type* SpecialWordString[] = {
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

		const int SPECIAL_WORD_COUNT = 10;
		const int GB2312_COUNT = 6768;
		const std::locale locale_gbk(CodePageString[CODEPAGE_GBK]);

		void load_words_from_dct(Dictionary& dict, const char* filename);
		void load_words_transit_from_dct(Dictionary& dict, const char* filename);
		void load_tags_from_ctx(Dictionary& dict, const char* filename);
		void load_from_file(Dictionary& dict, const char* tag_filename, const char* words_filename, const char* words_transit_filename = 0);

		void save_words_to_dct(Dictionary& dict, const char* filename);
		void save_words_transit_to_dct(Dictionary& dict, const char* filename);
		void save_tags_to_ctx(Dictionary& dict, const char* filename);
		void save_to_file(Dictionary& dict, const char* tag_filename, const char* words_filename, const char* words_transit_filename = 0);
	}

	const unsigned short DICT_MAGIC_CODE = 'D' << 8 | 'C';

	struct DictHeader{
		unsigned short magic_code;
		unsigned short tag_count;
		int word_count;
	};

	struct WordHeader {
		unsigned char length;
		unsigned char tag_count;
		unsigned short transit_count;
	};

	struct TagItem{
		int tag;
		int weight;
	};

	struct TransitHeader {
		int length;
		int weight;
	};

	const std::locale locale_utf8(CodePageString[CODEPAGE_UTF8]);

	void load_from_dict(Dictionary& dict, const char* filename);
	void save_to_dict(Dictionary& dict, const char* filename);
}

#endif	//	_OPENCLAS_SERIALIZATION_H_
