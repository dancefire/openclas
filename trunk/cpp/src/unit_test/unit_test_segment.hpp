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

BOOST_AUTO_TEST_CASE( test_Segment_create_empty )
{
    Dictionary dict;
	load_from_txt_file(dict, mini_dict_base_name, true);

	const wchar_t* empty_text = L"";
	Segment::graph_list_type empty_graph_list = Segment::create_graphs(empty_text, dict);
	BOOST_CHECK_EQUAL( empty_graph_list.size(), 0 );
}

BOOST_AUTO_TEST_CASE( test_Segment_create_english_string )
{
    Dictionary dict;
	load_from_txt_file(dict, mini_dict_base_name, true);

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
	load_from_txt_file(dict, mini_dict_base_name, true);

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
	std::vector<Segment::segment_type> segs = Segment::segment(text, dict, 3);
	BOOST_REQUIRE_EQUAL( segs.size(), 3 );

	const double weights[] = {136420, 136427, 136429};
	const wchar_t* seg_correct[] = {
		L"19９5/m 年底/ ｇoｏgｌｅ/nx 在/ 1/m 月份/n 大会/n 上/ 说/ 的/ 确实/ 在理/a 。/w",
		L"19９5/m 年底/ ｇoｏgｌｅ/nx 在/ 1/m 月份/n 大/ 会上/ 说/ 的/ 确实/ 在理/a 。/w",
		L"19９5/m 年底/ ｇoｏgｌｅ/nx 在/ 1/m 月/ 份/ 大会/n 上/ 说/ 的/ 确实/ 在理/a 。/w"
	};

	for (size_t i = 0; i < 3; ++i)
	{
		BOOST_CHECK_CLOSE( segs[i].weight, weights[i], 0.001 );
		std::wstring seg_text = Segment::segment_to_string(text, segs[i]);
		BOOST_CHECK( seg_text == seg_correct[i] );
	}
}

BOOST_AUTO_TEST_CASE( test_Segment_segment_single_sentence )
{
    Dictionary dict;
	
	load_from_txt_file(dict, mini_dict_base_name, true);

	std::wofstream out("data/segment_test.txt");
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
