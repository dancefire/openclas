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
			add_edge(1, 2, 100, g);
			add_edge(2, 3, 100, g);
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
	BOOST_CHECK_CLOSE( result[0].weight, 300., 0.00001 );
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
	BOOST_CHECK_CLOSE( result[0].weight, 300., 0.00001 );
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

	BOOST_CHECK_CLOSE( result.weight, 300., 0.00001 );
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
