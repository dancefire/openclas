/*********************************************************************************
	Copyright 2007 Dancefire (dancefire@gmail.com).
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


	版权所有 2007 Dancefire (dancefire@gmail.com)。
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

#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <limits>
#include <utility>
#include <iostream>
#include <list>

#include "graph.h"

using namespace std;

namespace ictclas{

	typedef pair<int, int> prenode_t;

	template <typename T>
	struct node_weight_entry{
		T weight;
		vector< prenode_t > props;
		//	constructor
		node_weight_entry(T w) : weight(w) {}

	};

	template <typename T>
	struct prenode_item{
		int node;
		int index;
		T weight;
		//	constructor
		prenode_item(int node, int index, T weight) : node(node), index(index), weight(weight) {}
		//	operator
		bool operator > (const prenode_item<T> & rhs) const
		{
			return weight > rhs.weight;
		}
	};

	template <typename T, typename G = AdjacencyList<T> >
	class KShortestPath{
	public:
		typedef typename vector< node_weight_entry<T> > node_property_t;
		typedef typename vector< node_property_t > node_property_list_t;
		typedef typename list<int> path_id_list_t;
		typedef typename pair< T, path_id_list_t > result_path_t;
		typedef typename vector<result_path_t> result_t;
	protected:
		G& g;
	public:
		//	Constructor
		KShortestPath(G& g) : g(g) {}
		//	Find K shortest path
		void Find(int k, result_t& result)
		{
			//	prepare nodes properties.
			node_property_list_t nodes_props;
			for(int i = 0; i < g.vertices_number(); ++i)
			{
				nodes_props.push_back(node_property_t());
			}

			//	for all nodes except the first one
			for(int curnode = 1; curnode < g.vertices_number(); ++curnode)
			{
				//	priority_queue
				priority_queue< prenode_item<T>, deque<prenode_item<T> >, greater<prenode_item<T> > > pqueue;
				//	get reference of node_properties;
				node_property_t& np = nodes_props.at(curnode-1);
				//	For all income edges of 'curnode'
				pair<G::edge_iterator, G::edge_iterator> ei = g.in_edges(curnode);
				for(G::edge_iterator iter = ei.first; iter != ei.second; ++iter)
				{
					int index = 0;
					if(iter->begin == 0)
					{
						pqueue.push(prenode_item<T>(iter->begin, index, iter->weight));
					}else{
						node_property_t& np_pre = nodes_props.at(iter->begin-1);
						for(node_property_t::iterator iter_props = np_pre.begin(); iter_props != np_pre.end(); ++iter_props, ++index)
						{
							pqueue.push(prenode_item<T>(iter->begin, index, iter->weight + iter_props->weight));
						}
					}
				}

				np.clear();

				//cout << "Priority Queue: [curnode=" << curnode << "]" << endl;
				int index = -1;
				while(!pqueue.empty())
				{
					prenode_item<T> item = pqueue.top();
					pqueue.pop();
					///************** debug output *********/
					//cout << "\t(node=" << item.node << ", index=" << item.index << ", weight=" << item.weight << ")" << endl;
					///************** debug end ************/
					if( np.empty() || np.at(index).weight != item.weight )
					{
						++index;
						//	we only want first K set paths
						if(index >= k)
						{
							break;
						}
						np.push_back(node_weight_entry<T>(item.weight));
					}
					np.at(index).props.push_back(prenode_t(item.node, item.index));
				}
			}
			///**************** debug output ***************/
			//cout << "[nodes properties]" << endl;
			//for(int i = 1; i < g.vertices_number(); ++i)
			//{
			//	node_property_t& np = nodes_props.at(i-1);
			//	cout << "\t[node=" << i << "]" << endl;
			//	for(node_property_t::iterator iter=np.begin(); iter != np.end(); ++iter)
			//	{
			//		cout << "\t\t(weight=" << iter->weight << ")" << endl;
			//		for(vector< prenode_t >::iterator it = iter->props.begin(); it != iter->props.end(); ++it)
			//		{
			//			cout << "\t\t  (node=" << it->first << ", index=" << it->second << ")" << endl;
			//		}
			//	}
			//}
			///**************** debug finish ***************/
			//vector<result_path_t> result;
			GetPaths(result, nodes_props, result_path_t());
			return;
		}
		void GetPaths(result_t& result, const node_property_list_t& nodes_props, result_path_t& path, int node = -1, int index = -1)
		{
			if(node < 0)
			{
				//	begin from last node
				const node_property_t& np_last = nodes_props.at(g.vertices_number()-2);
				for(node_property_t::const_iterator iter=np_last.begin(); iter != np_last.end(); ++iter)
				{
					for(vector<prenode_t>::const_iterator it = iter->props.begin(); it != iter->props.end(); ++it)
					{
						result_path_t p = path;
						p.first = iter->weight;
						//	set the last node and weight
						p.second.push_front(g.vertices_number() - 1);
						p.second.push_front(it->first);
						if (it->first == 0){
							//	reach the first node
							result.push_back(p);
						}else{
							GetPaths(result, nodes_props, p, it->first, it->second);
						}
					}
				}
			}else{
				//	normal case
				node_weight_entry<T> nw = nodes_props.at(node-1).at(index);
				for(vector<prenode_t>::iterator iter=nw.props.begin(); iter != nw.props.end(); ++iter)
				{
					result_path_t p = path;
					//	push current node;
					p.second.push_front(iter->first);
					if( iter->first == 0){
						//	reach the first node
						result.push_back(p);
					}else{
						GetPaths(result, nodes_props, p, iter->first, iter->second);
					}
				}
			}
		}
	};
}