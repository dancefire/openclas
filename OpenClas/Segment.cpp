/*********************************************************************************
Copyright 2009 Dancefire (dancefire@gmail.com).
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


版权所有 2009 Dancefire (dancefire@gmail.com)。
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
#include "common.h"
#include "Segment.h"

namespace openclas {

	Segment::Segment(const wstring& sentence, const Dictionary& dict)
		: m_sentence(sentence), m_dict(dict)
	{
	}

	std::vector<Segment::word_type> Segment::get_atom_list()
	{
		std::vector<word_type> atom_list;

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
					word_type word = create_word(previous_type, index_begin, i - index_begin);
					atom_list.push_back(word);
					index_begin = i;
				}
			}

			if (i == m_sentence.size() - 1)
			{
				word_type word = create_word(current_type, index_begin, i - index_begin + 1);
				atom_list.push_back(word);
			}
		}

		return atom_list;
	}

	void Segment::merge_atoms(std::vector<word_type>& atom_list)
	{
		//	TODO: implement the following:
		//		[may be better adjust it by dict's weight]
		//	get_bias_case_1():	don't split 月份
		//	get_continue_case_1(): 	([0-9０-９]+[年月])/([末内中底前间初])
	}

	void Segment::create_graph(const std::vector<word_type>& atom_list)
	{
		//	create look-up table for offset of string => index of the node end with the offset
		unordered_map<int, size_t> offset_index_table;
		for(std::vector<word_type>::const_iterator iter = atom_list.begin(); iter != atom_list.end(); ++iter)
		{
			offset_index_table[static_cast<int>(iter->offset)] = 0;
		}
		//	create list for all not fully connected nodes
		std::list<size_t> uncompleted_nodes;

		//	create new graph
		m_word_graph = shared_ptr<graph_type>(new graph_type);
		//	add the BEGIN and END node
		m_word_graph->add_node(word_type(pku::WORD_TAG_BEGIN));
		size_t end_index = m_word_graph->add_node(word_type(pku::WORD_TAG_END));

		//	Stage 1, connect atoms, and connect nodes with it's previous atom only.
		for(size_t i = 0; i < atom_list.size(); ++i)
		{
			const word_type& atom = atom_list[i];
			std::vector<word_type> word_list = create_word_list_from_dict(atom);
			for(std::vector<word_type>::iterator it = word_list.begin(); it != word_list.end(); ++it)
			{
				//	the end point of the word should be one of the atoms.
				if (offset_index_table.find(static_cast<int>(it->offset + it->length)) != offset_index_table.end()) {
					//	add the word to graph, and fetch the index
					size_t current_index = m_word_graph->add_node(*it);
					//	get previous [atom] node
					size_t previous_index = offset_index_table[static_cast<int>(atom.offset)];
					//	get the word transit weight
					double weight = get_weight(previous_index, previous_index);
					//	add the edge from the current word to previous [atom] (to form a tree)
					m_word_graph->add_edge(previous_index, current_index, word_transit_type(weight));

					if (it->offset + it->length == m_sentence.length())
					{
						//	reach the end of the sentence
						//	connect to the end node
						m_word_graph->add_edge(current_index, end_index, get_weight(current_index, end_index));
						continue;
					}

					if (it->length == atom.length)
					{
						offset_index_table[static_cast<int>(it->offset + it->length)] = current_index;
					}else{
						uncompleted_nodes.push_back(current_index);
					}
				}
			}
		}

		//	Stage 2, complete the connection. From tree to directed acyclic graph

		for(std::list<size_t>::iterator iter = uncompleted_nodes.begin(); iter != uncompleted_nodes.end(); ++iter)
		{
			size_t current_index = *iter;
			const graph_type::node_type& current_node = m_word_graph->nodes().at(current_index);
			//	if the out edges set is empty, then fill in by reference node
			if (current_node.out.empty()) {
				const word_type& current_word = current_node.value;
				//	get the offset of next node, so we can find the previous atom
				int next_offset = static_cast<int>(current_word.offset + current_word.length);
				const graph_type::node_type& previous_atom = m_word_graph->nodes().at(offset_index_table[next_offset]);
				for (size_t i = 0; i < previous_atom.out.size(); ++i)
				{
					size_t next_index = previous_atom.out.at(i)->end;
					double weight = get_weight(current_index, next_index);
					m_word_graph->add_edge(current_index, next_index, word_transit_type(weight));
				}
			}
		}

	}
	Segment::word_type Segment::create_word(enum SymbolType type, size_t offset, size_t length)
	{
		word_type word;
		if (type != SYMBOL_TYPE_CHINESE)
		{
			word = create_word_from_symbol_type(type);
		}
		word.offset = offset;
		word.length = length;
		return word;
	}

	Segment::word_type Segment::create_word_from_symbol_type(openclas::SymbolType type)
	{
		const double MAX = 1000000;

		word_type word;
		word.weight = 0;
		switch(type){
			case SYMBOL_TYPE_INDEX:
			case SYMBOL_TYPE_NUMBER:
				word.tag = pku::WORD_TAG_M;
				word.is_recorded = false;
				break;
			case SYMBOL_TYPE_LETTER:
			case SYMBOL_TYPE_SINGLE:
				word.tag = pku::WORD_TAG_NX;
				word.is_recorded = false;
				break;
			case SYMBOL_TYPE_PUNCTUATION:
				word.tag = pku::WORD_TAG_W;
				word.is_recorded = true;
				word.weight = MAX;
				break;
			default:
				word.tag = pku::WORD_TAG_UNKNOWN;
				break;
		}

		return word;
	}

	std::vector<Segment::word_type> Segment::create_word_list_from_dict(const word_type& word)
	{
		//	get all prefix word based on the given word from dictionary
		//	this function need to guaranteed that the given word must returned in the word_list.
		std::vector<word_type> word_list;

		std::list<DictEntry*> words = m_dict.find_prefixes(m_sentence.begin() + word.offset, m_sentence.end());

		for(std::list<DictEntry*>::iterator iter = words.begin(); iter != words.end(); ++iter)
		{
			if (iter == words.begin() && (*iter)->word.length() > word.length)
			{
				//	the first word is not the given atom, so add the atom manually
				word_list.push_back(word);
			}

			//	Construct the word
			word_type item;
			item.weight = 0;
			//	sum all tags weights as the item's weight
			for(size_t i = 0; i < (*iter)->tags.size(); ++i)
				item.weight += (*iter)->tags[i].weight;

			//	use the tag if the word has the only tag
			if ((*iter)->tags.size() == 1)
				item.tag = static_cast<enum pku::WordTag>((*iter)->tags[0].tag);

			item.is_recorded = true;
			item.offset = word.offset;
			item.length = (*iter)->word.length();
			item.id = (*iter)->id;

			word_list.push_back(item);
		}

		return word_list;
	}

	double Segment::get_weight(size_t current_index, size_t next_index)
	{
		const word_type& current_word = m_word_graph->nodes().at(current_index).value;
		const word_type& next_word = m_word_graph->nodes().at(next_index).value;

		double current_weight = current_word.weight;
		const DictEntry* current_entry = m_dict.get_word(m_sentence.begin() + current_word.offset, m_sentence.begin() + current_word.offset + current_word.length);
		double adj_weight = current_entry->get_forward_weight(m_sentence.substr(next_word.offset, next_word.length));
		
		//	Calculate the possibility
		//	0 < smoothing < 1
		//		A = smoothing * P(Ci-1)
		//		B = (1-smoothing) * P(Ci|Ci-1)
		//		frequency = - Log( A + B );
		double smoothing = 0.1;
		double P1 = (1 + current_weight) / (MAX_FREQUENCE+80000);
		double A = smoothing * P1;

		double t = 1/(double)MAX_FREQUENCE;
		double P2 = (((1-t) * adj_weight) / (1+current_weight)) + t;
		double B = (1 - smoothing) * P2;

		double weight = - log( A + B );

		return weight;
	}
}
