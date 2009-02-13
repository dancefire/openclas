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
#ifndef _OPENCLAS_DICTIONARY_H_
#define _OPENCLAS_DICTIONARY_H_

#include <vector>
#include <list>

using namespace std;

namespace openclas {
	class IndexGenerator {
	public:
		IndexGenerator(int init_value)
			: current_id(init_value)
		{
		}
		int get(){
			return current_id;
		}
		int generate(){
			return (++current_id);
		}
	protected:
		int current_id;
	};

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
		std::wstring word;
		std::vector<TagEntry> tags;
		//	transit table
		unordered_map<string_type, double> backward;
		unordered_map<string_type, double> forward;
	public:
		void add(int tag, int weight);
		void remove(int tag);
		double get_forward_weight(const string_type& word) const;
		double get_backward_weight(const string_type& word) const;
	};

	class WordIndexer {
	public:
		WordIndexer();
		virtual ~WordIndexer();
		void add(string_type::const_iterator& iter, string_type::const_iterator& end, DictEntry* entry_ptr);
		void remove(string_type::const_iterator& iter, string_type::const_iterator& end);
		DictEntry* get(string_type::const_iterator& iter, string_type::const_iterator& end) const;
		//		const DictEntry* get(string_type::const_iterator& iter, string_type::const_iterator& end) const;
		void find_prefixes(string_type::const_iterator& iter, string_type::const_iterator& end, std::list<DictEntry*>& entry_list) const;
	protected:
		DictEntry* m_entry_ptr;
		unordered_map <char_type, WordIndexer*> m_table;
	};

	class Dictionary {
	public:
		typedef std::vector<DictEntry*> word_dict_type;
		typedef std::vector<int> tag_dict_type;
		typedef std::vector<int> tag_transit_dict_type;
		typedef WordIndexer word_indexer_type;
	public:
		Dictionary();
		virtual ~Dictionary();
		//	word
		DictEntry* add_word(const string_type& word);
		void remove_word(const string_type& word);
		DictEntry* get_word(string_type::const_iterator& iter, string_type::const_iterator& end);
		const DictEntry* get_word(string_type::const_iterator& iter, string_type::const_iterator& end) const;
		std::list<DictEntry*> find_prefixes(string_type::const_iterator& iter, string_type::const_iterator& end) const;
		const word_dict_type words() const;
		////	word transit
		//void add_word_transit_weight(int current_index, int next_index, double weight);
		//void remove_word_transit_weight(int current_index, int next_index);
		//double get_word_transit_weight(int current_index, int next_index) const;
		//	tag
		void init_tag_dict(int size);
		void add_tag_weight(int tag, int weight);
		void remove_tag_weight(int tag);
		double get_tag_weight(int tag) const;
		const tag_dict_type tags() const;
		//	tag transit
		int get_tag_transit_index(int current_tag, int next_tag) const;
		void add_tag_transit_weight(int current_tag, int next_tag, int weight);
		void add_tag_transit_weight(int tags_index, int weight);
		void remove_tag_transit_weight(int current_tag, int next_tag);
		int get_tag_transit_weight(int current_tag, int next_tag) const;
		const tag_transit_dict_type tags_transit() const;

	protected:
		//	word
		word_dict_type m_word_dict;
		//transit_dict_type m_word_transit_dict;
		//	tag
		tag_dict_type m_tag_dict;
		tag_transit_dict_type m_tag_transit_dict;
		//	indexer
		word_indexer_type m_word_indexer;
	};
}	//	namespace openclas

#endif	//	_OPENCLAS_DICTIONARY_H_
