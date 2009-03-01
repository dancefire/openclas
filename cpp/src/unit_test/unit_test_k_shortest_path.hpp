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
#ifndef _OPENCLAS_UNIT_TEST_K_SHORTEST_PATH_HPP_
#define _OPENCLAS_UNIT_TEST_K_SHORTEST_PATH_HPP_

#include <openclas/k_shortest_path.hpp>
#include <openclas/segment.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/test/floating_point_comparison.hpp>

BOOST_AUTO_TEST_SUITE( k_shortest_path )

using namespace openclas;
using namespace boost;

enum TestGraphKind {
	GRAPH_EMPTY,
	GRAPH_SINGLE_NODE,
	GRAPH_SINGLE_EDGE,
	GRAPH_SINGLE_PATH,
	GRAPH_SIMPLE,
	GRAPH_COMPLEX
};

WordGraph generate_test_graph(enum TestGraphKind kind)
{
	switch(kind)
	{
	case GRAPH_SINGLE_NODE:
		{
			return WordGraph(1);
		}
	case GRAPH_SINGLE_EDGE:
		{
			WordGraph g(2);
			add_edge(0, 1, 100, g);
			return g;
		}
	case GRAPH_SINGLE_PATH:
		{
			WordGraph g(4);
			add_edge(0, 1, 100, g);
			add_edge(1, 2, 20, g);
			add_edge(2, 3, 3, g);
			return g;
		}
	case GRAPH_SIMPLE:
		{
			/*
			 *	Paths:
			 *	weight| path
			 *		3 | 0, 3
			 *		5 | 0, 1, 3
			 *		7 | 0, 2, 3
			 */
			WordGraph g(4);
			add_edge(0, 1, 1, g);
			add_edge(0, 2, 2, g);
			add_edge(0, 3, 3, g);
			add_edge(1, 3, 4, g);
			add_edge(2, 3, 5, g);
			return g;
		}
	case GRAPH_COMPLEX:
		{
			/*
			 *	Paths:
			 *			|	weight	|	path
			 *	--------+-----------+-------------------------
			 *		0	|	29.64	|	0, 1, 7, 4, 5, 10
			 *		1	|	33.42	|	0, 1, 7, 9, 10
			 *		2	|	41.72	|	0, 6, 8, 5, 10
			 *		3	|	42.90	|	0, 1, 2, 8, 5, 10
			 *		4	|	43.88	|	0, 6, 3, 9, 10
			 *		5	|	45.06	|	0, 1, 2, 3, 9, 10
			 *		6	|	47.15	|	0, 6, 3, 4, 5, 10
			 *		7	|	48.33	|	0, 1, 2, 3, 4, 5, 10
			 */
			WordGraph g(11);
			add_edge(0, 1, 4.05, g);
			add_edge(0, 6, 7.11, g);
			add_edge(1, 2, 4.10, g);
			add_edge(1, 7, 4.10, g);
			add_edge(2, 3, 11.63, g);
			add_edge(2, 8, 11.63, g);
			add_edge(3, 4, 10.98, g);
			add_edge(3, 9, 10.98, g);
			add_edge(4, 5, 5.62, g);
			add_edge(5, 10, 11.95, g);
			add_edge(6, 3, 11.49, g);
			add_edge(6, 8, 11.49, g);
			add_edge(7, 4, 3.92, g);
			add_edge(7, 9, 10.97, g);
			add_edge(8, 5, 11.17, g);
			add_edge(9, 10, 14.30, g);
			return g;
		}
	case GRAPH_EMPTY:
	default:
		{
			return WordGraph(0);
		}
	}
}
/****************************************************
 *
 *					dag_all_paths
 *
 ****************************************************/
BOOST_AUTO_TEST_CASE( test_ksp_all_paths_empty_graph )
{
	WordGraph g = generate_test_graph(GRAPH_EMPTY);

	std::vector<path_type> result;
	std::pair<WordGraph::vertex_iterator, WordGraph::vertex_iterator> nodes = vertices(g);
	dag_all_paths(g, *nodes.first, *nodes.second, result);

	BOOST_CHECK_EQUAL( result.size(), 0 );
}

BOOST_AUTO_TEST_CASE( test_ksp_all_paths_single_node )
{
	WordGraph g = generate_test_graph(GRAPH_SINGLE_NODE);

	std::vector<path_type> result;
	dag_all_paths(g, 0, 0, result);

	BOOST_REQUIRE_EQUAL( result.size(), 1 );
	BOOST_CHECK_CLOSE( result[0].weight, 0., 0.00001 );
	BOOST_REQUIRE_EQUAL( result[0].nodelist.size(), 1 );
	BOOST_CHECK_EQUAL( result[0].nodelist[0], 0 );
}

BOOST_AUTO_TEST_CASE( test_ksp_all_paths_single_edge )
{
	WordGraph g = generate_test_graph(GRAPH_SINGLE_EDGE);

	std::vector<path_type> result;
	property_map<WordGraph, edge_weight_t>::type
		w_map = get(edge_weight, g);
	dag_all_paths(g, 0, 1, result);

	BOOST_REQUIRE_EQUAL( result.size(), 1 );
	BOOST_CHECK_CLOSE( result[0].weight, 100., 0.00001 );
	BOOST_REQUIRE_EQUAL( result[0].nodelist.size(), 2 );
	BOOST_CHECK_EQUAL( result[0].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[0].nodelist[1], 1 );
}

BOOST_AUTO_TEST_CASE( test_ksp_all_paths_single_path )
{
	WordGraph g = generate_test_graph(GRAPH_SINGLE_PATH);

	std::vector<path_type> result;
	property_map<WordGraph, edge_weight_t>::type
		w_map = get(edge_weight, g);
	dag_all_paths(g, 0, 3, result);

	BOOST_REQUIRE_EQUAL( result.size(), 1 );
	BOOST_CHECK_CLOSE( result[0].weight, 123., 0.00001 );
	BOOST_REQUIRE_EQUAL( result[0].nodelist.size(), 4 );
	BOOST_CHECK_EQUAL( result[0].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[0].nodelist[1], 1 );
	BOOST_CHECK_EQUAL( result[0].nodelist[2], 2 );
	BOOST_CHECK_EQUAL( result[0].nodelist[3], 3 );
}

BOOST_AUTO_TEST_CASE( test_ksp_all_paths_simple_graph )
{
	WordGraph g = generate_test_graph(GRAPH_SIMPLE);

	std::vector<path_type> result;
	property_map<WordGraph, edge_weight_t>::type
		w_map = get(edge_weight, g);
	dag_all_paths(g, 0, 3, result);

	BOOST_REQUIRE_EQUAL( result.size(), 3 );
	std::sort(result.begin(), result.end());
	//	Path 0	|	3	|	0, 3
	BOOST_CHECK_CLOSE( result[0].weight, 3., 0.00001 );
	BOOST_REQUIRE_EQUAL( result[0].nodelist.size(), 2 );
	BOOST_CHECK_EQUAL( result[0].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[0].nodelist[1], 3 );
	//	Path 1	|	5	|	0, 1, 3
	BOOST_CHECK_CLOSE( result[1].weight, 5., 0.00001 );
	BOOST_REQUIRE_EQUAL( result[1].nodelist.size(), 3 );
	BOOST_CHECK_EQUAL( result[1].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[1].nodelist[1], 1 );
	BOOST_CHECK_EQUAL( result[1].nodelist[2], 3 );
	//	Path 2	|	7	|	0, 2, 3
	BOOST_CHECK_CLOSE( result[2].weight, 7., 0.00001 );
	BOOST_REQUIRE_EQUAL( result[2].nodelist.size(), 3 );
	BOOST_CHECK_EQUAL( result[2].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[2].nodelist[1], 2 );
	BOOST_CHECK_EQUAL( result[2].nodelist[2], 3 );
}

BOOST_AUTO_TEST_CASE( test_ksp_all_paths_complex_graph )
{
	WordGraph g = generate_test_graph(GRAPH_COMPLEX);

	std::vector<path_type> result;
	property_map<WordGraph, edge_weight_t>::type
		w_map = get(edge_weight, g);
	dag_all_paths(g, 0, 10, result);

	BOOST_REQUIRE_EQUAL( result.size(), 8 );
	std::sort(result.begin(), result.end());
	//	Path 0	|	29.64	|	0, 1, 7, 4, 5, 10
	BOOST_CHECK_CLOSE( result[0].weight, 29.64, 0.00001 );
	BOOST_REQUIRE_EQUAL( result[0].nodelist.size(), 6 );
	BOOST_CHECK_EQUAL( result[0].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[0].nodelist[1], 1 );
	BOOST_CHECK_EQUAL( result[0].nodelist[2], 7 );
	BOOST_CHECK_EQUAL( result[0].nodelist[3], 4 );
	BOOST_CHECK_EQUAL( result[0].nodelist[4], 5 );
	BOOST_CHECK_EQUAL( result[0].nodelist[5], 10 );
	//	Path 1	|	33.42	|	0, 1, 7, 9, 10
	BOOST_CHECK_CLOSE( result[1].weight, 33.42, 0.00001 );
	BOOST_REQUIRE_EQUAL( result[1].nodelist.size(), 5 );
	BOOST_CHECK_EQUAL( result[1].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[1].nodelist[1], 1 );
	BOOST_CHECK_EQUAL( result[1].nodelist[2], 7 );
	BOOST_CHECK_EQUAL( result[1].nodelist[3], 9 );
	BOOST_CHECK_EQUAL( result[1].nodelist[4], 10 );
	//	Path 2	|	41.72	|	0, 6, 8, 5, 10
	BOOST_CHECK_CLOSE( result[2].weight, 41.72, 0.00001 );
	BOOST_REQUIRE_EQUAL( result[2].nodelist.size(), 5 );
	BOOST_CHECK_EQUAL( result[2].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[2].nodelist[1], 6 );
	BOOST_CHECK_EQUAL( result[2].nodelist[2], 8 );
	BOOST_CHECK_EQUAL( result[2].nodelist[3], 5 );
	BOOST_CHECK_EQUAL( result[2].nodelist[4], 10 );
	//	Path 3	|	42.90	|	0, 1, 2, 8, 5, 10
	BOOST_CHECK_CLOSE( result[3].weight, 42.90, 0.00001 );
	BOOST_REQUIRE_EQUAL( result[3].nodelist.size(), 6 );
	BOOST_CHECK_EQUAL( result[3].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[3].nodelist[1], 1 );
	BOOST_CHECK_EQUAL( result[3].nodelist[2], 2 );
	BOOST_CHECK_EQUAL( result[3].nodelist[3], 8 );
	BOOST_CHECK_EQUAL( result[3].nodelist[4], 5 );
	BOOST_CHECK_EQUAL( result[3].nodelist[5], 10 );
	//	Path 4	|	43.88	|	0, 6, 3, 9, 10
	BOOST_CHECK_CLOSE( result[4].weight, 43.88, 0.00001 );
	BOOST_REQUIRE_EQUAL( result[4].nodelist.size(), 5 );
	BOOST_CHECK_EQUAL( result[4].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[4].nodelist[1], 6 );
	BOOST_CHECK_EQUAL( result[4].nodelist[2], 3 );
	BOOST_CHECK_EQUAL( result[4].nodelist[3], 9 );
	BOOST_CHECK_EQUAL( result[4].nodelist[4], 10 );
	//	Path 5	|	45.06	|	0, 1, 2, 3, 9, 10
	BOOST_CHECK_CLOSE( result[5].weight, 45.06, 0.00001 );
	BOOST_REQUIRE_EQUAL( result[5].nodelist.size(), 6 );
	BOOST_CHECK_EQUAL( result[5].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[5].nodelist[1], 1 );
	BOOST_CHECK_EQUAL( result[5].nodelist[2], 2 );
	BOOST_CHECK_EQUAL( result[5].nodelist[3], 3 );
	BOOST_CHECK_EQUAL( result[5].nodelist[4], 9 );
	BOOST_CHECK_EQUAL( result[5].nodelist[5], 10 );
	//	Path 6	|	47.15	|	0, 6, 3, 4, 5, 10
	BOOST_CHECK_CLOSE( result[6].weight, 47.15, 0.00001 );
	BOOST_REQUIRE_EQUAL( result[6].nodelist.size(), 6 );
	BOOST_CHECK_EQUAL( result[6].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[6].nodelist[1], 6 );
	BOOST_CHECK_EQUAL( result[6].nodelist[2], 3 );
	BOOST_CHECK_EQUAL( result[6].nodelist[3], 4 );
	BOOST_CHECK_EQUAL( result[6].nodelist[4], 5 );
	BOOST_CHECK_EQUAL( result[6].nodelist[5], 10 );
	//	Path 7	|	48.33	|	0, 1, 2, 3, 4, 5, 10
	BOOST_CHECK_CLOSE( result[7].weight, 48.33, 0.00001 );
	BOOST_REQUIRE_EQUAL( result[7].nodelist.size(), 7 );
	BOOST_CHECK_EQUAL( result[7].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[7].nodelist[1], 1 );
	BOOST_CHECK_EQUAL( result[7].nodelist[2], 2 );
	BOOST_CHECK_EQUAL( result[7].nodelist[3], 3 );
	BOOST_CHECK_EQUAL( result[7].nodelist[4], 4 );
	BOOST_CHECK_EQUAL( result[7].nodelist[5], 5 );
	BOOST_CHECK_EQUAL( result[7].nodelist[6], 10 );
}


/****************************************************
 *
 *					dag_k_shortest_paths
 *
 ****************************************************/


BOOST_AUTO_TEST_CASE( test_ksp_k_shortest_paths_empty_graph )
{
	WordGraph g = generate_test_graph(GRAPH_EMPTY);

	std::vector<path_type> result;
	std::pair<WordGraph::vertex_iterator, WordGraph::vertex_iterator> nodes = vertices(g);
	dag_k_shortest_paths(g, *nodes.first, *nodes.second, result, 1);

	BOOST_CHECK_EQUAL( result.size(), 0 );
}

BOOST_AUTO_TEST_CASE( test_ksp_k_shortest_paths_single_node )
{
	WordGraph g = generate_test_graph(GRAPH_SINGLE_NODE);

	std::vector<path_type> result;
	dag_k_shortest_paths(g, 0, 0, result, 1);

	BOOST_REQUIRE_EQUAL( result.size(), 1 );
	BOOST_CHECK_CLOSE( result[0].weight, 0., 0.00001 );
	BOOST_REQUIRE_EQUAL( result[0].nodelist.size(), 1 );
	BOOST_CHECK_EQUAL( result[0].nodelist[0], 0 );
}

BOOST_AUTO_TEST_CASE( test_ksp_k_shortest_paths_single_edge )
{
	WordGraph g = generate_test_graph(GRAPH_SINGLE_EDGE);

	std::vector<path_type> result;
	property_map<WordGraph, edge_weight_t>::type
		w_map = get(edge_weight, g);
	dag_k_shortest_paths(g, 0, 1, result, 1);

	BOOST_REQUIRE_EQUAL( result.size(), 1 );
	BOOST_CHECK_CLOSE( result[0].weight, 100., 0.00001 );
	BOOST_REQUIRE_EQUAL( result[0].nodelist.size(), 2 );
	BOOST_CHECK_EQUAL( result[0].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[0].nodelist[1], 1 );
}

BOOST_AUTO_TEST_CASE( test_ksp_k_shortest_paths_single_path )
{
	WordGraph g = generate_test_graph(GRAPH_SINGLE_PATH);

	std::vector<path_type> result;
	property_map<WordGraph, edge_weight_t>::type
		w_map = get(edge_weight, g);
	dag_k_shortest_paths(g, 0, 3, result, 1);

	BOOST_REQUIRE_EQUAL( result.size(), 1 );
	BOOST_CHECK_CLOSE( result[0].weight, 123., 0.00001 );
	BOOST_REQUIRE_EQUAL( result[0].nodelist.size(), 4 );
	BOOST_CHECK_EQUAL( result[0].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[0].nodelist[1], 1 );
	BOOST_CHECK_EQUAL( result[0].nodelist[2], 2 );
	BOOST_CHECK_EQUAL( result[0].nodelist[3], 3 );
}

BOOST_AUTO_TEST_CASE( test_ksp_k_shortest_paths_simple_graph )
{
	WordGraph g = generate_test_graph(GRAPH_SIMPLE);

	std::vector<path_type> result;
	property_map<WordGraph, edge_weight_t>::type
		w_map = get(edge_weight, g);
	dag_k_shortest_paths(g, 0, 3, result, 2);

	BOOST_REQUIRE_EQUAL( result.size(), 2 );
	std::sort(result.begin(), result.end());
	//	Path 0	|	3	|	0, 3
	BOOST_CHECK_CLOSE( result[0].weight, 3., 0.00001 );
	BOOST_REQUIRE_EQUAL( result[0].nodelist.size(), 2 );
	BOOST_CHECK_EQUAL( result[0].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[0].nodelist[1], 3 );
	//	Path 1	|	5	|	0, 1, 3
	BOOST_CHECK_CLOSE( result[1].weight, 5., 0.00001 );
	BOOST_REQUIRE_EQUAL( result[1].nodelist.size(), 3 );
	BOOST_CHECK_EQUAL( result[1].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[1].nodelist[1], 1 );
	BOOST_CHECK_EQUAL( result[1].nodelist[2], 3 );
}

BOOST_AUTO_TEST_CASE( test_ksp_k_shortest_paths_complex_graph )
{
	WordGraph g = generate_test_graph(GRAPH_COMPLEX);

	std::vector<path_type> result;
	property_map<WordGraph, edge_weight_t>::type
		w_map = get(edge_weight, g);
	dag_k_shortest_paths(g, 0, 10, result, 4);

	BOOST_REQUIRE_EQUAL( result.size(), 4 );
	std::sort(result.begin(), result.end());
	//	Path 0	|	29.64	|	0, 1, 7, 4, 5, 10
	BOOST_CHECK_CLOSE( result[0].weight, 29.64, 0.00001 );
	BOOST_REQUIRE_EQUAL( result[0].nodelist.size(), 6 );
	BOOST_CHECK_EQUAL( result[0].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[0].nodelist[1], 1 );
	BOOST_CHECK_EQUAL( result[0].nodelist[2], 7 );
	BOOST_CHECK_EQUAL( result[0].nodelist[3], 4 );
	BOOST_CHECK_EQUAL( result[0].nodelist[4], 5 );
	BOOST_CHECK_EQUAL( result[0].nodelist[5], 10 );
	//	Path 1	|	33.42	|	0, 1, 7, 9, 10
	BOOST_CHECK_CLOSE( result[1].weight, 33.42, 0.00001 );
	BOOST_REQUIRE_EQUAL( result[1].nodelist.size(), 5 );
	BOOST_CHECK_EQUAL( result[1].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[1].nodelist[1], 1 );
	BOOST_CHECK_EQUAL( result[1].nodelist[2], 7 );
	BOOST_CHECK_EQUAL( result[1].nodelist[3], 9 );
	BOOST_CHECK_EQUAL( result[1].nodelist[4], 10 );
	//	Path 2	|	41.72	|	0, 6, 8, 5, 10
	BOOST_CHECK_CLOSE( result[2].weight, 41.72, 0.00001 );
	BOOST_REQUIRE_EQUAL( result[2].nodelist.size(), 5 );
	BOOST_CHECK_EQUAL( result[2].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[2].nodelist[1], 6 );
	BOOST_CHECK_EQUAL( result[2].nodelist[2], 8 );
	BOOST_CHECK_EQUAL( result[2].nodelist[3], 5 );
	BOOST_CHECK_EQUAL( result[2].nodelist[4], 10 );
	//	Path 3	|	42.90	|	0, 1, 2, 8, 5, 10
	BOOST_CHECK_CLOSE( result[3].weight, 42.90, 0.00001 );
	BOOST_REQUIRE_EQUAL( result[3].nodelist.size(), 6 );
	BOOST_CHECK_EQUAL( result[3].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[3].nodelist[1], 1 );
	BOOST_CHECK_EQUAL( result[3].nodelist[2], 2 );
	BOOST_CHECK_EQUAL( result[3].nodelist[3], 8 );
	BOOST_CHECK_EQUAL( result[3].nodelist[4], 5 );
	BOOST_CHECK_EQUAL( result[3].nodelist[5], 10 );
}


/****************************************************
 *
 *					dag_shortest_path
 *
 ****************************************************/


BOOST_AUTO_TEST_CASE( test_ksp_shortest_paths_empty_graph )
{
	WordGraph g = generate_test_graph(GRAPH_EMPTY);

	path_type result;
	dag_shortest_path(g, 0, 0, result);

	BOOST_CHECK_EQUAL( result.nodelist.size(), 0 );
}

BOOST_AUTO_TEST_CASE( test_ksp_shortest_paths_single_node )
{
	WordGraph g = generate_test_graph(GRAPH_SINGLE_NODE);

	path_type result;
	dag_shortest_path(g, 0, 0, result);

	BOOST_CHECK_CLOSE( result.weight, 0., 0.00001 );
	BOOST_REQUIRE_EQUAL( result.nodelist.size(), 1 );
	BOOST_CHECK_EQUAL( result.nodelist[0], 0 );
}

BOOST_AUTO_TEST_CASE( test_ksp_shortest_paths_single_edge )
{
	WordGraph g = generate_test_graph(GRAPH_SINGLE_EDGE);

	path_type result;
	property_map<WordGraph, edge_weight_t>::type
		w_map = get(edge_weight, g);
	dag_shortest_path(g, 0, 1, result);

	BOOST_CHECK_CLOSE( result.weight, 100., 0.00001 );
	BOOST_REQUIRE_EQUAL( result.nodelist.size(), 2 );
	BOOST_CHECK_EQUAL( result.nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result.nodelist[1], 1 );
}

BOOST_AUTO_TEST_CASE( test_ksp_shortest_paths_single_path )
{
	WordGraph g = generate_test_graph(GRAPH_SINGLE_PATH);

	path_type result;
	property_map<WordGraph, edge_weight_t>::type
		w_map = get(edge_weight, g);
	dag_shortest_path(g, 0, 3, result);

	BOOST_CHECK_CLOSE( result.weight, 123., 0.00001 );
	BOOST_REQUIRE_EQUAL( result.nodelist.size(), 4 );
	BOOST_CHECK_EQUAL( result.nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result.nodelist[1], 1 );
	BOOST_CHECK_EQUAL( result.nodelist[2], 2 );
	BOOST_CHECK_EQUAL( result.nodelist[3], 3 );
}

BOOST_AUTO_TEST_CASE( test_ksp_shortest_paths_simple_graph )
{
	WordGraph g = generate_test_graph(GRAPH_SIMPLE);

	path_type result;
	property_map<WordGraph, edge_weight_t>::type
		w_map = get(edge_weight, g);
	dag_shortest_path(g, 0, 3, result);

	//	Path 0	|	3	|	0, 3
	BOOST_CHECK_CLOSE( result.weight, 3., 0.00001 );
	BOOST_REQUIRE_EQUAL( result.nodelist.size(), 2 );
	BOOST_CHECK_EQUAL( result.nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result.nodelist[1], 3 );
}

BOOST_AUTO_TEST_CASE( test_ksp_shortest_paths_complex_graph )
{
	WordGraph g = generate_test_graph(GRAPH_COMPLEX);

	path_type result;
	property_map<WordGraph, edge_weight_t>::type
		w_map = get(edge_weight, g);
	dag_shortest_path(g, 0, 10, result);

	//	Path 0	|	29.64	|	0, 1, 7, 4, 5, 10
	BOOST_CHECK_CLOSE( result.weight, 29.64, 0.00001 );
	BOOST_REQUIRE_EQUAL( result.nodelist.size(), 6 );
	BOOST_CHECK_EQUAL( result.nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result.nodelist[1], 1 );
	BOOST_CHECK_EQUAL( result.nodelist[2], 7 );
	BOOST_CHECK_EQUAL( result.nodelist[3], 4 );
	BOOST_CHECK_EQUAL( result.nodelist[4], 5 );
	BOOST_CHECK_EQUAL( result.nodelist[5], 10 );
}

BOOST_AUTO_TEST_SUITE_END()

#endif //	_OPENCLAS_UNIT_TEST_K_SHORTEST_PATH_HPP_
