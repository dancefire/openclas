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
*	$Rev$
*	$URL$
*/
#include "common.h"
#include "Segment.h"
#include <limits>
//#include <boost/config.hpp>
#include <boost/utility.hpp>
#include <boost/graph/graph_traits.hpp>

namespace openclas {

	Segment::Segment(const wstring& sentence, const Dictionary& dict)
		: m_sentence(sentence), m_dict(dict), m_graph(0), m_wordlist(), m_offset_array(0)
	{
		//	put [begin] to m_wordlist at the very first place
		word_type* word_begin = new word_type();
		word_begin->tag = pku::WORD_TAG_BEGIN;
		m_wordlist.push_back(word_begin);
	}

	Segment::~Segment()
	{
		delete m_graph;
		for (wordlist_type::iterator iter = m_wordlist.begin(); iter != m_wordlist.end(); ++iter)
			delete *iter;
		delete[] m_offset_array;
	}

	void Segment::generate_atoms()
	{
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
					word_type* word = create_word(previous_type, index_begin, i - index_begin);
					m_wordlist.push_back(word);
					index_begin = i;
				}
			}

			if (i == m_sentence.size() - 1)
			{
				word_type* word = create_word(current_type, index_begin, i - index_begin + 1);
				m_wordlist.push_back(word);
			}
		}
	}

	void Segment::merge_atoms()
	{
		//	TODO: implement the following:
		//		[may be better adjust it by dict's weight]
		//	get_bias_case_1():	don't split 月份
		//	get_continue_case_1(): 	([0-9０-９]+[年月])/([末内中底前间初])
	}

	void Segment::generate_offset_array()
	{
		if (m_offset_array)
			delete[] m_offset_array;

		m_offset_array = new offset_array_list_type[m_sentence.size()+1];
		//	initialize from atoms
		for (wordlist_type::iterator iter = m_wordlist.begin(); iter != m_wordlist.end(); ++iter)
			if ((*iter)->tag != pku::WORD_TAG_BEGIN && (*iter)->tag != pku::WORD_TAG_END)
				m_offset_array[(*iter)->offset].push_back(*iter);

		//	initialize from dictionary
		size_t atom_begin = 1;
		size_t atom_end = m_wordlist.size();
		for (size_t i = atom_begin; i < atom_end; ++i)
		{
			word_type* atom = m_wordlist[i];
			std::list<DictEntry*> entries = m_dict.find_prefixes(m_sentence.begin() + atom->offset, m_sentence.end());
			for(std::list<DictEntry*>::iterator iter = entries.begin(); iter != entries.end(); ++iter)
			{
				if ((*iter)->word.length() >= atom->length 
					&& m_offset_array[atom->offset + (*iter)->word.length()].size() > 0)
				{
					//	Construct the word
					word_type* item;

					if ((*iter)->word.length() == atom->length)
						item = atom;
					else
						item = new word_type();

					item->weight = 0;
					//	sum all tags weights as the item's weight
					for(size_t i = 0; i < (*iter)->tags.size(); ++i)
						item->weight += (*iter)->tags[i].weight;

					//	use the tag if the word has the only tag
					if ((*iter)->tags.size() == 1)
						item->tag = static_cast<enum pku::WordTag>((*iter)->tags[0].tag);

					item->is_recorded = true;
					item->offset = atom->offset;
					item->length = (*iter)->word.length();

					if (item->length != atom->offset)
					{
						//	add new word to both offset array and wordlist
						m_offset_array[atom->offset].push_back(item);
						m_wordlist.push_back(item);
					}
				}
			}
		}
		//	add the [end] as the last node
		word_type* word_end = new word_type();
		word_end->tag = pku::WORD_TAG_END;
		m_wordlist.push_back(word_end);
		//	add the [end] to the last list of offset array
		m_offset_array[m_sentence.length()].push_back(word_end);

		//	assign index to each word
		for (size_t i = 0; i < m_wordlist.size(); ++i)
			m_wordlist[i]->index = i;
	}

	void Segment::generate_graph()
	{
		//	all vertices information are stored in m_wordlist,
		//	so, it's not necessary to at more information on vertices.
		m_graph = new WordGraph(m_wordlist.size());
		//	adding edges
		for (size_t i = 0; i < m_wordlist.size()-1; ++i)
		{
			size_t next_offset = m_wordlist[i]->offset + m_wordlist[i]->length;
			offset_array_list_type& next_wordlist = m_offset_array[next_offset];
			//	Get dictionary entry of current word
			const DictEntry* current_entry;
			if (m_wordlist[i]->is_recorded)
			{
				//	get normal word
				current_entry = m_dict.get_word(m_sentence.begin() + m_wordlist[i]->offset, m_sentence.begin() + m_wordlist[i]->offset + m_wordlist[i]->length);
			}
			else
			{
				//	get special word
				string_type special_word = get_special_word_string(m_wordlist[i]->tag);
				current_entry = m_dict.get_word(special_word.begin(), special_word.end());
			}
			double current_weight = m_wordlist[i]->weight;

			//	add all edges begin from the end of current word
			for (offset_array_list_type::iterator iter = next_wordlist.begin(); iter != next_wordlist.end(); ++iter)
			{
				//	get next word or use the special word
				string_type next_word;
				if ((*iter)->is_recorded)
					next_word = m_sentence.substr((*iter)->offset, (*iter)->length);
				else
					next_word = get_special_word_string((*iter)->tag);

				//	get transit weight
				double adjacency_weight = current_entry->get_forward_weight(next_word);
				double weight = calculate_transit_weight(current_weight, adjacency_weight);
				//	add the edge with weight
				graph_traits<WordGraph>::edge_descriptor edge_desc;
				bool inserted;
				tie(edge_desc, inserted) = add_edge(m_wordlist[i]->index, (*iter)->index, weight, *m_graph);
			}
		}
	}

	double Segment::calculate_transit_weight(double current_weight, double adjacency_weight, double smoothing)
	{
		//	Calculate the possibility
		//	0 < smoothing < 1
		//		A = smoothing * P(Ci-1)
		//		B = (1-smoothing) * P(Ci|Ci-1)
		//		frequency = - Log( A + B );
		double P1 = (1 + current_weight) / (MAX_FREQUENCE+80000);
		double A = smoothing * P1;

		double t = 1/(double)MAX_FREQUENCE;
		double P2 = (((1-t) * adjacency_weight) / (1+current_weight)) + t;
		double B = (1 - smoothing) * P2;

		double weight = - log( A + B );
		return weight;
	}


	Segment::word_type* Segment::create_word(enum SymbolType type, size_t offset, size_t length)
	{
		word_type* word = new word_type();
		word->offset = offset;
		word->length = length;
		if (type != SYMBOL_TYPE_CHINESE)
		{
			word->weight = 0;
			switch(type){
			case SYMBOL_TYPE_INDEX:
			case SYMBOL_TYPE_NUMBER:
				//	number
				word->tag = pku::WORD_TAG_M;
				word->is_recorded = false;
				break;
			case SYMBOL_TYPE_LETTER:
			case SYMBOL_TYPE_SINGLE:
				//	nouns of english, or etc.
				word->tag = pku::WORD_TAG_NX;
				word->is_recorded = false;
				break;
			case SYMBOL_TYPE_PUNCTUATION:
				//	punctuation
				word->tag = pku::WORD_TAG_W;
				word->is_recorded = true;
				word->weight = std::numeric_limits<double>::max();
				break;
			default:
				word->tag = pku::WORD_TAG_UNKNOWN;
				break;
			}	
		}
		return word;
	}
}
