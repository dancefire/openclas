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

BOOST_AUTO_TEST_CASE( test_ksp_all_paths_empty_graph )
{
	WordGraph g(0);
	std::vector<path_type> result;

	std::pair<WordGraph::vertex_iterator, WordGraph::vertex_iterator> nodes = vertices(g);
	dag_all_paths(g, *nodes.first, *nodes.second, result);

	BOOST_CHECK_EQUAL( result.size(), 0 );
}

BOOST_AUTO_TEST_CASE( test_ksp_all_paths_single_node )
{
	WordGraph g(1);
	std::vector<path_type> result;

	dag_all_paths(g, 0, 0, result);

	BOOST_REQUIRE_EQUAL( result.size(), 1 );
	BOOST_CHECK_CLOSE( result[0].weight, 0., 0.00001 );
	BOOST_REQUIRE_EQUAL( result[0].nodelist.size(), 1 );
	BOOST_CHECK_EQUAL( result[0].nodelist[0], 0 );
}

BOOST_AUTO_TEST_CASE( test_ksp_all_paths_single_edge )
{
	WordGraph g(2);
	std::vector<path_type> result;
	property_map<WordGraph, edge_weight_t>::type
		w_map = get(edge_weight, g);
	add_edge(0, 1, 100, g);

	dag_all_paths(g, 0, 1, result);

	BOOST_REQUIRE_EQUAL( result.size(), 1 );
	BOOST_CHECK_CLOSE( result[0].weight, 100., 0.00001 );
	BOOST_REQUIRE_EQUAL( result[0].nodelist.size(), 2 );
	BOOST_CHECK_EQUAL( result[0].nodelist[0], 0 );
	BOOST_CHECK_EQUAL( result[0].nodelist[1], 1 );
}

BOOST_AUTO_TEST_CASE( test_ksp_all_paths_single_path )
{
	WordGraph g(4);
	std::vector<path_type> result;
	property_map<WordGraph, edge_weight_t>::type
		w_map = get(edge_weight, g);
	add_edge(0, 1, 100, g);
	add_edge(1, 2, 100, g);
	add_edge(2, 3, 100, g);

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
    BOOST_FAIL( "Test is not ready yet" );
}

BOOST_AUTO_TEST_CASE( test_ksp_all_paths_complex_graph )
{
    BOOST_FAIL( "Test is not ready yet" );
}

BOOST_AUTO_TEST_SUITE_END()

#endif //	_OPENCLAS_UNIT_TEST_K_SHORTEST_PATH_HPP_
