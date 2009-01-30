﻿/*********************************************************************************
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
#include "dictionary.h"

namespace openclas {

	/*******************************************************************
	*
	*	DictEntry
	*
	********************************************************************/

	void DictEntry::add(int tag, int weight)
	{
		std::vector<TagEntry>::iterator iter = find(this->tags.begin(), this->tags.end(), TagEntry(tag));
		//	if find the entry, then update it; otherwise add new one
		if (iter != this->tags.end())
			iter->weight = weight;
		else
			this->tags.push_back(TagEntry(tag, weight));
	}

	void DictEntry::remove(int tag)
	{
		std::vector<TagEntry>::iterator iter = find(this->tags.begin(), this->tags.end(), TagEntry(tag));

		if (iter != this->tags.end())
			this->tags.erase(iter);
	}

	double DictEntry::get_forward_weight(const string_type& word) const
	{
		unordered_map<string_type, double>::const_iterator iter = forward.find(word);
		if (iter != forward.end())
		{
			return iter->second;
		}else{
			return 0;
		}
	}

	double DictEntry::get_backward_weight(const string_type& word) const
	{
		unordered_map<string_type, double>::const_iterator iter = backward.find(word);
		if (iter != backward.end())
		{
			return iter->second;
		}else{
			return 0;
		}
	}

	/*******************************************************************
	*
	*	WordIndexerNode
	*
	********************************************************************/

	WordIndexerNode::WordIndexerNode()
		: m_entry_ptr(0)
	{
	}

	WordIndexerNode::~WordIndexerNode()
	{
		for(unordered_map<char_type, WordIndexerNode*>::iterator iter = m_table.begin(); iter != m_table.end(); ++iter)
		{
			if (iter->second)
				delete iter->second;
		}
	}

	void WordIndexerNode::add(string_type::const_iterator& iter, string_type::const_iterator& end, DictEntry* entry_ptr)
	{
		if (iter == end)
		{
			m_entry_ptr = entry_ptr;
			return;
		}

		unordered_map<char_type, WordIndexerNode*>::iterator it = m_table.find(*iter);
		if (it == m_table.end())
		{
			//	not existed in table
			WordIndexerNode* node = new WordIndexerNode();
			m_table[it->first] = node;
			node->add(iter+1, end, entry_ptr);
		}else{
			it->second->add(iter+1, end, entry_ptr);
		}
	}

	DictEntry* WordIndexerNode::get(string_type::const_iterator& iter, string_type::const_iterator& end) const
	{
		if (iter == end)
			return m_entry_ptr;

		unordered_map<char_type, WordIndexerNode*>::const_iterator it = m_table.find(*iter);
		if (it == m_table.end())
		{
			//	not existed in table
			return 0;
		}else{
			return it->second->get(iter+1, end);
		}
	}

	void WordIndexerNode::find_prefixes(string_type::const_iterator& iter, string_type::const_iterator& end, std::list<DictEntry*>& entry_list) const
	{
		if (m_entry_ptr)
			entry_list.push_back(m_entry_ptr);

		if (iter == end)
		{
			return;
		}

		unordered_map<char_type, WordIndexerNode*>::const_iterator it = m_table.find(*iter);
		if (it != m_table.end())
		{
			it->second->find_prefixes(iter+1, end, entry_list);
		}
	}

	/*******************************************************************
	*
	*	WordIndexer
	*
	********************************************************************/

	void WordIndexer::add(const string_type& word, DictEntry* entry_ptr)
	{
		m_top_node.add(word.begin(), word.end(), entry_ptr);
	}

	DictEntry* WordIndexer::get(string_type::const_iterator& iter, string_type::const_iterator& end) const
	{
		return m_top_node.get(iter, end);
	}

	std::list<DictEntry*> WordIndexer::find_prefixes(string_type::const_iterator& iter, string_type::const_iterator& end) const
	{
		std::list<DictEntry*> entry_list;
		m_top_node.find_prefixes(iter, end, entry_list);
		return entry_list;
	}

	/*******************************************************************
	*
	*	Dictionary
	*
	********************************************************************/

	Dictionary::Dictionary()
	{
	}

	Dictionary::~Dictionary()
	{
		for(word_dict_type::iterator iter = m_word_dict.begin(); iter != m_word_dict.end(); ++iter)
		{
			delete (*iter);
		}
	}

	/*****************   Word   *****************/

	DictEntry* Dictionary::add_word(const string_type& word)
	{
		DictEntry* entry_ptr = get_word(word.begin(), word.end());

		if (entry_ptr)
		{
			//	exists, then return the index
			return entry_ptr;
		}else{
			//	not exists, create new one
			DictEntry* ptr = new DictEntry();
			ptr->word = word;
			m_word_dict.push_back(ptr);
			m_word_indexer.add(word, ptr);
			return ptr;
		}
	}

	void Dictionary::remove_word(const string_type& word)
	{
		const DictEntry* entry_ptr = get_word(word.begin(), word.end());
		if (entry_ptr)
		{
			m_word_indexer.remove(word);
			word_dict_type::iterator iter = std::find(m_word_dict.begin(), m_word_dict.end(), entry_ptr);
			if (iter != m_word_dict.end())
				m_word_dict.erase(iter);
			
			delete entry_ptr;	//	release memory
		}
	}

	DictEntry* Dictionary::get_word(string_type::const_iterator& iter, string_type::const_iterator& end)
	{
		return m_word_indexer.get(iter, end);
	}

	const DictEntry* Dictionary::get_word(string_type::const_iterator& iter, string_type::const_iterator& end) const
	{
		return m_word_indexer.get(iter, end);
	}

	std::list<DictEntry*> Dictionary::find_prefixes(string_type::const_iterator &iter, string_type::const_iterator &end) const
	{
		return m_word_indexer.find_prefixes(iter, end);
	}

	/*****************   Tag   *****************/

	void Dictionary::init_tag_dict(int size)
	{
		//	initialize tag table
		m_tag_dict.clear();
		m_tag_dict.resize(size, 0);
		//	initialize tag transit table
		m_tag_transit_dict.clear();
		m_tag_transit_dict.resize(size*size, 0);
	}

	void Dictionary::add_tag_weight(int tag, int weight)
	{
		if (static_cast<int>(m_tag_dict.size()) > tag)
			m_tag_dict[tag] = weight;
	}

	void Dictionary::remove_tag_weight(int tag)
	{
		if (static_cast<int>(m_tag_dict.size()) > tag)
			m_tag_dict[tag] = 0;
	}

	double Dictionary::get_tag_weight(int tag) const
	{
		if (static_cast<int>(m_tag_dict.size()) > tag)
			return m_tag_dict[tag];
		else
			return 0;
	}

	/*****************   Tag transit   *****************/

	int Dictionary::get_tag_transit_index(int current_tag, int next_tag) const
	{
		return static_cast<int>((current_tag * m_tag_dict.size()) + next_tag);
	}

	void Dictionary::add_tag_transit_weight(int current_tag, int next_tag, int weight)
	{
		int index = get_tag_transit_index(current_tag, next_tag);
		m_tag_transit_dict.at(index) = weight;
	}

	void Dictionary::remove_tag_transit_weight(int current_tag, int next_tag)
	{
		int index = get_tag_transit_index(current_tag, next_tag);
		m_tag_transit_dict.at(index) = 0;
	}

	int Dictionary::get_tag_transit_weight(int current_tag, int next_tag) const
	{
		int index = get_tag_transit_index(current_tag, next_tag);
		return m_tag_transit_dict.at(index);
	}
}	//	namespace openclas
