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
#ifndef _OPENCLAS_SEGMENT_H_
#define _OPENCLAS_SEGMENT_H_

#include <boost/graph/adjacency_list.hpp>
#include <utility>
#include <list>
#include <string>
#include "utility.h"
#include "dictionary.h"

namespace openclas {

	using namespace boost;

	struct Word {
		enum pku::WordTag tag;
		double weight;
		size_t offset;
		size_t length;
		bool is_recorded;
		size_t index;
		Word()
			: tag(pku::WORD_TAG_UNKNOWN), weight(0), offset(0), length(0), is_recorded(false), index(0)
		{}
	};

	typedef property<vertex_index_t, size_t,
		property<vertex_distance_t, double, 
		property<vertex_predecessor_t, size_t> > > VertexProperty;
	typedef property<edge_weight_t, double> EdgeProperty;

	typedef adjacency_list<vecS, vecS, directedS, VertexProperty, EdgeProperty> WordGraph;

	class Segment{
	public:
		typedef WordGraph graph_type;
		typedef Word word_type;
		typedef std::vector<Word *> wordlist_type;
		typedef std::list<Word *> offset_array_list_type;
	public:
		Segment(const wstring& sentence, const Dictionary& dict);
		virtual ~Segment();

	protected:
		void generate_atoms();
		void merge_atoms();
		void generate_offset_array();
		void generate_graph();

		double get_weight(size_t current_index, size_t next_index);

		static double calculate_transit_weight(double current_weight, double adjacency_weight, double smoothing = 0.1);
		static word_type* create_word(enum SymbolType type, size_t offset, size_t length);
	protected:
		const wstring m_sentence;
		const Dictionary& m_dict;
		WordGraph* m_graph;
		wordlist_type m_wordlist;
		offset_array_list_type* m_offset_array;
	};
}

#endif	//	_OPENCLAS_SEGMENT_H_