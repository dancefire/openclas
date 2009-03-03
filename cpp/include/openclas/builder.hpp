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
#ifndef _OPENCLAS_BUILDER_HPP_
#define _OPENCLAS_BUILDER_HPP_

#include "common.hpp"
#include "utility.hpp"
#include "dictionary.hpp"

#include <boost/utility.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <limits>
#include <list>
#include <vector>
#include <stdexcept>	//	for std::logic_error

namespace boost {
	enum vertex_desc_t { vertex_desc };
	BOOST_INSTALL_PROPERTY(vertex, desc);
}

namespace openclas {

	using namespace boost;

	struct WordInformation {
		enum WordTag tag;
		double weight;
		size_t offset;
		size_t length;
		bool is_recorded;
		size_t index;
		const DictEntry* entry;
		WordInformation()
			: tag(WORD_TAG_UNKNOWN), weight(0), offset(0), length(0), is_recorded(false), index(0), entry(0)
		{}
	};

	typedef property<vertex_index_t, size_t,
		property<vertex_distance_t, double, 
		property<vertex_predecessor_t, size_t,
		property<vertex_desc_t, WordInformation> > > > VertexProperty;

	typedef property<edge_weight_t, double> EdgeProperty;

	typedef adjacency_list<vecS, vecS, directedS, VertexProperty, EdgeProperty> WordGraph;

	class Builder{
	public:
		typedef std::map< size_t, std::vector<WordInformation> > out_table_type;
	public:

		static std::vector<WordGraph> create(const wstring& text, const Dictionary& dict)
		{
			std::vector<WordGraph> graph_list;

			//	return empty graph list if the text is empty.
			if (text.empty())
				return graph_list;

			std::vector<WordInformation> atoms;
			split_atoms(text, atoms);

			out_table_type out_table;
			create_out_table(text, dict, atoms, out_table);


			create_graphs(text, dict, out_table, graph_list);
			return graph_list;
		}

	protected:
		///	Input:	text,
		///	Output:	atoms
		static void split_atoms(const wstring& text, std::vector<WordInformation>& atoms)
		{
			//	Add text.
			size_t		index_begin = 0;
			wchar_t		current_symbol = wchar_t();
			enum SymbolType	current_type = SYMBOL_TYPE_UNKNOWN;

			for(size_t i = 0; i < text.size(); ++i)
			{
				wchar_t		previous_symbol = current_symbol;
				enum SymbolType	previous_type = current_type;
				current_symbol = text[i];
				current_type = get_symbol_type(current_symbol);

				if (i != 0) {
					bool pending = false;
					//	Exception cases
					//	case: [\.+-．－＋][0-9]+
					if (current_type == SYMBOL_TYPE_NUMBER && exist(previous_symbol, NUMBER_PREFIXS))
						pending = true;
					//	case: [\d][\d] or [\w][\w]
					if ( (previous_type == current_type) && (previous_type == SYMBOL_TYPE_LETTER || previous_type == SYMBOL_TYPE_NUMBER) )
						pending = true;

					if (!pending) {
						WordInformation word = create_word(previous_type, index_begin, i - index_begin);
						atoms.push_back(word);
						index_begin = i;
					}
				}

				if (i == text.size() - 1)
				{
					WordInformation word = create_word(current_type, index_begin, i - index_begin + 1);
					atoms.push_back(word);
				}
			}
		}


		static void merge_atoms(const wstring& /*text*/, std::vector<WordInformation>& /*atoms*/)
		{
			//	TODO: implement the following:
			//		[may be better adjust it by dict's weight]
			//	get_bias_case_1():	don't split 月份
			//	get_continue_case_1(): 	([0-9０-９]+[年月])/([末内中底前间初])
		}

		///	Input:	text, dict, atoms
		///	Output:	out_table_type
		static void create_out_table(const wstring& text, const Dictionary& dict, const std::vector<WordInformation>& atoms, out_table_type& out_table)
		{
			//	initialize from atoms
			for (size_t i = 0; i < atoms.size(); ++i)
			{
				WordInformation atom = atoms[i];
				atom.index = i;
				out_table[atoms[i].offset].push_back(atom);
			}

			//	initialize from dictionary
			size_t max_index = atoms.size();
			for (size_t i = 0; i < atoms.size(); ++i)
			{
				const WordInformation& atom = atoms[i];
				if (atom.is_recorded)
				{
					//	look up dictionary for prefixes of given sequence.
					std::vector<DictEntry*> entries = dict.prefix(text.begin() + atom.offset, text.end());
					for(std::vector<DictEntry*>::iterator iter = entries.begin(); iter != entries.end(); ++iter)
					{
						//	find the word list of the given offset.
						size_t word_length = (*iter)->word.length();
						out_table_type::iterator it = out_table.find(atom.offset + word_length);
						if (word_length >= atom.length && it != out_table.end())
						{
							//	Construct the word
							WordInformation new_word;
							WordInformation& item = new_word;

							//	replace the reference to the atom, if the atom exists.
							if (word_length == atom.length)
								item = it->second.at(0);	//	The first is the atom.

							//	attach the Dictionary entry
							item.entry = *iter;
							//	calculate the weight
							item.weight = 0;
							//	sum all tags weights as the item's weight
							for(size_t i = 0; i < (*iter)->tags.size(); ++i)
								item.weight += (*iter)->tags[i].weight;

							//	use the tag if the word has the only tag
							if ((*iter)->tags.size() == 1)
								item.tag = static_cast<enum WordTag>((*iter)->tags[0].tag);

							item.is_recorded = true;
							item.offset = atom.offset;
							item.length = word_length;

							if (item.length != atom.length)
							{
								//	add new word to both offset array and wordlist
								item.index = max_index++;
								out_table[atom.offset].push_back(item);
							}
						}
					}
				}else{
					//	not recorded
					std::wstring special_word = get_special_word_string(atom.tag);
					const DictEntry* entry = dict.get_word(special_word);
					if (entry)
					{
						out_table[atom.offset].at(0).entry = entry;
					}else{
						std::ostringstream out;
						out << "Dictionary does not contain the entry for special word \"" << narrow(special_word, locale_utf8) << "\"";
						throw std::logic_error(out.str());
					}
				}
			}

			//	Add the [End] to the last offset
			WordInformation word_end;
			word_end.tag = WORD_TAG_END;
			word_end.index = max_index++;
			word_end.offset = text.size();
			word_end.length = 0;
			word_end.is_recorded = false;
			out_table[word_end.offset].push_back(word_end);
		}

		///	Input:	text, dict, out_table
		///	Output:	graph_list
		static void create_graphs(const wstring& text, const Dictionary& dict, const out_table_type& out_table, std::vector<WordGraph>& graph_list)
		{
			//	candidate graph (out_table) into several sub-graph.
			//	The split point should be the node with multiple out-edges.
			out_table_type::const_iterator prev_iter = out_table.begin();
			size_t max_offset = 0;
			for (out_table_type::const_iterator iter = ++out_table.begin(); iter != out_table.end(); ++iter)
			{
				if (iter->second.size() > 1 && max_offset <= iter->first)
				{
					//	multiple out-edges, so split here
					graph_list.push_back(create_graph(text, dict, out_table, prev_iter, iter));
					prev_iter = iter;
				}

				for (std::vector<WordInformation>::const_iterator it = iter->second.begin(); it != iter->second.end(); ++it)
				{
					if (it->offset > max_offset)
						max_offset = it->offset;
				}
			}
			//	reach the last node
			graph_list.push_back(create_graph(text, dict, out_table, prev_iter, out_table.end()));
		}

		///	Input:	text, dict, out_table, (begin, end)
		///	Output:	graph
		static WordGraph create_graph(const wstring& text, const Dictionary& dict, const out_table_type& out_table, out_table_type::const_iterator begin, out_table_type::const_iterator end)
		{
			//	create graph
			size_t word_count = 0;
			for (out_table_type::const_iterator iter = begin; iter != end; ++iter)
			{
				word_count += iter->second.size();
			}
			WordGraph graph(word_count + 1);	//	Word count + [Begin] + [End]
			//	attach vertex information
			property_map<WordGraph, vertex_desc_t>::type
				vprop_map = get(vertex_desc, graph);

			//	put [begin] to graph as the first node
			size_t index_begin = 0;
			WordInformation word_begin;
			word_begin.tag = WORD_TAG_BEGIN;
			word_begin.index = index_begin;
			vprop_map[index_begin] = word_begin;

			////	put [end] to graph as the last node
			//size_t index_end = num_vertices(graph) - 1;
			//WordInformation word_end;
			//word_end.tag = WORD_TAG_END;
			//word_end.index = index_end;
			//vprop_map[index_end] = word_end;

			for (out_table_type::const_iterator iter = begin; iter != end; ++iter)
			{
				for (std::vector<WordInformation>::const_iterator it = iter->second.begin(); it != iter->second.end(); ++it)
				{
					vprop_map[it->index + 1] = *it;
					vprop_map[it->index + 1].index += 1;
				}
			}

			//	adding edges
			property_map<WordGraph, vertex_index_t>::type
				vindex_map = get(vertex_index, graph);

			for (size_t i = 0; i < num_vertices(graph); ++i)
			{
				WordInformation& prop = vprop_map[i];
				size_t next_offset = prop.offset + prop.length;
				out_table_type::const_iterator iter_out_next = out_table.find(next_offset);
				const std::vector<WordInformation>& next_wordlist = iter_out_next->second;
				//	Get dictionary entry of current word
				const DictEntry* current_entry;
				if (prop.is_recorded)
				{
					//	get normal word
					current_entry = dict.get_word(text.begin() + prop.offset, text.begin() + prop.offset + prop.length);
				}
				else
				{
					//	get special word
					std::wstring special_word = get_special_word_string(prop.tag);
					current_entry = dict.get_word(special_word.begin(), special_word.end());
					if (current_entry == 0) {
						std::ostringstream out;
						out << "Dictionary does not contain the entry for special word \"" << narrow(special_word, locale_utf8) << "\"";
						throw std::logic_error(out.str());
					}
				}
				double current_weight = prop.weight;

				//	add all edges begin from the end of current word
				for (std::vector<WordInformation>::const_iterator iter = next_wordlist.begin(); iter != next_wordlist.end(); ++iter)
				{
					//	get next word or use the special word
					std::wstring next_word;
					if (iter->is_recorded)
						next_word = text.substr(iter->offset, iter->length);
					else
						next_word = get_special_word_string(iter->tag);

					//	get transit weight
					double adjacency_weight = 0;
					if (current_entry)
						adjacency_weight = current_entry->get_forward_weight(next_word);

					double weight = calculate_transit_weight(current_weight, adjacency_weight);
					//	add the edge with weight
					graph_traits<WordGraph>::edge_descriptor edge_desc;
					bool inserted;
					tie(edge_desc, inserted) = add_edge(prop.index, iter->index, weight, graph);
				}
			}

			return graph;
		}

		///	Calculate the possibility
		///	0 < smoothing < 1
		///		A = smoothing * P(Ci-1)
		///		B = (1-smoothing) * P(Ci|Ci-1)
		///		frequency = - Log( A + B );
		static double calculate_transit_weight(double current_weight, double adjacency_weight, double smoothing = 0.1)
		{
			double P1 = (1 + current_weight) / (MAX_FREQUENCE+80000);
			double A = smoothing * P1;

			double t = 1/(double)MAX_FREQUENCE;
			double P2 = (((1-t) * adjacency_weight) / (1+current_weight)) + t;
			double B = (1 - smoothing) * P2;

			double weight = - ::log( A + B );
			return weight;
		}

		static WordInformation create_word(enum SymbolType type, size_t offset, size_t length)
		{
			WordInformation word;
			word.offset = offset;
			word.length = length;
			word.is_recorded = true;
			if (type != SYMBOL_TYPE_CHINESE)
			{
				word.weight = 0;
				switch(type){
	case SYMBOL_TYPE_INDEX:
	case SYMBOL_TYPE_NUMBER:
		//	number
		word.tag = WORD_TAG_M;
		word.is_recorded = false;
		break;
	case SYMBOL_TYPE_LETTER:
	case SYMBOL_TYPE_SINGLE:
		//	nouns of english, or etc.
		word.tag = WORD_TAG_NX;
		word.is_recorded = false;
		break;
	case SYMBOL_TYPE_PUNCTUATION:
		//	punctuation
		word.tag = WORD_TAG_W;
		word.is_recorded = true;
		word.weight = std::numeric_limits<double>::max();
		break;
	default:
		word.tag = WORD_TAG_UNKNOWN;
		break;
				}
			}
			return word;
		}
	};
}
//	_OPENCLAS_BUILDER_HPP_
#endif
