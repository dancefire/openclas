/*
 * Copyright (c) 2007-2010 Tao Wang <dancefire@gmail.org>
 * See the file "LICENSE.txt" for usage and redistribution license requirements
 *
 *	$Id$
 */

#pragma once
#ifndef _OPENCLAS_UNIT_TEST_LONGTIME_HPP_
#define _OPENCLAS_UNIT_TEST_LONGTIME_HPP_

#include <openclas/serialization.hpp>
#include <openclas/segment.hpp>
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

BOOST_AUTO_TEST_CASE( test_Segment_performance )
{
	Dictionary dict;

	clock_t tick;
	const char* core_name = "data/core.ocd";
	test_file_existence(core_name);

	std::cout << "Loading dictionary ... ";
	tick = clock();
	load_from_ocd_file(dict, core_name);
	std::cout << "OK (" << ms(tick) << " ms)" << std::endl;

	//	Punctuation as Terminal of each segment
	DictEntry* entry_w = dict.add_word(get_special_word_string(WORD_TAG_W));
	if (entry_w->tags.size() == 0)
	{
		DictEntry* entry_begin = dict.get_word(get_special_word_string(WORD_TAG_BEGIN));
		entry_w->add(WORD_TAG_W, entry_begin->tags.front().weight);
	}

	tick = clock();
	std::cout << "Generating 1MB text ... ";
	std::wostringstream oss;
	int i = 0;
	size_t length = 0;
	while (length < 1024*1024 / sizeof(wchar_t) )
	{
		i = (i+1) % sample_count;
		length += wcslen(sample[i]) + 1;
		oss << sample[i] << L"，";
	}
	std::wstring content(oss.str());
	std::cout << "OK (" << ms(tick) << " ms)" << std::endl;

	tick = clock();
	std::cout << "Writing content to file ... ";
	std::wofstream content_out("data/content.txt", ios::out);
	content_out.imbue(locale_utf8);
	content_out << content;
	content_out.close();
	std::cout << "OK (" << ms(tick) << " ms)" << std::endl;

	tick = clock();
	std::cout << "Segmenting 1MB text ... ";
	std::vector<Segment::segment_type> segs = Segment::segment(content, dict, 1);
	int time_cost = ms(tick);
	std::cout << "OK (" << time_cost << " ms)" << std::endl;
	std::cout << "Segmented " << (content.size() * sizeof(wchar_t)) << " bytes in " << time_cost << " ms" << std::endl;
	std::cout << "Speed = " << ((content.size() * sizeof(wchar_t) / 1024.) / (time_cost / 1000.)) << " KB/s" << std::endl;

	tick = clock();
	std::cout << "Convert segs array to graph, and writing segment result to file ... ";
	std::wofstream segment_out("data/content_segment.txt", ios::out);
	segment_out.imbue(locale_utf8);
	segment_out << Segment::segment_to_string(content, segs.at(0));
	segment_out.close();
	std::cout << "OK (" << ms(tick) << " ms)" << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()

//	_OPENCLAS_UNIT_TEST_LONGTIME_HPP_
#endif
