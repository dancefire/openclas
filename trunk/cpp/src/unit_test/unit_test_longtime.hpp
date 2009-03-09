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


��Ȩ���� 2007-2009 Dancefire (dancefire@gmail.com)��
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


BOOST_AUTO_TEST_CASE( test_Serialization_dct_ocd_txt_gz )
{
	Dictionary dict;

	clock_t tick;
	test_file_existence(core_dict_name);
	BOOST_CHECK_EQUAL( dict.words().size(), 0 );
	
	tick = clock();
	load_words_from_dct(dict, core_dict_name);
	std::cout << "Load " << core_dict_name << " : \t" << (clock() - tick) << " ms" << std::endl;

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
	std::cout << "Load " << bigram_dict_name << " : \t" << (clock() - tick) << " ms" << std::endl;

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
	std::cout << "Load " << core_tag_name << " : \t" << (clock() - tick) << " ms" << std::endl;


	/******************************************
	 *		Test .ocd format (save and load)
	 ******************************************/
	const char* core_name = "data/core.ocd";

	tick = clock();
	save_to_ocd_file(dict, core_name);
	std::cout << "Save " << core_name << " : \t" << (clock() - tick) << " ms" << std::endl;

	Dictionary dict_ocd;

	tick = clock();
	load_from_ocd_file(dict_ocd, core_name);
	std::cout << "Load " << core_name << " : \t" << (clock() - tick) << " ms" << std::endl;

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
	std::cout << "Save " << core_base_name << ".{tag, unigram, bigram}.txt : \t" << (clock() - tick) << " ms" << std::endl;

	Dictionary dict_txt;

	tick = clock();
	load_from_txt_file(dict_txt, core_base_name, true);
	std::cout << "Load " << core_base_name << ".{tag, unigram, bigram}.txt : \t" << (clock() - tick) << " ms" << std::endl;

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
	std::cout << "Save " << core_base_name << ".{tag, unigram, bigram}.gz : \t" << (clock() - tick) << " ms" << std::endl;

	Dictionary dict_gz;

	tick = clock();
	load_from_gz_file(dict_gz, core_base_name, true);
	std::cout << "Load " << core_base_name << ".{tag, unigram, bigram}.gz : \t" << (clock() - tick) << " ms" << std::endl;

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
	std::cout << "Save " << mini_dict_base_name << ".{tag, unigram, bigram}.txt : \t" << (clock() - tick) << " ms" << std::endl;
}


BOOST_AUTO_TEST_SUITE_END()

//	_OPENCLAS_UNIT_TEST_LONGTIME_HPP_
#endif
