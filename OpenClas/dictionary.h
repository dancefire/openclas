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

#include <string>
#include <hash_map>
#include <vector>
#include <utility>

#include "Utility.h"
#include "tag.h"

using namespace std;

namespace ictclas{

	struct WordTag{
		int tag;
		int frequency;
		//	constructor
		WordTag(int t = 0, int f = 0) : tag(t), frequency(f) {}
	};

	struct WordEntry{
		int id;
		wstring word;
		vector<WordTag> tags;
		//	constructor
		WordEntry(const wstring& w = wstring(), vector<WordTag>& ts = vector<WordTag>()) : word(w), tags(ts), id() {}
		//	method
		void Add(WordTag& tag)
		{
			bool found = false;
			for(vector<WordTag>::iterator iter_tag = tags.begin(); iter_tag != tags.end(); ++iter_tag)
			{
				if(iter_tag->tag == tag.tag)
				{
					//	match new tag and the tag in dictionary
					//	update the exist tag's frequency to new value
					if(tag.frequency > 0)
					{
						iter_tag->frequency = tag.frequency;
					}
					//	stop inner loop
					found = true;
					break;
				}
			}
			if (!found)
			{
				//	if the tag is not exist in the entry, then we add it
				tags.push_back(WordTag(tag.tag, tag.frequency));
			}
		}
		void Remove(WordTag& tag)
		{
			for(vector<WordTag>::iterator iter_tag = tags.begin(); iter_tag != tags.end(); ++iter_tag)
			{
				if(iter_tag->tag == tag.tag)
				{
					tags.erase(iter_tag);
					break;
				}
			}
		}
	};

	//	inherit from pair<int,int> to add to size_t conversion function for hash_value() in stdext::hash_map;
	class WordIdPair : public pair<int, int>{
	public:
		WordIdPair(int a, int b) : pair<int, int>(a,b) {}
		operator size_t() const {
			return (first << 15) ^ second;
		}
	};

	class Dictionary{
	public:
		//	for word dict
		typedef stdext::hash_map<wstring, WordEntry> dict_t;
		typedef stdext::hash_map<wstring, WordEntry>::iterator iterator;
		typedef stdext::hash_map<int, wstring> index_table_t;
		//	for adjacency word list
		typedef WordIdPair wordid_pair_t;
		typedef stdext::hash_map<WordIdPair, int> adjacency_dict_t;	//	pair<first_word_id, second_word_id>, frequency
		typedef adjacency_dict_t::iterator adjacency_iterator;

		enum Constants{
			DEFAULT_DICTIONARY_ID = 1,
			WORD_ID_MIN = 100
		};
	protected:
		//	Dictionary Id
		int dict_id;
		//	main dictionary
		dict_t dict;
		//	index table, transfer id to wstring, so we can use dict to find the entry.
		index_table_t id_index_table;
		int max_id;
		//	adjacency dictionary
		adjacency_dict_t adjacency_dict;

	protected:
		bool get_pair_id(const wstring& a, const wstring& b, int& A, int& B)
		{
			//	get a's id
			WordEntry wea = Find(a);
			if (wea.word.length() == 0){
				return false;
			}
			//	get b's id
			WordEntry web = Find(b);
			if (web.word.length() == 0){
				return false;
			}
			//	return A,B
			A = wea.id;
			B = web.id;
			return true;
		}
	public:
		//	constructor
		Dictionary() : dict(), id_index_table(), max_id(WORD_ID_MIN), adjacency_dict(), dict_id(0)
		{
			//	create the empty word entry
			dict[L""];
		}
		//	method
		int NewId()
		{
			return max_id++;
		}
		//	Maintainance the Word Dictionary
		void Add(WordEntry& de)
		{
			//	check the word length
			if (de.word.length() == 0)
			{
				return;
			}
			//	locate the word in dictionary
			dict_t::iterator iter_entry = dict.find(de.word);
			if(iter_entry == dict.end())
			{
				//	if the word is not exist in dict, then we add the word to dictionary
				//	if no id, than allocate a new id
				if(de.id == 0) {
					de.id = NewId();
				}else{
					//	if id is exist, we need adjust max_id
					if( de.id >= max_id )
					{
						max_id = de.id+1;
					}
				}
				dict[de.word] = de;
				id_index_table[de.id] = de.word;
			}else{
				//	if the word is exist, then we check the tags.
				WordEntry& we = iter_entry->second;
				//	check whether the tag of new dict entry is exist in tag list
				for(vector<WordTag>::iterator iter_new_tag = de.tags.begin(); iter_new_tag != de.tags.end(); ++iter_new_tag)
				{
					we.Add(*iter_new_tag);
				}
			}
		}
		void Remove(WordEntry& de)
		{
			//	check the word length
			if (de.word.length() == 0)
			{
				return;
			}
			//	first, we remove all tag attached to de
			//	locate the word in dictionary
			dict_t::iterator iter_entry = dict.find(de.word);
			if(iter_entry == dict.end())
			{
				//	if the word is not exist in dict, then we do nothing
				return;
			}else{
				//	if the word is exist, then we remove the tags.
				WordEntry& we = iter_entry->second;
				//	remove the tags from dict
				for(vector<WordTag>::iterator iter_new_tag = de.tags.begin(); iter_new_tag != de.tags.end(); ++iter_new_tag)
				{
					we.Remove(*iter_new_tag);
				}
			}
			//	second, we remove the 'de' from dict if the there is no tag attached to the word
			if (iter_entry->second.tags.size() == 0)
			{
				dict.erase(iter_entry);
			}
		}
		const WordEntry& Find(const wstring& w) const
		{
			//locale& loc = Encoding::get_locale(Encoding::GBK);
			//cerr << "Try to find the word [" << Encoding::to_string(w, loc) << "]" << endl;
			dict_t::const_iterator iter_entry = dict.find(w);
			if(iter_entry == dict.end())
			{
				//cerr << "\t Not exist." << endl;
				dict_t::const_iterator iter_empty = dict.find(wstring());
				return iter_empty->second;
			}else{
				//cerr << "\t Found the match." << endl;
				return iter_entry->second;
			}
		}
		const WordEntry& Find(int id) const
		{
			index_table_t::const_iterator iter = id_index_table.find(id);
			if(iter == id_index_table.end())
			{
				dict_t::const_iterator iter_empty = dict.find(wstring());
				return iter_empty->second;
			}else{
				return Find(iter->second);
			}
		}
		size_t Size() const
		{
			return dict.size();
		}
		//	Maintainance Adjacency words frequency dictionary
		bool AddAdjacencyFrequency(const wstring& a, const wstring& b, int frequency)
		{
			int A, B;
			if (!get_pair_id(a, b, A, B)){
				//	fail to parse
				return false;
			}else{
				//	add it
				return AddAdjacencyFrequency(A, B, frequency);
			}
		}
		bool AddAdjacencyFrequency(int a, int b, int frequency)
		{
			//	check a and b
			if( Find(a).word.length() == 0 || Find(b).word.length() == 0){
				return false;
			}
			//	 add it
			adjacency_dict[WordIdPair(a,b)] = frequency;
			return true;
		}
		void RemoveAdjacencyFrequency(const wstring& a, const wstring& b)
		{
			int A, B;
			if (!get_pair_id(a, b, A, B)){
				//	fail to parse
				return;
			}else{
				//	remove it
				return RemoveAdjacencyFrequency(A, B);
			}
		}
		void RemoveAdjacencyFrequency(int a, int b)
		{
			adjacency_dict_t::iterator iter = adjacency_dict.find(WordIdPair(a,b));
			if(iter == adjacency_dict.end())
			{
				return;
			}else{
				adjacency_dict.erase(iter);
			}
		}
		int GetAdjacencyFrequency(const wstring& a, const wstring& b)
		{
			int A, B;
			if (!get_pair_id(a, b, A, B)){
				//	fail to parse
				return 0;
			}else{
				//	remove it
				return GetAdjacencyFrequency(A, B);
			}
		}
		int GetAdjacencyFrequency(int a, int b) const
		{
			adjacency_dict_t::const_iterator iter = adjacency_dict.find(WordIdPair(a,b));
			if (iter == adjacency_dict.end())
			{
				//	not found
				return 0;
			}else{
				return iter->second;
			}
		}
		size_t AdjacencySize () const
		{
			return adjacency_dict.size();
		}
		//	Dictionary ID
		int ID() const {	return dict_id;	}
		int ID(int id) {	return (dict_id = id); }
		//	Iterator
		dict_t::iterator begin()
		{
			return dict.begin();
		}
		dict_t::iterator end()
		{
			return dict.end();
		}
		//	adjacency_iterator
		adjacency_iterator adjacency_begin()
		{
			return adjacency_dict.begin();
		}
		adjacency_iterator adjacency_end()
		{
			return adjacency_dict.end();
		}

	};
}

namespace openclas {

	class TagEntry {
	public:
		int tag;
		int weight;
	public:
		TagEntry(int tag = 0, int weight = 0)
			: tag(tag), weight(weight)
		{
		}
		bool operator==(const TagEntry& other)
		{
			return this->tag == other.tag;
		}
	};

	class DictEntry{
	public:
		int id;
		std::wstring word;
		std::vector<TagEntry> tags;
	public:
		void add(int tag, int weight);
		void remove(int tag);
	};

	class WordIndexNode {
	public:
		void add(string_type::const_iterator& iter, string_type::const_iterator& end);
		int get(string_type::const_iterator& iter, string_type::const_iterator& end);
	protected:
		int index;
		unordered_map <char_type, WordIndexNode> m_table;
	};

	class WordIndex {
	public:
		void add(const string_type& word, size_t index);
		int get(string_type::const_iterator& iter, string_type::const_iterator& end);
		std::list<int> find_prefixes(string_type::const_iterator& iter, string_type::const_iterator& end);
	protected:
		WordIndexNode m_top_node;
	};

	class Dictionary {
	public:
		typedef std::vector<DictEntry> word_dict_type;
		typedef std::vector<int> tag_dict_type;
		typedef std::pair<int, int> transit_index_type;
		typedef unordered_map<typename transit_index_type, int> transit_dict_type;
		typedef WordIndex word_indexer_type;
	public:
		//	word
		int add_word(const string_type& word);
		void remove_word(int index);
		int get_word_index(const string_type& word) const;
		DictEntry get_word(int index);
		DictEntry get_word(int index) const;
		std::list<DictEntry> find_prefixes(string_type::const_iterator& iter, string_type::const_iterator& end) const;
		//	word transit
		void add_word_transit_weight(int current_index, int next_index, double weight);
		double get_word_transit_weight(int current_index, int next_index) const;
		void remove_word_transit_weight(int current_index, int next_index);
		//	tag
		void add_tag_weight(int tag, double weight);
		void remove_tag_weight(int tag);
		double get_tag_weight(int tag) const;
		//	tag transit
		void add_tag_transit_weight(int current_tag, int next_tag, double weight);
		void remove_tag_transit_weight(int current_tag, int next_tag);
		double get_tag_transit_weight(int current_tag, int next_tag) const;

	protected:
		//	word
		word_dict_type m_word_dict;
		transit_dict_type m_word_transit_dict;
		//	tag
		tag_dict_type m_tag_dict;
		transit_dict_type m_tag_transit_dict;
		//	indexer
		word_indexer_type m_word_indexer;
	};
}	//	namespace openclas
