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
#ifndef _OPENCLAS_UNIT_TEST_LONGTIME_HPP_
#define _OPENCLAS_UNIT_TEST_LONGTIME_HPP_

#include <openclas/serialization.hpp>
#include <fstream>
#include <ctime>

BOOST_AUTO_TEST_SUITE( long_time_run )

/*******************************************************************
*
*				namespace openclas::ict
*
********************************************************************/

using namespace openclas;
using namespace openclas::ict;

static const char* core_dict_name = "data/coreDict.dct";
static const char* bigram_dict_name = "data/BigramDict.dct";
static const char* core_tag_name = "data/lexical.ctx";
//static const char* people_dict_basename = "data/nr";
//static const char* place_dict_basename = "data/ns";
//static const char* org_dict_basename = "data/tr";
//static const char* dict_ext = ".dct";
//static const char* tag_ext = ".ctx";


void test_file_existence(const char* filename)
{
	//	Test file existence
	std::ifstream in(filename);
	std::ostringstream oss;
	oss << "File [" << filename << "] is not exist.";
	BOOST_REQUIRE_MESSAGE( in , oss.str() );
}

void construct_dict_subset(Dictionary& dict, Dictionary& mini_dict, const std::wstring& text)
{
	for (std::wstring::const_iterator iter = text.begin(); iter != text.end(); ++iter)
	{
		std::vector<DictEntry*> entries = dict.prefix(iter, text.end());
		for (std::vector<DictEntry*>::iterator iEntry = entries.begin(); iEntry != entries.end(); ++iEntry)
		{
			DictEntry* entry = *iEntry;
			DictEntry* mini_entry = mini_dict.add_word(entry->word);
			*mini_entry = *entry;
		}
	}
}

int ms(clock_t tick)
{
	return static_cast<int>( (clock()-tick) * 1000. / CLOCKS_PER_SEC );
}

BOOST_AUTO_TEST_CASE( test_Serialization_dct_ocd_txt_gz )
{
	Dictionary dict;

	clock_t tick;
	test_file_existence(core_dict_name);
	BOOST_CHECK_EQUAL( dict.words().size(), 0 );
	
	tick = clock();
	load_words_from_dct(dict, core_dict_name);
	std::cout << "Load " << core_dict_name << " : \t" << ms(tick) << " ms" << std::endl;

	BOOST_CHECK_EQUAL( dict.words().size(), 85604 );
	//	count tags
	size_t word_tag_count = 0;
	for (Dictionary::word_dict_type::const_iterator iter = dict.words().begin(); iter != dict.words().end(); ++iter)
	{
		word_tag_count += (*iter)->tags.size();
	}
	BOOST_CHECK_EQUAL( word_tag_count, 104451 );	//	104455

	test_file_existence(bigram_dict_name);

	tick = clock();
	load_words_transit_from_dct(dict, bigram_dict_name);
	std::cout << "Load " << bigram_dict_name << " : \t" << ms(tick) << " ms" << std::endl;

	//	count word transit
	size_t transit_count = 0;
	for (Dictionary::word_dict_type::const_iterator iter = dict.words().begin(); iter != dict.words().end(); ++iter)
	{
		transit_count += (*iter)->forward.size();
	}
	BOOST_CHECK_EQUAL( transit_count, 408960 );

	//	load from .ctx
	tick = clock();
	load_tags_from_ctx(dict, core_tag_name);
	std::cout << "Load " << core_tag_name << " : \t" << ms(tick) << " ms" << std::endl;


	/******************************************
	 *		Test .ocd format (save and load)
	 ******************************************/
	const char* core_name = "data/core.ocd";

	tick = clock();
	save_to_ocd_file(dict, core_name);
	std::cout << "Save " << core_name << " : \t" << ms(tick) << " ms" << std::endl;

	Dictionary dict_ocd;

	tick = clock();
	load_from_ocd_file(dict_ocd, core_name);
	std::cout << "Load " << core_name << " : \t" << ms(tick) << " ms" << std::endl;

	BOOST_CHECK_EQUAL( dict_ocd.words().size(), 85604 );
	word_tag_count = 0;
	for (Dictionary::word_dict_type::const_iterator iter = dict_ocd.words().begin(); iter != dict_ocd.words().end(); ++iter)
	{
		word_tag_count += (*iter)->tags.size();
	}
	BOOST_CHECK_EQUAL( word_tag_count, 104451 );	//	104455

	transit_count = 0;
	for (Dictionary::word_dict_type::const_iterator iter = dict_ocd.words().begin(); iter != dict_ocd.words().end(); ++iter)
	{
		transit_count += (*iter)->forward.size();
	}
	BOOST_CHECK_EQUAL( transit_count, 408960 );

	/******************************************
	 *		Test .txt format (save and load)
	 ******************************************/

	const char* core_base_name = "data/core";

	tick = clock();
	save_to_txt_file(dict, core_base_name, true);
	std::cout << "Save " << core_base_name << ".{tag, unigram, bigram}.txt : \t" << ms(tick) << " ms" << std::endl;

	Dictionary dict_txt;

	tick = clock();
	load_from_txt_file(dict_txt, core_base_name, true);
	std::cout << "Load " << core_base_name << ".{tag, unigram, bigram}.txt : \t" << ms(tick) << " ms" << std::endl;

	BOOST_CHECK_EQUAL( dict_txt.words().size(), 85604 );
	word_tag_count = 0;
	for (Dictionary::word_dict_type::const_iterator iter = dict_txt.words().begin(); iter != dict_txt.words().end(); ++iter)
	{
		word_tag_count += (*iter)->tags.size();
	}
	BOOST_CHECK_EQUAL( word_tag_count, 104451 );	//	104455

	transit_count = 0;
	for (Dictionary::word_dict_type::const_iterator iter = dict_txt.words().begin(); iter != dict_txt.words().end(); ++iter)
	{
		transit_count += (*iter)->forward.size();
	}
	BOOST_CHECK_EQUAL( transit_count, 408960 );

	/******************************************
	 *		Test .gz format (save and load)
	 ******************************************/

	tick = clock();
	save_to_gz_file(dict, core_base_name, true);
	std::cout << "Save " << core_base_name << ".{tag, unigram, bigram}.gz : \t" << ms(tick) << " ms" << std::endl;

	Dictionary dict_gz;

	tick = clock();
	load_from_gz_file(dict_gz, core_base_name, true);
	std::cout << "Load " << core_base_name << ".{tag, unigram, bigram}.gz : \t" << ms(tick) << " ms" << std::endl;

	BOOST_CHECK_EQUAL( dict_gz.words().size(), 85604 );
	word_tag_count = 0;
	for (Dictionary::word_dict_type::const_iterator iter = dict_gz.words().begin(); iter != dict_gz.words().end(); ++iter)
	{
		word_tag_count += (*iter)->tags.size();
	}
	BOOST_CHECK_EQUAL( word_tag_count, 104451 );	//	104455

	transit_count = 0;
	for (Dictionary::word_dict_type::const_iterator iter = dict_gz.words().begin(); iter != dict_gz.words().end(); ++iter)
	{
		transit_count += (*iter)->forward.size();
	}
	BOOST_CHECK_EQUAL( transit_count, 408960 );

	/******************************************
	 *		generate mini dict
	 ******************************************/
	Dictionary mini_dict;

	//	add special word
	for (int i = 0; i < WORD_TAG_SIZE; ++i)
	{
		std::wstring special_word = get_special_word_string(static_cast<enum WordTag>(i));
		DictEntry* entry = dict.get_word(special_word);
		if (entry)
		{
			DictEntry* mini_entry = mini_dict.add_word(special_word);
			*mini_entry = *entry;
		}
	}

	//	add subset of normal word
	for (int i = 0; i < sample_count; ++i)
		construct_dict_subset(dict, mini_dict, sample[i]);

	//	one special
	//	Punctuation as Terminal of each segment
	DictEntry* entry_w = mini_dict.add_word(get_special_word_string(WORD_TAG_W));
	if (entry_w->tags.size() == 0)
	{
		DictEntry* entry_begin = mini_dict.get_word(get_special_word_string(WORD_TAG_BEGIN));
		entry_w->add(WORD_TAG_W, entry_begin->tags.front().weight);
	}

	tick = clock();
	save_to_txt_file(mini_dict, mini_dict_base_name, true);
	std::cout << "Save " << mini_dict_base_name << ".{tag, unigram, bigram}.txt : \t" << ms(tick) << " ms" << std::endl;
}


BOOST_AUTO_TEST_SUITE_END()

//	_OPENCLAS_UNIT_TEST_LONGTIME_HPP_
#endif
