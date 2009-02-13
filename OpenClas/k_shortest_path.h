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
#ifndef _OPENCLAS_K_SHORTEST_PATH_H_
#define _OPENCLAS_K_SHORTEST_PATH_H_

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/dag_shortest_paths.hpp>
#include <list>
#include <vector>
#include <algorithm>

namespace openclas {
	struct path_type {
		double weight;
		std::list<size_t> nodelist;
	};

	inline operator < (const path_type& left, const path_type& right)
	{
		return left.weight < right.weight;
	}

	//	Find all paths of given pair of node in a DAG. (DFS-like algorithm)
	template <class IncidenceGraph>
	void dag_all_paths(const IncidenceGraph& g, 
		typename graph_trais<IncidenceGraph>::vertex_descriptor begin, 
		typename graph_trais<IncidenceGraph>::vertex_descriptor end,
		std::list<size_t> current_nodelist,
		std::vector<path_type>& result_paths)
	{
		function_requires<IncidenceGraphConcept<IncidenceGraph> >();

		current_nodelist.push_back(begin);
		if (begin == end) {
			result_paths.push_back(current_nodelist);
		} else {
			typename graph_traits<IncidenceGraph>::out_edge_iterator ei, ei_end;
			for (tie(ei, ei_end) = out_edges(begin, g); ei != ei_end; ++ei) {
				typename graph_traits<IncidenceGraph>::vertex_descriptor v = target(*ei, g);
				dag_all_paths(g, v, end, current_nodelist, result_paths);
			}
		}
	}

	//	Find k-shortest-path in DAG.
	template <class IncidenceGraph>
	void dag_k_shortest_paths(const IncidenceGraph& g, 
		typename graph_trais<IncidenceGraph>::vertex_descriptor begin, 
		typename graph_trais<IncidenceGraph>::vertex_descriptor end,
		std::vector<path_type>& result_paths,
		int k)
	{
		if (k == 1) {
			path_type shortest_path;
			dag_shortest_path(g, begin, end, shortest_path);
			result_paths.push_back(shortest_path);
		}else{
			std::vector<path_type> candidate_paths;
			dag_all_paths(g, begin, end, std::list<size_t>(), candidate_paths);

			k = std::min(k, candidate_paths.size());
			std::partial_sort(candidate_paths.begin(), candidate_paths.begin() + k, candidate_paths.end());
			result_paths.assign(candidate_paths.begin(), candidate_paths.begin() + k);
		}
	}

	//	Find shortest-path in DAG. O(n+m)
	template <class Graph>
	void dag_shortest_path(const Graph& g, 
		typename graph_trais<Graph>::vertex_descriptor begin, 
		typename graph_trais<Graph>::vertex_descriptor end,
		path_type& result_path)
	{
		typename property_map<Graph, vertex_distance_t>::type
			d_map = get(vertex_distance, g);
		typename property_map<Graph, vertex_predecessor_t>::type
			p_map = get(vertex_predecessor, g);
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

#endif	//	_OPENCLAS_K_SHORTEST_PATH_H_
