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

#include <map>
#include <string>
#include <cmath>
#include <utility>
#include <list>

#include "graph.h"
#include "utility.h"
#include "dictionary.h"
#include "tag.h"

namespace ictclas{

	using namespace std;

	struct Atom{
		wstring word;
		CharType type;
		//	constructor
		Atom(){}
		Atom(CharType t, const wstring& w = wstring()) : word(w), type(t) {}
	};

	template <typename T>
	class TagEdge : public Edge<T> {
	public:
		int tag;
		wstring word;
		//	constructor
		TagEdge() {}
		TagEdge(int begin_node, int end_node, const T& w = T(), int t = 0, const wstring& word = wstring())
			: Edge(begin_node, end_node, w), tag(t), word(word) {}
	};

	template <typename T>
	class Segment{
	public:
		typedef typename AdjacencyList<T> graph_t;
		typedef typename vector<Atom> atom_list_t;
		typedef typename TagEdge<T> word_t;
		typedef typename AdjacencyList<T, typename word_t> word_graph_t;
		typedef typename std::map<typename std::pair<int,int>, int> word_index_t;	//	map<pair<begin, end>, id>
	public:
		void AtomSegment(const wstring& sentence, atom_list_t& atom_list)
		{
			Atom atom;
			atom_list.push_back(Atom(CT_BEGIN, SPECIAL_STRING[SENTENCE_BEGIN]));
			for(wstring::const_iterator wi = sentence.begin(); wi != sentence.end(); ++wi)
			{
				//	get current char
				wchar_t c = *wi;
				atom.word.push_back(c);
				atom.type = Utility::CharType(c);

				//	get next char if has next.
				wchar_t c_next;
				bool has_next = false;
				CharType ct_next;
				if ((wi+1) != sentence.end())
				{
					has_next = true;
					c_next = *(wi+1);
					ct_next = Utility::CharType(c_next);
				}

				//	dot follow by a number, such as ".1234", so we consider the dot as float point, and it's a part of number.
				//	and "+-" follow by a number, such as "-134", "+234", "234-234+234", we consider "+-" is part of the number.
				if (has_next && ct_next == CT_NUMBER)
				{
					const wstring pre_number(L".-+．－＋");		//	FIXME: we should centralize all L"" string to avoid encoding problem.
					if (pre_number.find(c) != string::npos)
					{
						atom.type = CT_NUMBER;
					}
				}

				//	if atom.type is letter, number or single, and next char type is same, we should consider them as an atom.
				if( has_next ){
					switch(atom.type){
	case CT_LETTER:
	case CT_NUMBER:
	case CT_SINGLE:
		if (atom.type == ct_next)
			continue;	//	continue for-loop
		else
			break;	//	break switch
					}
				}

				//	for all other case, we push the atom into the list.
				atom_list.push_back(atom);
				atom = Atom();
			}
			atom_list.push_back(Atom(CT_END, SPECIAL_STRING[SENTENCE_END]));
		}

		word_t GetAtomEdge(const Atom& atom)
		{
			const T MAX_FREQUENCE = 1000000;	//FIXME: just for compile.
			word_t w;
			if(atom.type == CT_CHINESE)
			{
				w = word_t(0, 0, 0, 0, atom.word);
			}else{
				w.word = atom.word;
				w.weight = MAX_FREQUENCE;
				switch(atom.type)
				{
				case CT_INDEX:
				case CT_NUMBER:
					w.tag = -('m' << 8);
					w.weight = 0;
					w.word = SPECIAL_STRING[UNKNOWN_NUMBER];
					break;
				case CT_DELIMITER:
					w.tag = ('w' << 8);
					break;
				case CT_LETTER:
					w.tag = -(('n' << 8) + 'x');
					w.weight = 0;
					w.word = SPECIAL_STRING[UNKNOWN_STRING];
					break;
				case CT_SINGLE:
					w.tag = -(('n' << 8) + 'x');
					w.weight = 0;
					w.word = SPECIAL_STRING[UNKNOWN_STRING];
					break;
				default:
					w.tag = atom.type;
					break;
				}
			}
			return w;
		}
		int get_bias_case_1(const wstring& current, const wstring& next)
		{
			//	don't split 月份
			if(current == L"月" && next == L"份")
			{
				return 1;
			}else{
				return 0;
			}
		}
		bool get_continue_case_1(const Atom& prev, const wstring& word)
		{
			//	don't merge last 2 [0-9０-９]+[年月][末内中底前间初]
			if (prev.type == CT_NUMBER && word.length() == 2)
			{
				const wstring first(L"年月");
				const wstring second(L"末内中底前间初");
				if( (first.find(word[0]) != string::npos) && (second.find(word[1]) != string::npos))
				{
					return false;
				}
			}
			return true;
		}

		wstring GetAtomWord(const atom_list_t& atom_list, int begin, int end)
		{
			wstring str;
			if(begin < 0 || end >= (int)atom_list.size())
				return str;
			for(int i = begin; i <= end; ++i)
			{
				str += atom_list.at(i).word;
			}
			return str;
		}
		void Connect(const atom_list_t& atom_list, word_graph_t& word_graph, Dictionary& dict)
		{
			for(int i = 0; i < (int)atom_list.size(); ++i)
			{
				const Atom& atom = atom_list[i];

				//	Add the atom to the word graph, this will make all node connectted.
				word_t w_atom = GetAtomEdge(atom);
				w_atom.begin = i;
				w_atom.end = i+1;
				word_graph.add(w_atom);

				//	FIXME: we should centralize special case
				//	we can use this style make a list of functor,
				//	and centralize call the functors.
				int inc_bias = 0;
				if( (i+1) < (int)atom_list.size() )
				{
					inc_bias += get_bias_case_1(atom.word, atom_list.at(i+1).word);
				}
				clock_t tick_all = clock();
				dict_findallmatches_loop_counter++;
				for(int j = i+inc_bias; j < (int)atom_list.size() && j < 3; ++j)
				{
					++dict_findallmatches_wasteloop_counter;
					int total_frequency = 0;
					wstring word = GetAtomWord(atom_list, i, j);
					clock_t tick = clock();
					WordEntry we = dict.Find(word);
					dict_unigram_counter += clock() - tick;
					if (we.word.length() > 0)
					{
						//	found word in dictionary
						//	sum all tag for total frequency
						for(vector<WordTag>::iterator it = we.tags.begin(); it != we.tags.end(); ++it)
						{
							total_frequency += it->frequency;
						}

						//	special case
						//	FIXME: we should centralize special case
						if (i>0)
						{
							//	we can use this style make a list of functor,
							//	and centralize call the functors.
							bool bContinue = true;
							bContinue = get_continue_case_1(atom_list.at(i-1), word);
							if(!bContinue)
							{
								break;	//	break for(j)
							}
						}

						int tag = 0;
						if (we.tags.size() == 1)
						{
							tag = we.tags.at(0).tag;
						}
						word_graph.add(word_t(i,j+1,total_frequency, tag, word));
					}
				}
				--dict_findallmatches_wasteloop_counter;
				dict_findallmatches_counter += clock() - tick_all;
			}
		}
		void GenerateAdjacencyWordGraph(Dictionary& dict, word_graph_t& word_graph, word_index_t& word_index_table, word_graph_t& adjacency_word_graph, T smoothing)
		{
			word_graph.sort();
			for(word_graph_t::edge_iterator iter = word_graph.begin(); iter != word_graph.end(); ++iter)
			{
				//	get the current word frequency
				T current_freq;
				if(iter->tag >= 0){
					current_freq = iter->weight;
				}else{
					//	if the word is Unknown word and classified, such as Person, Number...
					clock_t tick = clock();
					WordEntry we = dict.Find(iter->word);
					dict_unigram_counter += clock() - tick;
					if(we.word.length() > 0 && we.tags.size() > 0){
						current_freq = we.tags.at(0).frequency;
					}else{
						//	cannot find the Unknown word;
						current_freq = 0;
					}
				}

				//	for each edge begin with the end of the current edge.
				const pair<word_graph_t::edge_iterator, word_graph_t::edge_iterator> edge_range = word_graph.out_edges(iter->end);
				for(word_graph_t::edge_iterator it = edge_range.first; it != edge_range.second; ++it)
				{
					//	get adjacency word(transfer) frequency
					clock_t tick = clock();
					int adj_freq = dict.GetAdjacencyFrequency(iter->word, it->word);
					dict_bigram_counter += clock() - tick;


					//	Calculate the possibility
					//	0 < smoothing < 1
					//		A = smoothing * P(Ci-1)
					//		B = (1-smoothing) * P(Ci|Ci-1)
					//		frequency = - Log( A + B );
					double P1 = (1 + current_freq) / (MAX_FREQUENCE+80000);
					double A = smoothing * P1;

					double t = 1/(double)MAX_FREQUENCE;
					double P2 = (((1-t) * adj_freq) / (1+current_freq)) + t;
					double B = (1 - smoothing) * P2;

					double frequency = - log( A + B );

					//	Adjust for Unknown words
					if (iter->tag < 0) {
						frequency += iter->weight;
					}

					//	set adjacency_word_graph
					int word_id_a = word_index_table[pair<int,int>(iter->begin, iter->end)];
					int word_id_b = word_index_table[pair<int,int>(it->begin, it->end)];
					adjacency_word_graph.add(word_t(word_id_a, word_id_b, frequency, iter->tag, iter->word + L"@" + it->word));
				}
			}
		}
		void SegmentWord(const wstring& sentence, Dictionary& dict, atom_list_t& atom_list, graph_t& graph)
		{
			//	get atom list
			AtomSegment(sentence, atom_list);
			//	connect all atom
			edge_list_t edge_list;

			//	generate word connection by dict
			Connect(atom_list, edge_list, dict);
		}
	};
}

#include <boost/graph/adjacency_list.hpp>

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