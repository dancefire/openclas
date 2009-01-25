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


	��Ȩ���� 2007-2009 Dancefire (dancefire@gmail.com)��
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

#include <map>
#include <string>
#include <cmath>
#include <utility>

#include "graph.h"
#include "utility.h"
#include "dictionary.h"


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
					const wstring pre_number(L".-+������");		//	FIXME: we should centralize all L"" string to avoid encoding problem.
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
			//	don't split �·�
			if(current == L"��" && next == L"��")
			{
				return 1;
			}else{
				return 0;
			}
		}
		bool get_continue_case_1(const Atom& prev, const wstring& word)
		{
			//	[0-9��-��]+[����][ĩ���е�ǰ���]
			if (prev.type == CT_NUMBER && word.length() == 2)
			{
				const wstring first(L"����");
				const wstring second(L"ĩ���е�ǰ���");
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
								break;
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

namespace openclas {

	typedef int SymbolEdgeValueType;
	class SymbolNodeValueType {
	public:
		enum SymbolType type;
		int offset;
		int length;
	public:
		SymbolNodeValueType()
		{
		}
		SymbolNodeValueType(enum SymbolType type, int offset = 0, int length = 0)
			: type(type), offset(offset), length(length)
		{
		}
	};

	typedef Graph<int, SymbolNodeValueType> SymbolSegmentGraph;

	class Segment{
	public:
		Segment(const wstring& sentence);

		void construct_symbol_graph();
		void init_symbol_graph();
		void connect_symbol_graph();

	protected:
		const wstring m_sentence;
		shared_ptr<SymbolSegmentGraph> m_symbol_graph;
		shared_ptr<SymbolSegmentGraph> m_word_graph;
	};
}