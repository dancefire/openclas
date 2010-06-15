/*
 * Copyright (c) 2007-2010 Tao Wang <dancefire@gmail.org>
 * See the file "LICENSE.txt" for usage and redistribution license requirements
 *
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
