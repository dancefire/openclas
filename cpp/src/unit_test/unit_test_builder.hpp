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
#ifndef _OPENCLAS_UNIT_TEST_BUILDER_HPP_
#define _OPENCLAS_UNIT_TEST_BUILDER_HPP_

#include <openclas/builder.hpp>
#include <openclas/dictionary.hpp>

BOOST_AUTO_TEST_SUITE( builder )

using namespace openclas;

void construct_dictionary(Dictionary& dict)
{
	DictEntry* entry = 0;

	entry = dict.add_word(get_special_word_string(WORD_TAG_BEGIN));
	entry->add(WORD_TAG_BEGIN, 50610);

	entry = dict.add_word(get_special_word_string(WORD_TAG_END));
	entry->add(WORD_TAG_END, 50610);

	entry = dict.add_word(get_special_word_string(WORD_TAG_NX));
	entry->add(WORD_TAG_NX, 459);	//	String

	entry = dict.add_word(get_special_word_string(WORD_TAG_NS));
	entry->add(WORD_TAG_NS, 964);	//	Place

	entry = dict.add_word(get_special_word_string(WORD_TAG_NR));
	entry->add(WORD_TAG_NR, 16294);	//	Person Name

	entry = dict.add_word(get_special_word_string(WORD_TAG_T));
	entry->add(WORD_TAG_T, 9716);	//	Date/Time

	entry = dict.add_word(get_special_word_string(WORD_TAG_M));
	entry->add(WORD_TAG_M, 17316);	//	Number

	entry = dict.add_word(get_special_word_string(WORD_TAG_N));
	entry->add(WORD_TAG_N, 4537);	//	Item name

	entry = dict.add_word(get_special_word_string(WORD_TAG_NT));
	entry->add(WORD_TAG_NT, 285);	//	Organization Name

	entry = dict.add_word(get_special_word_string(WORD_TAG_NZ));
	entry->add(WORD_TAG_NZ, 804);	//	Terminology


}

BOOST_AUTO_TEST_CASE( test_Builder_create_empty )
{
    Dictionary dict;
	construct_dictionary(dict);

	const wchar_t* empty_text = L"";
	std::vector<WordGraph> empty_graph_list = Builder::create(empty_text, dict);
	BOOST_CHECK_EQUAL( empty_graph_list.size(), 0 );
}

BOOST_AUTO_TEST_CASE( test_Builder_create_english_string )
{
    Dictionary dict;
	construct_dictionary(dict);

	const wchar_t* text = L"English Words";
	std::vector<WordGraph> graph_list = Builder::create(text, dict);
	BOOST_REQUIRE_EQUAL( graph_list.size(), 1 );
	WordGraph& graph = graph_list.at(0);
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

BOOST_AUTO_TEST_CASE( test_Builder_create_single_sentence )
{
    Dictionary dict;
	construct_dictionary(dict);

	const wchar_t* text = L"他说的确实在理。";
	property_map<WordGraph, vertex_desc_t>::type vprop_map;

	std::vector<WordGraph> graph_list = Builder::create(text, dict);
	BOOST_REQUIRE_EQUAL( graph_list.size(), 2 );

	WordGraph& graph = graph_list.at(0);
	vprop_map = get(vertex_desc, graph);
	BOOST_REQUIRE_EQUAL( num_vertices(graph), 3 );

	BOOST_CHECK_EQUAL( vprop_map[0].tag, WORD_TAG_BEGIN );
	BOOST_CHECK_EQUAL( vprop_map[1].tag, WORD_TAG_UNKNOWN );
	BOOST_CHECK_EQUAL( vprop_map[1].offset, 0 );
	BOOST_CHECK_EQUAL( vprop_map[1].length, 1 );
	BOOST_CHECK_EQUAL( vprop_map[2].tag, WORD_TAG_UNKNOWN );
	BOOST_CHECK_EQUAL( vprop_map[2].offset, 1 );
	BOOST_CHECK_EQUAL( vprop_map[2].length, 1 );

	graph = graph_list.at(1);
	vprop_map = get(vertex_desc, graph);
	BOOST_REQUIRE_EQUAL( num_vertices(graph), 12 );

	//BOOST_CHECK_EQUAL( vprop_map[0].tag, WORD_TAG_BEGIN );
	//BOOST_CHECK_EQUAL( vprop_map[1].tag, WORD_TAG_NX );
	//BOOST_CHECK_EQUAL( vprop_map[1].offset, 0 );
	//BOOST_CHECK_EQUAL( vprop_map[1].length, 7 );
	//BOOST_CHECK_EQUAL( vprop_map[2].tag, WORD_TAG_W );
	//BOOST_CHECK_EQUAL( vprop_map[2].offset, 7 );
	//BOOST_CHECK_EQUAL( vprop_map[2].length, 1 );
	//BOOST_CHECK_EQUAL( vprop_map[3].tag, WORD_TAG_NX );
	//BOOST_CHECK_EQUAL( vprop_map[3].offset, 8 );
	//BOOST_CHECK_EQUAL( vprop_map[3].length, 5 );
	//BOOST_CHECK_EQUAL( vprop_map[4].tag, WORD_TAG_END );
	//BOOST_CHECK_EQUAL( vprop_map[4].offset, 13 );
}

BOOST_AUTO_TEST_SUITE_END()

//	_OPENCLAS_UNIT_TEST_BUILDER_HPP_
#endif
