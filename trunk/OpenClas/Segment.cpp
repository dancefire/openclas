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
*	$Date$
*	$Revision$
*	$Author$
*	$Id$
*/
#include "common.h"
#include "Segment.h"

namespace openclas {

	Segment::Segment(const wstring& sentence)
		: m_sentence(sentence)
	{
	}

	void Segment::construct_symbol_graph()
	{
		init_symbol_graph();
		connect_symbol_graph();
	}

	void Segment::init_symbol_graph()
	{
		m_symbol_graph.reset(new SymbolSegmentGraph());

		//	Add Begin Symbol
		m_symbol_graph->add_node(SymbolSegmentGraph::node_value_type(SYMBOL_TYPE_BEGIN));

		//	Add sentence.
		size_t			index_begin = 0;
		char_type		current_symbol = char_type();
		enum SymbolType	current_type = SYMBOL_TYPE_UNKNOWN;

		for(size_t i = 0; i < m_sentence.size(); ++i)
		{
			char_type		previous_symbol = current_symbol;
			enum SymbolType	previous_type = current_type;
			current_symbol = m_sentence[i];
			current_type = get_symbol_type(current_symbol);

			if (i != 0) {
				bool pending = false;
				//	Exception cases
				//	case: [\.+-．－＋][0-9]+
				if (current_type == SYMBOL_TYPE_NUMBER && is_inside(previous_symbol, NUMBER_PREFIXS))
					pending = true;
				//	case: [\d][\d] or [\w][\w]
				if ( (previous_type == current_type) && (previous_type == SYMBOL_TYPE_LETTER || previous_type == SYMBOL_TYPE_NUMBER) )
					pending = true;

				if (!pending) {
					m_symbol_graph->add_node(SymbolSegmentGraph::node_value_type(previous_type, index_begin, i - index_begin));
					index_begin = i;
				}
			}

			if (i == m_sentence.size() - 1)
			{
				m_symbol_graph->add_node(SymbolSegmentGraph::node_value_type(current_type, index_begin, i - index_begin + 1));
			}
		}

		//	Add End Symbol
		m_symbol_graph->add_node(SymbolSegmentGraph::node_value_type(SYMBOL_TYPE_END));
	}

	void Segment::connect_symbol_graph()
	{
	}
}
