/*********************************************************************************
	Copyright 2007 JYe	(JYe2007@gmail.com)
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


	��Ȩ���� 2007 Dancefire (dancefire@gmail.com)��
	��������Ȩ����

	����������������ǰ���£��������·ַ��޸Ĺ���δ���޸ĵģ���Դ������ѱ�����ʽ
	���ڵı������

	1. ��Դ������ʽ���·������뱣��δ���޸ĵ�������Ȩ����������������Լ�������
	   ��������
	2. ���ѱ�����ʽ���·��������ڷ����汾���ĵ���/������ͬʱ��������������������
	   ��Ȩ����������������Լ���������������

	����������߼������� ������״��ʽ��(AS-IS) �ṩ���ڴ���ʾ���ṩ�κΰ�����������
	�����ض�Ŀ�ĵ������ԡ�ʵ���Ե���ʾ��ʾ�ĵ��������κ�����£����߼������߻���
	�������߽Բ�������ʹ�ô������ɵ�ֱ�ӡ���ӡ��������ر𡢳ͽ�Ļ������ɵĵ�
	��(���������޶��ڻ������Ｐ�����޷�ʹ�á����ݶ�ʧ����ʧӯ���������ж�)��
	�����������κ������ʹ��ʱ�������������ɵġ����ں��������ƶϡ��Ƿ����ں�
	ͬ���롢�ϸ��⳥���λ�������Ȩ��Ϊ(�������������ԭ��)����ʹԤ�ȱ���֪������
	�����Ŀ����ԡ�
	
**********************************************************************************/
/*
*	$Date$
*	$Revision$
*	$Author$
*	$Id$
*/

#pragma once

#include <algorithm>
#include <iostream>

namespace ictclas{

template <typename T, typename G = AdjacencyList<T> >
class KShortestPath{
public:
	//typedef typename std::vector< node_weight_entry<T> > node_property_t;
	//typedef typename std::vector< node_property_t > node_property_list_t;
	typedef typename std::vector<int> path_id_list_t;
	typedef typename std::pair< T, path_id_list_t > result_path_t;
	typedef typename std::vector<result_path_t> result_t;
protected:
	struct pre_node_entry{
		T c_weight;
		int c_PreNode;
		size_t c_PreIndex;
		pre_node_entry(int node, size_t index, const T& weight)
			:	c_PreNode(node), c_PreIndex(index), c_weight(weight){}

			bool operator< (const pre_node_entry& entry2)
			{
				return c_weight<entry2.c_weight;
			}
	};
	typedef typename std::vector< pre_node_entry > node_item_t;
	typedef typename std::vector< node_item_t > node_item_list_t;
	
public:
	//	Constructor
	KShortestPath(G& g) : c_g(g) { c_node_items.resize(g.vertices_number()); }
	//	Find K shortest path
	void Find(const size_t k0, result_t& resPaths)
	{
		//	prepare node item list. We have a DAG that has single direction !!
		//	so scan sequentially all the nodes except the first one, 
		for(unsigned int curnode = 1; curnode < c_node_items.size(); ++curnode)
		{		
			node_item_t entryVect; //hold all the pre_node entries
			node_item_t& nt = c_node_items[curnode];  //current node
			
			G::edge_it_pair it_pair = c_g.in_edges(curnode);

			for(G::edge_iterator it_edge = it_pair.first; it_edge != it_pair.second; ++it_edge)
			{				
				if(it_edge->begin == 0)
				{
					entryVect.push_back(pre_node_entry(0, 0, it_edge->weight)); // edge index is not important for the source node
				}
				else
				{
					node_item_t& nt_pre = c_node_items[it_edge->begin];
					size_t nSize = nt_pre.size();
					for(size_t i =0; i<nSize; i++)
					{
						entryVect.push_back(pre_node_entry(it_edge->begin, i, it_edge->weight + nt_pre[i].c_weight));
					}
				}
			}
			
			size_t k = std::min(k0, entryVect.size());
			partial_sort(entryVect.begin(), entryVect.begin()+k, entryVect.end() ); //only sort the best k entries
			nt.assign(entryVect.begin(), entryVect.begin()+k); //keep the shortest k pre_entry

		}

		GetPaths(resPaths);
	}


protected:
	void GetPaths(result_t& resPaths)
	{
		//	begin from last node, trace backward
		const node_item_t& np_last = c_node_items.back();
		size_t nPath = np_last.size();
		resPaths.resize(nPath); //k paths
		
		int  PreNodeId, PreNodeId2;
		size_t PreEntryIndex;  //the index on the pre-node entry

		for(size_t i=0; i<nPath; i++) //build each path
		{
			resPaths[i].first = np_last[i].c_weight;
			path_id_list_t& path = resPaths[i].second;

			//track backward, pretend we are one node after the last node
			PreNodeId = static_cast<int>(c_node_items.size()-1); // id of the last node 
			PreEntryIndex = i; //the i-th entry on the last node

			while(PreNodeId>0) 
			{
				//path.push_front(PreNodeId);
				path.push_back(PreNodeId);
				PreNodeId2 = c_node_items[PreNodeId][PreEntryIndex].c_PreNode;
				PreEntryIndex = c_node_items[PreNodeId][PreEntryIndex].c_PreIndex;	
				PreNodeId = PreNodeId2;
			} 

			path.push_back(0); //add the first node

			//reverse the path id
			reverse (path.begin(), path.end() );

		}
	}


protected:
	G& c_g;
	node_item_list_t c_node_items; //the item list for the  c_g

};

}