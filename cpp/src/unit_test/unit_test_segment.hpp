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
#ifndef _OPENCLAS_UNIT_TEST_SEGMENT_HPP_
#define _OPENCLAS_UNIT_TEST_SEGMENT_HPP_

#include <openclas/segment.hpp>
#include <openclas/dictionary.hpp>
#include <openclas/serialization.hpp>

BOOST_AUTO_TEST_SUITE( segment )

using namespace openclas;

static const wchar_t* sample[] = {
	L"",
	L"English Words",
	L"他说的确实在理。",
	L"19９5年底ｇoｏgｌｅ在1月份大会上说的确实在理。",
	L"这个门的把手坏了好几天了",
	L"你把手抬高一点儿",
	L"这个动作的要领其实很简单",
	L"下午我要领工资，恐怕赶不回去",
	L"办独生子女证，一对夫妻一次性交一百元钱",
	L"我知道你不知道我知道你不知道我知道你不知道",
	L"已经分房和尚未分房的同志",
	L"馆内陈列着周恩来和邓颖超生前使用过的物品",
	L"张华平喜欢看书",
	L"舒展喜欢看书"
};

const int sample_count = 14;


DictEntry* dict_add_word(Dictionary& dict, const std::wstring& word, int tag, int weight)
{
	DictEntry* entry = dict.add_word(word);
	entry->add(tag, weight);
	return entry;
}

void dict_add_word_transit(Dictionary& dict, const std::wstring& word, const std::wstring& word_next, int weight)
{
	DictEntry* entry = dict.add_word(word);
	entry->forward[word_next] = weight;
}

void dict_add_special_word(Dictionary& dict, enum WordTag tag, int weight)
{
	dict_add_word(dict, get_special_word_string(tag), tag, weight);
}

void test_segment(std::wofstream& out, Dictionary& dict, int k, const std::wstring& text)
{
	std::vector<Segment::segment_type> segs = Segment::segment(text, dict, k);
	
	out << "Original text: " << text << std::endl;
	for (size_t i = 0; i < segs.size(); ++i)
	{
		out << "[" << i << "] Segment text : " << Segment::segment_to_string(text, segs[i]) << "\t(" << segs[i].weight << ")" << std::endl;
	}
	out << std::endl;
}

void construct_dictionary(Dictionary& dict)
{
	dict_add_special_word(dict, WORD_TAG_BEGIN, 50610);
	dict_add_special_word(dict, WORD_TAG_END,	50610);
	dict_add_special_word(dict, WORD_TAG_W,	50610);	//	Punctuation as Terminal of each segment
	dict_add_special_word(dict, WORD_TAG_NX, 459);	//	String
	dict_add_special_word(dict, WORD_TAG_NS, 964);	//	Place
	dict_add_special_word(dict, WORD_TAG_NR, 16294);	//	Person Name
	dict_add_special_word(dict, WORD_TAG_T, 9716);	//	Date/Time
	dict_add_special_word(dict, WORD_TAG_M, 17316);	//	Number
	dict_add_special_word(dict, WORD_TAG_N, 4537);	//	Item name
	dict_add_special_word(dict, WORD_TAG_NT, 285);	//	Organization Name
	dict_add_special_word(dict, WORD_TAG_NZ, 804);	//	Terminology

	DictEntry* entry;

	entry = dict.add_word(L"年");
	entry->add(WORD_TAG_A, 0);
	entry->add(WORD_TAG_F, 0);
	entry->add(WORD_TAG_M, 0);
	entry->add(WORD_TAG_N, 95);
	entry->add(WORD_TAG_NG, 0);
	entry->add(WORD_TAG_NR, 0);
	entry->add(WORD_TAG_Q, 2421);
	entry->add(WORD_TAG_T, 0);

	entry = dict.add_word(L"年底");
	entry->add(WORD_TAG_F, 0);
	entry->add(WORD_TAG_T, 46);

	entry = dict.add_word(L"底");
	entry->add(WORD_TAG_F, 149);
	entry->add(WORD_TAG_N, 5);
	entry->add(WORD_TAG_NG, 0);
	entry->add(WORD_TAG_U, 0);

	entry = dict.add_word(L"在");
	entry->add(WORD_TAG_C, 0);
	entry->add(WORD_TAG_D, 284);
	entry->add(WORD_TAG_F, 0);
	entry->add(WORD_TAG_P, 11481);
	entry->add(WORD_TAG_R, 0);
	entry->add(WORD_TAG_V, 257);

	entry = dict.add_word(L"月");
	entry->add(WORD_TAG_N, 295);
	entry->add(WORD_TAG_NR, 0);

	entry = dict.add_word(L"月份");
	entry->add(WORD_TAG_N, 0);

	entry = dict.add_word(L"份");
	entry->add(WORD_TAG_NG, 1);
	entry->add(WORD_TAG_Q, 157);

	entry = dict.add_word(L"大");
	entry->add(WORD_TAG_A, 2071);
	entry->add(WORD_TAG_AD, 0);
	entry->add(WORD_TAG_AN, 2);
	entry->add(WORD_TAG_D, 158);
	entry->add(WORD_TAG_J, 5);
	entry->add(WORD_TAG_N, 0);
	entry->add(WORD_TAG_NG, 8);
	entry->add(WORD_TAG_V, 0);

	entry = dict.add_word(L"大会");
	entry->add(WORD_TAG_N, 100);

	entry = dict.add_word(L"会");
	entry->add(WORD_TAG_N, 12);
	entry->add(WORD_TAG_V, 864);
	entry->add(WORD_TAG_VN, 0);
	entry->forward[L"上"] = 1;

	entry = dict.add_word(L"会上");
	entry->add(WORD_TAG_S, 0);
	entry->add(WORD_TAG_T, 66);

	entry = dict.add_word(L"上");
	entry->add(WORD_TAG_F, 3049);
	entry->add(WORD_TAG_J, 1);
	entry->add(WORD_TAG_N, 0);
	entry->add(WORD_TAG_NG, 1);
	entry->add(WORD_TAG_NR, 0);
	entry->add(WORD_TAG_U, 0);
	entry->add(WORD_TAG_V, 745);
	entry->forward[L"说"] = 64;

	entry = dict.add_word(L"他");
	entry->add(WORD_TAG_N, 2);
	entry->add(WORD_TAG_R, 2823);
	entry->add(WORD_TAG_V, 0);
	entry->forward[L"说"] = 355;

	entry = dict.add_word(L"说");
	entry->add(WORD_TAG_N, 1);
	entry->add(WORD_TAG_NG, 6);
	entry->add(WORD_TAG_V, 2549);
	entry->add(WORD_TAG_VN, 0);
	entry->forward[L"的"] = 43;

	entry = dict.add_word(L"的");
	entry->add(WORD_TAG_B, 0);
	entry->add(WORD_TAG_D, 0);
	entry->add(WORD_TAG_F, 0);
	entry->add(WORD_TAG_N, 0);
	entry->add(WORD_TAG_NG, 0);
	entry->add(WORD_TAG_P, 0);
	entry->add(WORD_TAG_R, 0);
	entry->add(WORD_TAG_V, 0);
	entry->add(WORD_TAG_W, 0);
	entry->forward[L"确实"] = 1;

	entry = dict.add_word(L"的确");
	entry->add(WORD_TAG_D, 27);

	entry = dict.add_word(L"确");
	entry->add(WORD_TAG_AG, 0);
	entry->add(WORD_TAG_D, 26);

	entry = dict.add_word(L"确实");
	entry->add(WORD_TAG_A, 8);
	entry->add(WORD_TAG_AD, 39);
	entry->add(WORD_TAG_D, 6);

	entry = dict.add_word(L"实");
	entry->add(WORD_TAG_A, 16);
	entry->add(WORD_TAG_AD, 10);
	entry->add(WORD_TAG_AN, 1);
	entry->add(WORD_TAG_DG, 0);
	entry->add(WORD_TAG_NG, 4);

	entry = dict.add_word(L"实在");
	entry->add(WORD_TAG_A, 7);
	entry->add(WORD_TAG_AN, 0);
	entry->add(WORD_TAG_D, 27);

	entry = dict.add_word(L"在");
	entry->add(WORD_TAG_C, 0);
	entry->add(WORD_TAG_D, 284);
	entry->add(WORD_TAG_F, 0);
	entry->add(WORD_TAG_P, 11481);
	entry->add(WORD_TAG_R, 0);
	entry->add(WORD_TAG_V, 257);

	entry = dict.add_word(L"在理");
	entry->add(WORD_TAG_A, 1);

	entry = dict.add_word(L"理");
	entry->add(WORD_TAG_J, 1);
	entry->add(WORD_TAG_N, 14);
	entry->add(WORD_TAG_V, 13);
}

BOOST_AUTO_TEST_CASE( test_Segment_create_empty )
{
    Dictionary dict;
	construct_dictionary(dict);

	const wchar_t* empty_text = L"";
	Segment::graph_list_type empty_graph_list = Segment::create_graphs(empty_text, dict);
	BOOST_CHECK_EQUAL( empty_graph_list.size(), 0 );
}

BOOST_AUTO_TEST_CASE( test_Segment_create_english_string )
{
    Dictionary dict;
	construct_dictionary(dict);

	const wchar_t* text = L"English Words";
	Segment::graph_list_type graph_list = Segment::create_graphs(text, dict);
	BOOST_REQUIRE_EQUAL( graph_list.size(), 1 );
	WordGraph& graph = *graph_list.at(0);
	BOOST_REQUIRE_EQUAL( num_vertices(graph), 5 );

	property_map<WordGraph, vertex_desc_t>::type vprop_map = get(vertex_desc, graph);

	BOOST_CHECK_EQUAL( vprop_map[0].tag, WORD_TAG_BEGIN );
	BOOST_CHECK_EQUAL( vprop_map[1].tag, WORD_TAG_NX );
	BOOST_CHECK_EQUAL( vprop_map[1].offset, 0 );
	BOOST_CHECK_EQUAL( vprop_map[1].length, 7 );
	BOOST_CHECK_EQUAL( vprop_map[2].tag, WORD_TAG_W );
	BOOST_CHECK_EQUAL( vprop_map[2].offset, 7 );
	BOOST_CHECK_EQUAL( vprop_map[2].length, 1 );
	BOOST_CHECK_EQUAL( vprop_map[3].tag, WORD_TAG_NX );
	BOOST_CHECK_EQUAL( vprop_map[3].offset, 8 );
	BOOST_CHECK_EQUAL( vprop_map[3].length, 5 );
	BOOST_CHECK_EQUAL( vprop_map[4].tag, WORD_TAG_END );
	BOOST_CHECK_EQUAL( vprop_map[4].offset, 13 );
}

BOOST_AUTO_TEST_CASE( test_Segment_create_single_sentence )
{
    Dictionary dict;
	construct_dictionary(dict);

	const wchar_t* text = L"19９5年底ｇoｏgｌｅ在1月份大会上说的确实在理。";
	property_map<WordGraph, vertex_desc_t>::type vprop_map;
	graph_property<WordGraph, graph_terminal_t>::type gterminal;

	Segment::graph_list_type graph_list = Segment::create_graphs(text, dict);
	BOOST_REQUIRE_EQUAL( graph_list.size(), 3 );

	//	"19９5年底ｇoｏgｌｅ在1"
	WordGraph& graph = *graph_list.at(0);
	vprop_map = get(vertex_desc, graph);
	gterminal = get_property(graph, graph_terminal);

	BOOST_REQUIRE_EQUAL( num_vertices(graph), 8 );
	//	[Begin]
	BOOST_CHECK_EQUAL( vprop_map[gterminal.first].tag, WORD_TAG_BEGIN );
	//	"19９5"
	BOOST_CHECK_EQUAL( vprop_map[1].tag, WORD_TAG_M );
	BOOST_CHECK_EQUAL( vprop_map[1].offset, 0 );
	BOOST_CHECK_EQUAL( vprop_map[1].length, 4 );
	//	"年"
	BOOST_CHECK_EQUAL( vprop_map[2].tag, WORD_TAG_UNKNOWN );
	BOOST_CHECK_EQUAL( vprop_map[2].offset, 4 );
	BOOST_CHECK_EQUAL( vprop_map[2].length, 1 );
	//	"1"
	BOOST_CHECK_EQUAL( vprop_map[gterminal.second].tag, WORD_TAG_M );
	BOOST_CHECK_EQUAL( vprop_map[gterminal.second].offset, 13 );
	BOOST_CHECK_EQUAL( vprop_map[gterminal.second].length, 1 );

	//	"1月份大会上说"
	graph = *graph_list.at(1);
	vprop_map = get(vertex_desc, graph);
	gterminal = get_property(graph, graph_terminal);
	BOOST_REQUIRE_EQUAL( num_vertices(graph), 10 );
	//	"1"
	BOOST_CHECK_EQUAL( vprop_map[gterminal.first].tag, WORD_TAG_M );
	BOOST_CHECK_EQUAL( vprop_map[gterminal.first].offset, 13 );
	BOOST_CHECK_EQUAL( vprop_map[gterminal.first].length, 1 );
	//	"月"
	BOOST_CHECK_EQUAL( vprop_map[1].tag, WORD_TAG_UNKNOWN );
	BOOST_CHECK_EQUAL( vprop_map[1].offset, 14 );
	BOOST_CHECK_EQUAL( vprop_map[1].length, 1 );
	//	"说"
	BOOST_CHECK_EQUAL( vprop_map[gterminal.second].tag, WORD_TAG_UNKNOWN );
	BOOST_CHECK_EQUAL( vprop_map[gterminal.second].offset, 19 );
	BOOST_CHECK_EQUAL( vprop_map[gterminal.second].length, 1 );

	//	"说的确实在理。"
	graph = *graph_list.at(2);
	vprop_map = get(vertex_desc, graph);
	gterminal = get_property(graph, graph_terminal);
	BOOST_REQUIRE_EQUAL( num_vertices(graph), 12 );
	//	"说"
	BOOST_CHECK_EQUAL( vprop_map[gterminal.first].tag, WORD_TAG_UNKNOWN );
	BOOST_CHECK_EQUAL( vprop_map[gterminal.first].offset, 19 );
	BOOST_CHECK_EQUAL( vprop_map[gterminal.first].length, 1 );
	//	"的"
	BOOST_CHECK_EQUAL( vprop_map[1].tag, WORD_TAG_UNKNOWN );
	BOOST_CHECK_EQUAL( vprop_map[1].offset, 20 );
	BOOST_CHECK_EQUAL( vprop_map[1].length, 1 );
	//	[End]
	BOOST_CHECK_EQUAL( vprop_map[gterminal.second].tag, WORD_TAG_END );

	/***************************************************************
	 *
	 *		Test segment(graphs, k) and segment_to_string()
	 *
	 ***************************************************************/
	std::vector<Segment::segment_type> segs = Segment::segment(graph_list, 3);
	BOOST_REQUIRE_EQUAL( segs.size(), 3 );

	BOOST_CHECK_CLOSE( segs[0].weight, 0., 0.00001 );
	BOOST_CHECK( Segment::segment_to_string(text, segs[0]) ==
		L"19９5/m 年底/ ｇoｏgｌｅ/nx 在/ 1/m 月份/n 大会/n 上/ 说/ 的确/d 实在/ 理/ 。/w" );

	BOOST_CHECK_CLOSE( segs[1].weight, 0., 0.00001 );
	BOOST_CHECK( Segment::segment_to_string(text, segs[1]) ==
		L"19９5/m 年底/ ｇoｏgｌｅ/nx 在/ 1/m 月份/n 大会/n 上/ 说/ 的/ 确实/ 在理/a 。/w" );

	BOOST_CHECK_CLOSE( segs[2].weight, 0., 0.00001 );
	BOOST_CHECK( Segment::segment_to_string(text, segs[2]) ==
		L"19９5/m 年底/ ｇoｏgｌｅ/nx 在/ 1/m 月份/n 大会/n 上/ 说/ 的确/d 实/ 在理/a 。/w" );

	wcout.imbue(locale_gbk);
	wcout << "Original text: " << text << std::endl;
	for (size_t i = 0; i < segs.size(); ++i)
	{
		wcout << "[" << i << "] Segment text : " << Segment::segment_to_string(text, segs[i]) << "\t(" << segs[i].weight << ")" << std::endl;
	}
	wcout << std::endl;
}

BOOST_AUTO_TEST_CASE( test_Segment_segment_single_sentence )
{
    Dictionary dict;
	
	construct_dictionary(dict);
	load_from_ocd_file(dict, "data/core.ocd");

	std::wofstream out("Segment.txt");
	out.imbue(locale_utf8);

	for (int i = 0; i < sample_count; ++i)
	{
		test_segment(out, dict, 3, sample[i]);
	}

	out.close();
}

BOOST_AUTO_TEST_SUITE_END()

//	_OPENCLAS_UNIT_TEST_SEGMENT_HPP_
#endif
