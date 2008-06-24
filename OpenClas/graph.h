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

#include <vector>
#include <algorithm>

namespace ictclas{

	using namespace std;

	template <typename T>
	class Edge{
	public:
		int id;
		int begin;
		int end;
		T weight;
		//	Constructor
		Edge() {};
		Edge(int begin_node, int end_node, const T& w = T())
			: begin(begin_node), end(end_node), weight(w), id() {}
		Edge(const Edge<T> & e) : begin(e.begin), end(e.end), weight(e.weight), id() {}
		//	operator==
		bool operator==(const Edge<T>& rhs)
		{
			return (
				(this->begin == rhs.begin) && (this->end == rhs.end)	//	position same
				||
				(this->id != 0 && rhs.id != 0 && this->id == rhs.id)	//	id is valid and same
				);
		}
	};

	template <typename T>
	struct less_begin_node
	{
		bool operator() ( const Edge<T> & lhs, const Edge<T> & rhs )
		{
			return (lhs.begin < rhs.begin);
		}
	};

	template <typename T>
	struct less_end_node
	{
		bool operator() ( const Edge<T> & lhs, const Edge<T> & rhs )
		{
			return (lhs.end < rhs.end);
		}
	};

	template <typename T, typename E = Edge<T>>
	class AdjacencyList{
	public:
		typedef typename E edge_t;
		typedef typename vector< edge_t > edge_list_t;
		typedef typename edge_list_t::iterator edge_iterator;
		typedef typename pair<edge_iterator, edge_iterator> edge_it_pair;
	protected:
		edge_list_t edges;
		int number_vertices;
		bool begin_order_dirty;
		bool end_order_dirty;
	public:
		AdjacencyList(int n, const edge_list_t& elist = edge_list_t())
			: edges(elist), number_vertices(n), begin_order_dirty(true), end_order_dirty(true) {}
		int vertices_number() const
		{
			return number_vertices;
		}
		bool add(edge_t& e)
		{
			if(validate_node(e.begin) && validate_node(e.end))
			{
				// the edge changed, so set all dirty bit.
				begin_order_dirty = end_order_dirty = true;
				edge_list_t::iterator result = find(edges.begin(), edges.end(), e);
				if (result != edges.end())
				{
					//	the edge exist in graph
					//	update the data
					*result = e;
				}else{
					//	doesn't exist
					//	then add new one
					edges.push_back(e);
				}
				return true;
			}else{
				return false;
			}
		}
		bool validate_node(int node) const
		{
			if(node >= 0 && node <= number_vertices)
			{
				return true;
			}else{
				return false;
			}
		}
		const edge_it_pair in_edges(int node)
		{
			sort(false);
			//	return the range
			return equal_range(edges.begin(), edges.end(), edge_t(0, node), less_end_node<T>());
		}
		/*
			begin_order = 
				true:	sort by begin node first order. Good for find income edges
				false:	sort by end node first order. Good for find outgoing edges
		*/
		void sort(bool begin_order = false)
		{
			if(begin_order)
			{
				if (begin_order_dirty) {
					//	sort by begin node
					std::sort(edges.begin(), edges.end(), less_begin_node<T>());
					//	reset dirty bits
					begin_order_dirty = false;
					end_order_dirty = true;
				}
			}else{
				if(end_order_dirty)
				{
					//	sort by end node
					std::sort(edges.begin(), edges.end(), less_end_node<T>());
					//	reset dirty bits
					begin_order_dirty = true;
					end_order_dirty = false;
				}
			}
		}
		const edge_it_pair out_edges(int node)
		{
			sort(true);
			//	return the range
			return equal_range(edges.begin(), edges.end(), edge_t(node, 0), less_begin_node<T>());
		}
		edge_iterator begin(){
			return edges.begin();
		}
		edge_iterator end(){
			return edges.end();
		}
		size_t size(){
			return edges.size();
		}
	};

}