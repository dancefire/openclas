/*
 * Copyright (c) 2007-2010 Tao Wang <dancefire@gmail.org>
 * See the file "LICENSE.txt" for usage and redistribution license requirements
 *
 *	$Id$
 */

#pragma once
#ifndef _OPENCLAS_DICTIONARY_HPP_
#define _OPENCLAS_DICTIONARY_HPP_

#include "common.hpp"
#include <vector>
#include <list>

using namespace std;

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

	/*******************************************************************
	*
	*	DictEntry
	*
	********************************************************************/

	class DictEntry{
	public:
		typedef std::map<std::wstring, double> transit_type;

		std::wstring word;
		std::vector<TagEntry> tags;
		//	transit table
		transit_type backward;
		transit_type forward;
	public:
		void add(int tag, int weight)
		{
			std::vector<TagEntry>::iterator iter = find(this->tags.begin(), this->tags.end(), TagEntry(tag));
			//	if find the entry, then update it; otherwise add new one
			if (iter != this->tags.end())
				iter->weight = weight;
			else
				this->tags.push_back(TagEntry(tag, weight));
		}

		void remove(int tag)
		{
			std::vector<TagEntry>::iterator iter = find(this->tags.begin(), this->tags.end(), TagEntry(tag));

			if (iter != this->tags.end())
				this->tags.erase(iter);
		}

		double get_forward_weight(const std::wstring& word) const
		{
			transit_type::const_iterator iter = forward.find(word);
			if (iter != forward.end())
			{
				return iter->second;
			}else{
				return 0;
			}
		}

		double get_backward_weight(const std::wstring& word) const
		{
			transit_type::const_iterator iter = backward.find(word);
			if (iter != backward.end())
			{
				return iter->second;
			}else{
				return 0;
			}
		}
	};	//	class DictEntry

	/*******************************************************************
	*
	*	WordIndexer
	*
	********************************************************************/

	class WordIndexer {
	public:
		typedef std::map<wchar_t, WordIndexer*> map_type;
	public:
		WordIndexer()
			: m_entry_ptr(0)
		{
		}

		virtual ~WordIndexer()
		{
			for(map_type::iterator iter = m_table.begin(); iter != m_table.end(); ++iter)
				delete iter->second;
		}

		void add(const std::wstring& word, DictEntry* entry_ptr)
		{
			add(word.begin(), word.end(), entry_ptr);
		}

		void add(std::wstring::const_iterator iter, std::wstring::const_iterator end, DictEntry* entry_ptr)
		{
			if (iter == end)
			{
				m_entry_ptr = entry_ptr;
				return;
			}

			map_type::iterator it = m_table.find(*iter);
			if (it == m_table.end())
			{
				//	not existed in table
				WordIndexer* node = new WordIndexer();
				m_table[*iter] = node;
				node->add(iter+1, end, entry_ptr);
			}else{
				it->second->add(iter+1, end, entry_ptr);
			}
		}

		void remove(const std::wstring& word)
		{
			remove(word.begin(), word.end());
		}

		void remove(std::wstring::const_iterator iter, std::wstring::const_iterator end)
		{
			if (iter == end)
			{
				m_entry_ptr = 0;
				return;
			}

			map_type::iterator it = m_table.find(*iter);
			if (it == m_table.end())
			{
				return;
			}else{
				it->second->remove(iter+1, end);
				//	remove the sub-branch if the branch is empty and current pointer is 0.
				if (it->second->m_entry_ptr == 0 && it->second->m_table.size() == 0)
				{
					delete it->second;
					m_table.erase(it);
				}
			}
		}

		DictEntry* get(const std::wstring& word) const
		{
			return get(word.begin(), word.end());
		}

		DictEntry* get(std::wstring::const_iterator iter, std::wstring::const_iterator end) const
		{
			if (iter == end)
				return m_entry_ptr;

			map_type::const_iterator it = m_table.find(*iter);
			if (it == m_table.end())
			{
				//	not existed in table
				return 0;
			}else{
				return it->second->get(iter+1, end);
			}
		}

		std::vector<DictEntry*> prefix(const std::wstring& word) const
		{
			return prefix(word.begin(), word.end());
		}

		std::vector<DictEntry*> prefix(std::wstring::const_iterator iter, std::wstring::const_iterator end) const
		{
			std::vector<DictEntry*> prefix_list;
			prefix(iter, end, prefix_list);
			return prefix_list;
		}

		void prefix(const std::wstring& word, std::vector<DictEntry*>& entry_list) const
		{
			prefix(word.begin(), word.end(), entry_list);
		}

		void prefix(std::wstring::const_iterator iter, std::wstring::const_iterator end, std::vector<DictEntry*>& entry_list) const
		{
			if (m_entry_ptr)
				entry_list.push_back(m_entry_ptr);

			if (iter == end)
			{
				return;
			}

			map_type::const_iterator it = m_table.find(*iter);
			if (it != m_table.end())
			{
				it->second->prefix(iter+1, end, entry_list);
			}
		}

	protected:
		//	this point is hold for reference purpose, so WordIndexer class should never try to delete this pointer.
		DictEntry* m_entry_ptr;
		map_type m_table;
	};

	/*******************************************************************
	*
	*	Dictionary
	*
	********************************************************************/

	class Dictionary {
	public:
		typedef std::vector<DictEntry*> word_dict_type;
		typedef std::vector<int> tag_dict_type;
		typedef std::vector<int> tag_transit_dict_type;
		typedef WordIndexer word_indexer_type;
	public:
		Dictionary()
		{
		}

		virtual ~Dictionary()
		{
			for(word_dict_type::iterator iter = m_word_dict.begin(); iter != m_word_dict.end(); ++iter)
			{
				delete (*iter);
			}
		}

		/*****************   Word   *****************/
		DictEntry* add_word(const std::wstring& word)
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
				m_word_indexer.add(word.begin(), word.end(), ptr);
				
				if (m_longest_word_length < word.length())
					m_longest_word_length = word.length();

				return ptr;
			}
		}

		void remove_word(const std::wstring& word)
		{
			const DictEntry* entry_ptr = get_word(word.begin(), word.end());
			if (entry_ptr)
			{
				m_word_indexer.remove(word.begin(), word.end());
				word_dict_type::iterator iter = std::find(m_word_dict.begin(), m_word_dict.end(), entry_ptr);
				if (iter != m_word_dict.end())
					m_word_dict.erase(iter);

				delete entry_ptr;	//	release memory
			}
		}

		DictEntry* get_word(const std::wstring& word)
		{
			return get_word(word.begin(), word.end());
		}

		const DictEntry* get_word(const std::wstring& word) const
		{
			return get_word(word.begin(), word.end());
		}

		DictEntry* get_word(std::wstring::const_iterator iter, std::wstring::const_iterator end) const
		{
			return m_word_indexer.get(iter, end);
		}

		std::vector<DictEntry*> prefix(const std::wstring& word) const
		{
			return prefix(word.begin(), word.end());
		}

		std::vector<DictEntry*> prefix(std::wstring::const_iterator iter, std::wstring::const_iterator end) const
		{
			return m_word_indexer.prefix(iter, end);
		}

		size_t longest_word_length()
		{
			return m_longest_word_length;
		}

		const word_dict_type& words() const
		{
			return m_word_dict;
		}

		/*****************   Tag   *****************/
		void init_tag_dict(int size)
		{
			//	initialize tag table
			m_tag_dict.clear();
			m_tag_dict.resize(size, 0);
			//	initialize tag transit table
			m_tag_transit_dict.clear();
			m_tag_transit_dict.resize(size*size, 0);
		}

		int get_tag_total_weight()
		{
			return m_tag_total_weight;
		}

		void set_tag_total_weight(int weight)
		{
			m_tag_total_weight = weight;
		}

		void add_tag_weight(int tag, int weight)
		{
			if (static_cast<int>(m_tag_dict.size()) > tag)
				m_tag_dict[tag] = weight;
		}

		void remove_tag_weight(int tag)
		{
			if (static_cast<int>(m_tag_dict.size()) > tag)
				m_tag_dict[tag] = 0;
		}

		double get_tag_weight(int tag) const
		{
			if (static_cast<int>(m_tag_dict.size()) > tag)
				return m_tag_dict[tag];
			else
				return 0;
		}

		const tag_dict_type& tags() const
		{
			return m_tag_dict;
		}

		/*****************   Tag transit   *****************/
		int get_tag_transit_index(int current_tag, int next_tag) const
		{
			return static_cast<int>((current_tag * m_tag_dict.size()) + next_tag);
		}

		void add_tag_transit_weight(int current_tag, int next_tag, int weight)
		{
			int index = get_tag_transit_index(current_tag, next_tag);
			m_tag_transit_dict.at(index) = weight;
		}

		void add_tag_transit_weight(int tags_index, int weight)
		{
			if (tags_index < static_cast<int>(m_tag_transit_dict.size()))
				m_tag_transit_dict.at(tags_index) = weight;
		}

		void remove_tag_transit_weight(int current_tag, int next_tag)
		{
			int index = get_tag_transit_index(current_tag, next_tag);
			m_tag_transit_dict.at(index) = 0;
		}

		int get_tag_transit_weight(int current_tag, int next_tag) const
		{
			int index = get_tag_transit_index(current_tag, next_tag);
			return m_tag_transit_dict.at(index);
		}

		const tag_transit_dict_type& tags_transit() const
		{
			return m_tag_transit_dict;
		}


	protected:
		//	word
		word_dict_type m_word_dict;
		size_t m_longest_word_length;
		//	tag
		tag_dict_type m_tag_dict;
		tag_transit_dict_type m_tag_transit_dict;
		int m_tag_total_weight;
		//	indexer
		word_indexer_type m_word_indexer;
	};	//	class Dictionary
}	//	namespace openclas

//	_OPENCLAS_DICTIONARY_HPP_
#endif
