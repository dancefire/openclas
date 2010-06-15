/*
 * Copyright (c) 2007-2010 Tao Wang <dancefire@gmail.org>
 * See the file "LICENSE.txt" for usage and redistribution license requirements
 *
 *	$Id$
 */
#pragma once
#ifndef _OPENCLAS_K_SHORTEST_PATH_HPP_
#define _OPENCLAS_K_SHORTEST_PATH_HPP_

//	workaround for remove VC min(a,b) macro function definition,
//	which is make std::min() not working.
#if defined(_MSC_VER) && defined(min)
#undef min
#endif	//	min

#include "common.hpp"
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/dag_shortest_paths.hpp>
#include <list>
#include <vector>
#include <algorithm>

namespace openclas {

	using namespace boost;

	struct path_type {
		double weight;
		std::vector<size_t> nodelist;
		path_type()
			: weight(0), nodelist()
		{}
	};

	inline bool operator < (const path_type& left, const path_type& right)
	{
		return left.weight < right.weight;
	}

	//	Find all paths of given pair of node in a DAG. (DFS-like algorithm)
	template <class IncidenceGraph>
	void dag_all_paths(IncidenceGraph& g, 
		typename graph_traits<IncidenceGraph>::vertex_descriptor begin, 
		typename graph_traits<IncidenceGraph>::vertex_descriptor end,
		std::vector<path_type>& result_paths,
		path_type current_path = path_type())
	{
		function_requires<IncidenceGraphConcept<IncidenceGraph> >();

		if (num_vertices(g) == 0)
			return;	//	return if g is empty

		current_path.nodelist.push_back(begin);
		if (begin == end) {
			result_paths.push_back(current_path);
		} else {
			typename graph_traits<IncidenceGraph>::out_edge_iterator ei, ei_end;
			typename property_map<IncidenceGraph, edge_weight_t>::type
				w_map = get(edge_weight, g);
			double original_weight = current_path.weight;
			for (tie(ei, ei_end) = out_edges(begin, g); ei != ei_end; ++ei) {
				typename graph_traits<IncidenceGraph>::vertex_descriptor v = target(*ei, g);
				current_path.weight = original_weight + w_map[edge(begin, v, g).first];
				dag_all_paths(g, v, end, result_paths, current_path);
			}
		}
	}

	//	Find k-shortest-path in DAG.
	template <class IncidenceGraph>
	void dag_k_shortest_paths(IncidenceGraph& g, 
		typename graph_traits<IncidenceGraph>::vertex_descriptor begin, 
		typename graph_traits<IncidenceGraph>::vertex_descriptor end,
		std::vector<path_type>& result_paths,
		int k)
	{
		if (num_vertices(g) == 0)
			return;	//	return if g is empty

		if (k == 1) {
			path_type shortest_path;
			dag_shortest_path(g, begin, end, shortest_path);
			result_paths.push_back(shortest_path);
		}else{
			std::vector<path_type> candidate_paths;
			dag_all_paths(g, begin, end, candidate_paths);

			k = std::min(k, static_cast<int>(candidate_paths.size()));
			std::partial_sort(candidate_paths.begin(), candidate_paths.begin() + k, candidate_paths.end());
			result_paths.assign(candidate_paths.begin(), candidate_paths.begin() + k);
		}
	}

	//	Find shortest-path in DAG. O(n+m)
	template <class Graph>
	void dag_shortest_path(Graph& g, 
		typename graph_traits<Graph>::vertex_descriptor begin, 
		typename graph_traits<Graph>::vertex_descriptor end,
		path_type& result_path)
	{
		if (num_vertices(g) == 0)
			return;	//	return if g is empty

		typename property_map<Graph, vertex_distance_t>::type
			d_map = get(vertex_distance, g);
		typename property_map<Graph, vertex_predecessor_t>::type
			p_map = get(vertex_predecessor, g);

		if (begin == end) {
			result_path.nodelist.push_back(begin);
		} else {
			dag_shortest_paths(g, begin, distance_map(d_map).predecessor_map(p_map));

			if (end == p_map[end])	//	[begin] is not connected with [end]
				return;

			result_path.weight = d_map[end];
			result_path.nodelist.push_back(end);

			size_t current = end;
			while(current != begin)
			{
				current = p_map[current];
				result_path.nodelist.push_back(current);
			}
			std::reverse(result_path.nodelist.begin(), result_path.nodelist.end());
		}
	}
}

//	_OPENCLAS_K_SHORTEST_PATH_HPP_
#endif
