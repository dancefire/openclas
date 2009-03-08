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

#pragma once
#ifndef _OPENCLAS_SERIALIZATION_HPP_
#define _OPENCLAS_SERIALIZATION_HPP_

#include "dictionary.hpp"
#include "utility.hpp"
#include <fstream>
#include <iostream>

//#include <boost/iostreams/filtering_stream.hpp>
//#include <boost/iostreams/filter/gzip.hpp>

namespace openclas {

	namespace ict {

		struct WordHeader {
			int weight;
			int length;
			int pos;
		};

		static const enum WordTag SpecialWordTagIndex[] = {
			WORD_TAG_BEGIN,
			WORD_TAG_END,
			WORD_TAG_NX,
			WORD_TAG_NS,
			WORD_TAG_NR,
			WORD_TAG_T,
			WORD_TAG_M,
			WORD_TAG_N,
			WORD_TAG_NT,
			WORD_TAG_NZ
		};

		static const wchar_t* SpecialWordString[] = {
			L"始##始",
			L"末##末",
			L"未##串",
			L"未##地",
			L"未##人",
			L"未##时",
			L"未##数",
			L"未##它",
			L"未##团",
			L"未##专"
		};

		const int SPECIAL_WORD_COUNT = 10;
		const int GB2312_COUNT = 6768;

		static enum WordTag get_special_word_tag(const std::wstring& word)
		{
			for (int i = 0 ; i < SPECIAL_WORD_COUNT; ++i)
				if (word == SpecialWordString[i])
					return SpecialWordTagIndex[i];

			//	nothing matched
			return WORD_TAG_UNKNOWN;
		}

		static std::string get_name_from_pos(int pos)
		{
			if (pos > 'a' * 0x100 * 100){
				std::cerr << "Error in pos to tag convertion. 'cc+d' format found" << std::endl;
			}

			char buf[3];
			if (pos < 0x100) {
				buf[0] = pos;
				buf[1] = 0;
			}else{
				buf[0] = pos / 0x100;
				buf[1] = pos % 0x100;
				buf[2] = 0;
			}
			return std::string(buf);
		}

		static enum WordTag get_tag_from_pos(int pos)
		{
			if (pos == 1 || pos == 100)
				return WORD_TAG_BEGIN;

			if (pos == 4 || pos == 101)
				return WORD_TAG_END;

			std::string pos_name = get_name_from_pos(pos);
			std::wstring tag_string(widen(pos_name, locale_gbk));
			for (size_t i = 0; i < WORD_TAG_COUNT; ++i)
			{
				if (tag_string == WORD_TAG_NAME[i]){
					return static_cast<enum WordTag>(i);
				}
			}
			//	cannot find the tag
			std::cerr << "Cannot the find the corresponding tag. [" << pos_name << "]" << std::endl;
			return WORD_TAG_UNKNOWN;
		}

		static shared_array<wchar_t> get_gb2312_array()
		{
			shared_array<wchar_t> gb2312_array(new wchar_t[GB2312_COUNT]);

			char buf[3];

			for(int i = 0; i < GB2312_COUNT; ++i)
			{
				buf[0] = static_cast<char>(i/0x5E + 0xB0);
				buf[1] = static_cast<char>(i%0x5E + 0xA1);
				buf[2] = 0;

				std::wstring symbol = widen(buf, locale_gbk);
				if (symbol.length() == 1)
					gb2312_array[i] = symbol[0];
				else
					gb2312_array[i] = 0;
			}

			return gb2312_array;
		}

		static void load_from_dct(Dictionary& dict, const char* filename, bool is_transit)
		{
			std::ifstream in(filename, std::ios_base::in | std::ios_base::binary);

			shared_array<wchar_t> GB2312_ARRAY = get_gb2312_array();

#ifdef _DEBUG
			struct unknown_tag_t{
				int pos;
				std::string name;
				std::vector<std::wstring> wordlist;
				unknown_tag_t() : pos(0), name(), wordlist() {}
			};
			std::map<int, unknown_tag_t> data;
#endif
			for (int id = 0; id < GB2312_COUNT; ++id)
			{
				//	read the number of words begin with {id}
				int words_count;
				in.read(reinterpret_cast<char*>(&words_count), sizeof(int));
				if (in.bad())
				{
					std::cerr << "Error raised during reading words_count." << std::endl;
					return;
				}

				for(int i = 0; i < words_count; ++i)
				{
					//	read word header
					WordHeader header;
					in.read(reinterpret_cast<char*>(&header), sizeof(WordHeader));
					if (in.bad())
					{
						std::cerr << "Error raised during reading word_header." << std::endl;
						return;
					}
					//	read the word content
					std::wstring word_content;
					word_content.push_back(GB2312_ARRAY[id]);

					if (header.length > 0) {
						shared_array<char> buf_ptr(new char[header.length + 1]);
						in.read(buf_ptr.get(), header.length);
						if (in.fail()) {
							std::cerr << "Error raised during reading of the word content." << std::endl;
							return;
						}
						std::string gb2312_word(buf_ptr.get(), buf_ptr.get() + header.length);
						word_content.append(widen(gb2312_word, locale_gbk));
					}

					if (is_transit)
					{
						//	split the word content by "@"
						size_t pos = word_content.find(L'@');
						if (pos != std::wstring::npos)
						{
							std::wstring first(word_content.begin(), word_content.begin() + pos);
							std::wstring second(word_content.begin() + pos + 1, word_content.end());

							if (first.find(L'#') != std::wstring::npos)
								first = get_special_word_string(get_special_word_tag(first));

							if (second.find(L'#') != std::wstring::npos)
								second = get_special_word_string(get_special_word_tag(second));

							//	TODO: add new entry to dict if cannot find the entry
							DictEntry* first_entry = dict.get_word(first.begin(), first.end());
							if (first_entry)
								first_entry->forward[second] = header.weight;

							DictEntry* second_entry = dict.get_word(second.begin(), second.end());
							if (second_entry)
								second_entry->backward[first] = header.weight;
						}else{
							std::cerr << "Cannot find '@' in the word content." << std::endl;
						}
					}else{
#ifdef _DEBUG
						std::wstring word_from_dict = word_content;
#endif
						int special_tag = WORD_TAG_UNKNOWN;
						if (word_content.find(L'#') != std::wstring::npos)
						{
							special_tag = get_special_word_tag(word_content);
							word_content = get_special_word_string(static_cast<enum WordTag>(special_tag));
						}

						DictEntry* entry = dict.add_word(word_content);
						if (entry)
						{
							int tag;
							if (special_tag == WORD_TAG_UNKNOWN)
								tag = get_tag_from_pos(header.pos);
							else
								tag = special_tag;

							if (tag != WORD_TAG_UNKNOWN)
							{
								entry->add(tag, header.weight);
							}else{
								//	Found unknown POS
								std::cerr << "Found unknown POS tag(" << tag << ", " << header.weight << ") from dictionary entry [" << narrow(entry->word, locale_gbk) << "]" << std::endl;
#ifdef _DEBUG
								if (data[header.pos].pos == 0)
								{
									data[header.pos].pos = header.pos;
									data[header.pos].name = get_name_from_pos(header.pos);
								}
								data[header.pos].wordlist.push_back(word_from_dict);
#endif
							}
						}
					}
				}
			}

#ifdef _DEBUG
			for(std::map<int, unknown_tag_t>::iterator iter = data.begin(); iter != data.end(); ++iter)
			{
				std::cerr << "POS = " << iter->second.pos << ", name = \"" << iter->second.name << "\", Words = {";
				for (std::vector<std::wstring>::iterator it = iter->second.wordlist.begin(); it != iter->second.wordlist.end(); ++it)
				{
					if (it != iter->second.wordlist.begin())
						std::cerr << ",";
					std::cerr << narrow(*it, locale_gbk);
				}
				std::cerr << "}" << std::endl;
			}
#endif
		}

		static void load_words_from_dct(Dictionary& dict, const char* filename)
		{
			load_from_dct(dict, filename, false);
		}

		static void load_words_transit_from_dct(Dictionary& dict, const char* filename)
		{
			load_from_dct(dict, filename, true);
		}

		static void load_tags_from_ctx(Dictionary& dict, const char* filename)
		{
			std::ifstream in(filename, ios_base::in | ios_base::binary);
			if (in.bad())
				throw std::runtime_error("Cannot open .ctx file.");

			int symbol_count = 0;
			in.read(reinterpret_cast<char*>(&symbol_count), sizeof(int));
			if (in.bad())
				throw std::runtime_error("Cannot read symbol_count.");

			scoped_array<int> symbol_table(new int[symbol_count]);
			scoped_array<int> tag_freq(new int[symbol_count]);
			scoped_array<int> tag_transit_freq(new int[symbol_count * symbol_count]);
			
			int key = 0;
			int total_frequency = 0;

			//	Read the data

			in.read(reinterpret_cast<char*>(symbol_table.get()), sizeof(int) * symbol_count);
			if (in.bad())
				throw std::runtime_error("Cannot read symbol table.");

			in.read(reinterpret_cast<char*>(&key), sizeof(int));
			if (in.bad())
				throw std::runtime_error("Cannot read key.");

			in.read(reinterpret_cast<char*>(&total_frequency), sizeof(int));
			if (in.bad())
				throw std::runtime_error("Cannot read total_frequency.");

			in.read(reinterpret_cast<char*>(tag_freq.get()), sizeof(int) * symbol_count);
			if (in.bad())
				throw std::runtime_error("Cannot read tag_freq.");

			in.read(reinterpret_cast<char*>(tag_transit_freq.get()), sizeof(int) * symbol_count * symbol_count);
			if (in.bad())
				throw std::runtime_error("Cannot read tag_transit_freq.");


			//	Get symbol tag mapping
			scoped_array<int> tags_index(new int[symbol_count]);
			for (int i = 0; i < symbol_count; ++i)
			{
				enum WordTag tag = get_tag_from_pos(symbol_table[i]);
				std::cout << "[" << i << "] " << symbol_table[i] << " : " << narrow(WORD_TAG_NAME[tag], locale_gbk) << std::endl;
				tags_index[i] = tag;
			}

			//	Fill in the dictionary
			dict.init_tag_dict(symbol_count);
			dict.set_tag_total_weight(total_frequency);
			for (int i = 0; i < symbol_count; ++i)
			{
				int tag_i = tags_index[i];
				dict.add_tag_weight(tag_i, tag_freq[i]);
				int offset = i * symbol_count;
				for (int j = 0; j < symbol_count; ++j)
				{
					int tag_j = tags_index[j];
					dict.add_tag_transit_weight(tag_i, tag_j, tag_transit_freq[offset + j]);
				}
			}
		}


		static void load_from_ict_file(Dictionary& dict, const char* tag_filename, const char* words_filename, const char* words_transit_filename)
		{
			load_words_from_dct(dict, words_filename);
			if (words_transit_filename)
				load_words_transit_from_dct(dict, words_transit_filename);
			load_tags_from_ctx(dict, tag_filename);
		}

		static void save_words_to_dct(Dictionary& dict, const char* filename)
		{
		}

		static void save_words_transit_to_dct(Dictionary& dict, const char* filename)
		{
		}

		static void save_tags_to_ctx(Dictionary& dict, const char* filename)
		{
		}

		static void save_to_ict_file(Dictionary& dict, const char* tag_filename, const char* words_filename, const char* words_transit_filename)
		{
			save_words_to_dct(dict, words_filename);
			if (words_transit_filename)
				save_words_transit_to_dct(dict, words_transit_filename);
			save_tags_to_ctx(dict, tag_filename);
		}
	}	//	namespace ict

	const unsigned short DICT_MAGIC_CODE = 'D' << 8 | 'C';

	const char* unigram_type_name = ".unigram";
	const char* bigram_type_name = ".bigram";
	const char* tag_type_name = ".tag";
	const char* text_ext_name = ".txt";
	const char* gzip_ext_name = ".gz";

	struct DictHeader{
		unsigned short magic_code;
		unsigned short tag_count;
		int word_count;
	};

	struct WordHeader {
		unsigned char length;
		unsigned char tag_count;
		unsigned short transit_count;
	};

	struct TagItem{
		int tag;
		int weight;
	};

	struct TransitHeader {
		int length;
		int weight;
	};

	static void save_to_ocd_file(const Dictionary& dict, const char* filename)
	{
		std::ofstream out(filename, std::ios_base::out | std::ios_base::binary);

		//	Write Dictionary Header
		DictHeader header;
		header.magic_code = DICT_MAGIC_CODE;
		header.tag_count = static_cast<unsigned short>(dict.tags().size());
		header.word_count = static_cast<int>(dict.words().size());
		out.write(reinterpret_cast<const char *>(&header), sizeof(DictHeader));

		//	Write all tags
		//		tag
		scoped_array<int> tags(new int[header.tag_count]);
		std::copy(dict.tags().begin(), dict.tags().end(), tags.get());
		out.write(reinterpret_cast<const char*>(tags.get()), static_cast<int>(sizeof(int) * dict.tags().size()));
		//		tag transit
		scoped_array<int> tags_transit(new int[dict.tags_transit().size()]);
		std::copy(dict.tags_transit().begin(), dict.tags_transit().end(), tags_transit.get());

		out.write(reinterpret_cast<const char*>(tags_transit.get()), static_cast<int>(sizeof(int) * dict.tags_transit().size()));
		//	Write all words
		for (Dictionary::word_dict_type::const_iterator iter = dict.words().begin(); iter != dict.words().end(); ++iter)
		{
			std::string narrow_word = narrow((*iter)->word, locale_utf8);

			WordHeader word_header;
			word_header.length = static_cast<unsigned char>(narrow_word.length());
			word_header.tag_count = static_cast<unsigned char>((*iter)->tags.size());
			word_header.transit_count = static_cast<unsigned short>((*iter)->forward.size());
			//	Word Header
			out.write(reinterpret_cast<const char*>(&word_header), sizeof(WordHeader));
			//	Word content
			out.write(reinterpret_cast<const char*>(narrow_word.c_str()), static_cast<int>(narrow_word.length()));
			//	Word Tags
			for (std::vector<TagEntry>::iterator it = (*iter)->tags.begin(); it != (*iter)->tags.end(); ++it)
			{
				TagItem tag;
				tag.tag = it->tag;
				tag.weight = it->weight;
				out.write(reinterpret_cast<const char*>(&tag), sizeof(TagItem));
			}
			//	Word Transit
			for (DictEntry::transit_type::iterator it = (*iter)->forward.begin(); it != (*iter)->forward.end(); ++it)
			{
				std::string narrow_transit_word = narrow(it->first, locale_utf8);
				TransitHeader transit_header;
				transit_header.length = static_cast<int>(narrow_transit_word.length());
				transit_header.weight = static_cast<int>(it->second);
				out.write(reinterpret_cast<const char*>(&transit_header), sizeof(TransitHeader));
				out.write(narrow_transit_word.c_str(), static_cast<std::streamsize>(narrow_transit_word.length()));
			}
		}
	}

	static void load_from_ocd_file(Dictionary& dict, const char* filename)
	{
		std::ifstream in(filename, std::ios_base::in | std::ios_base::binary);

		//	Read Dictionary Header
		DictHeader header;
		in.read(reinterpret_cast<char*>(&header), sizeof(DictHeader));
		if (header.magic_code != DICT_MAGIC_CODE)
			return;

		//	Read all tags
		dict.init_tag_dict(header.tag_count);
		//		tag
		scoped_array<int> tags(new int[header.tag_count]);
		in.read(reinterpret_cast<char*>(tags.get()), header.tag_count * sizeof(int) );
		for (int i = 0; i < header.tag_count; ++i)
		{
			dict.add_tag_weight(i, tags[i]);
		}
		//		tag transit
		int tag_transit_count = header.tag_count * header.tag_count;
		scoped_array<int> tags_transit(new int[tag_transit_count]);
		in.read(reinterpret_cast<char*>(tags_transit.get()), static_cast<int>(tag_transit_count) * sizeof(int) );
		for (int i = 0; i < tag_transit_count; ++i)
		{
			dict.add_tag_transit_weight(i, tags_transit[i]);
		}
		//	Read all words
		for (int i = 0; i < header.word_count; ++i)
		{
			//	Word Header
			WordHeader word_header;
			in.read(reinterpret_cast<char*>(&word_header), sizeof(WordHeader));
			//	Word content
			scoped_array<char> word_ptr(new char[word_header.length]);
			in.read(reinterpret_cast<char*>(word_ptr.get()), word_header.length);
			std::wstring word_content = widen(std::string(word_ptr.get(), word_ptr.get() + word_header.length), locale_utf8);
			DictEntry* entry = dict.add_word(word_content);
			//	Word Tags
			for (int i = 0; i < word_header.tag_count; ++i)
			{
				TagItem tag;
				in.read(reinterpret_cast<char*>(&tag), sizeof(TagItem));
				entry->add(tag.tag, tag.weight);
				//dict.add_tag_weight(tag.tag, tag.weight);
			}
			//	Word Transit
			for (int i = 0; i < word_header.transit_count; ++i)
			{
				TransitHeader transit_header;
				in.read(reinterpret_cast<char*>(&transit_header), sizeof(TransitHeader));
				scoped_array<char> transit_word_ptr(new char[transit_header.length]);
				in.read(transit_word_ptr.get(), transit_header.length);
				std::string narrow_word(transit_word_ptr.get(), transit_word_ptr.get() + transit_header.length);
				std::wstring transit_word = widen(narrow_word, locale_utf8);
				entry->forward[transit_word] = transit_header.weight;
			}
		}
	}

	static void save_to_txt_stream(const Dictionary& dict, std::wostream& tag_out, std::wostream& unigram_out, std::wostream& bigram_out, bool save_bigram = true)
	{
		//	write tag
		{
			//	tag count
			size_t tag_count = dict.tags().size();
			tag_out << tag_count << std::endl;
			//	unigram tag weight

			for (size_t i = 0; i < tag_count; ++i)
			{
				if (i != 0)
					tag_out << " ";
				tag_out << static_cast<int>(dict.get_tag_weight(i));
			}
			tag_out << std::endl;
			//	bigram tag weight
			for (size_t i = 0; i < tag_count; ++i)
			{
				for (size_t j = 0; j < tag_count; ++j)
				{
					if (j != 0)
						tag_out << " ";
					tag_out << static_cast<int>(dict.get_tag_transit_weight(i, j));
				}
				tag_out << std::endl;
			}
		}
		//	write unigram & bigram
		{
			size_t word_count = dict.words().size();
			for (size_t i = 0; i < word_count; ++i)
			{
				DictEntry* entry = dict.words().at(i);
				for (std::vector<TagEntry>::iterator iter = entry->tags.begin(); iter != entry->tags.end(); ++iter)
				{
					unigram_out << entry->word << " " << iter->tag << " " << iter->weight << std::endl;
				}
				if (save_bigram)
				{
					for (DictEntry::transit_type::iterator iter = entry->forward.begin(); iter != entry->forward.end(); ++iter)
					{
						bigram_out << entry->word << " " << iter->first << " " << static_cast<int>(iter->second) << std::endl;
					}
				}
			}
		}
	}

	static void load_from_txt_stream(Dictionary& dict, std::wistream& tag_in, std::wistream& unigram_in, std::wistream& bigram_in, bool load_bigram = true)
	{
		//	load tag
		{
			//	tag count
			size_t tag_count;
			tag_in >> tag_count;
			dict.init_tag_dict(tag_count);

			//	unigram tag weight
			for (size_t i = 0; i < tag_count; ++i)
			{
				int weight;
				tag_in >> weight;
				dict.add_tag_weight(i, weight);
			}
			//	bigram tag weight
			for (size_t i = 0; i < tag_count; ++i)
			{
				for (size_t j = 0; j < tag_count; ++j)
				{
					int weight;
					tag_in >> weight;
					dict.add_tag_transit_weight(i, j, weight);
				}
			}
		}
		//	load unigram & bigram
		{
			while (unigram_in.good())
			{
				std::wstring word;
				int tag;
				int weight;

				unigram_in >> word;
				unigram_in >> tag;
				unigram_in >> weight;

				DictEntry* entry = dict.add_word(word);
				entry->add(tag, weight);
			}
		}
		if (load_bigram) {
			while (bigram_in.good())
			{
				std::wstring word1;
				std::wstring word2;
				int weight;

				bigram_in >> word1;
				bigram_in >> word2;
				bigram_in >> weight;

				DictEntry* entry = dict.add_word(word1);
				entry->forward[word2] = weight;
			}
		}
	}

	static void save_to_txt_file(const Dictionary& dict, const char* base_name, bool save_bigram = true)
	{
		std::wofstream tag_out;
		{
			std::ostringstream oss;
			oss << base_name << tag_type_name << text_ext_name;
			tag_out.open(oss.str().c_str(), ios::out);
			tag_out.imbue(locale_utf8);
		}
		std::wofstream unigram_out;
		{
			std::ostringstream oss;
			oss << base_name << unigram_type_name << text_ext_name;
			unigram_out.open(oss.str().c_str(), ios::out);
			unigram_out.imbue(locale_utf8);
		}
		std::wofstream bigram_out;
		if (save_bigram) {
			std::ostringstream oss;
			oss << base_name << bigram_type_name << text_ext_name;
			bigram_out.open(oss.str().c_str(), ios::out);
			bigram_out.imbue(locale_utf8);
		}

		save_to_txt_stream(dict, tag_out, unigram_out, bigram_out, save_bigram);
	}

	static void load_from_txt_file(Dictionary& dict, const char* base_name, bool load_bigram = true)
	{
		std::wifstream tag_in;
		{
			std::ostringstream oss;
			oss << base_name << tag_type_name << text_ext_name;
			tag_in.open(oss.str().c_str(), ios::in);
			tag_in.imbue(locale_utf8);
		}
		std::wifstream unigram_in;
		{
			std::ostringstream oss;
			oss << base_name << unigram_type_name << text_ext_name;
			unigram_in.open(oss.str().c_str(), ios::in);
			unigram_in.imbue(locale_utf8);
		}
		std::wifstream bigram_in;
		if (load_bigram) {
			std::ostringstream oss;
			oss << base_name << bigram_type_name << text_ext_name;
			bigram_in.open(oss.str().c_str(), ios::in);
			bigram_in.imbue(locale_utf8);
		}

		load_from_txt_stream (dict, tag_in, unigram_in, bigram_in, load_bigram);
	}

	//static void save_to_gz_file(const Dictionary& dict, std::string base_name, bool save_bigram = true)
	//{
	//	using namespace boost::iostreams;

	//	std::string tag_filename;
	//	std::string unigram_filename;
	//	std::string bigram_filename;
	//	{
	//		std::ostringstream oss;
	//		oss << base_name << tag_type_name << text_ext_name;
	//		tag_filename = oss.str();
	//		oss.clear();
	//		oss << base_name << unigram_type_name << text_ext_name;
	//		unigram_filename = oss.str();
	//		oss << base_name << bigram_type_name << text_ext_name;
	//		bigram_filename = oss.str();
	//	}

	//	std::ofstream tag_file(tag_filename.c_str(), ios::out | ios::binary);
	//	filtering_ostream tag_out;
	//	//tag_out.imbue(locale_utf8);
	//	tag_out.push(gzip_compressor());
	//	tag_out.push(tag_file);

	//	std::wofstream unigram_file(unigram_filename.c_str(), ios::out | ios::binary);
	//	filtering_wostream unigram_out;
	//	unigram_out.imbue(locale_utf8);
	//	unigram_out.push(gzip_compressor());
	//	unigram_out.push(unigram_file);
	//
	//	std::wofstream bigram_file(bigram_filename.c_str(), ios::out | ios::binary);
	//	filtering_wostream bigram_out;
	//	bigram_out.imbue(locale_utf8);
	//	bigram_out.push(gzip_compressor());
	//	bigram_out.push(bigram_file);
	//
	//	save_to_txt_stream(dict, tag_out, unigram_out, bigram_out, save_bigram);

	//}

	//static void load_from_gz_file(Dictionary& dict, const char* base_name, bool load_bigram = true)
	//{
	//	using namespace boost::iostreams;

	//	std::wifstream tag_in;
	//	{
	//		std::ostringstream oss;
	//		oss << base_name << tag_type_name << text_ext_name;
	//		tag_in.open(oss.str().c_str(), ios::in);
	//		tag_in.imbue(locale_utf8);
	//	}
	//	std::wifstream unigram_in;
	//	{
	//		std::ostringstream oss;
	//		oss << base_name << unigram_type_name << text_ext_name;
	//		unigram_in.open(oss.str().c_str(), ios::in);
	//		unigram_in.imbue(locale_utf8);
	//	}
	//	std::wifstream bigram_in;
	//	if (load_bigram) {
	//		std::ostringstream oss;
	//		oss << base_name << bigram_type_name << text_ext_name;
	//		bigram_in.open(oss.str().c_str(), ios::in);
	//		bigram_in.imbue(locale_utf8);
	//	}

	//	load_from_txt_stream (dict, tag_in, unigram_in, bigram_in, load_bigram);
	//}

}	//	namespace openclas

//	_OPENCLAS_SERIALIZATION_HPP_
#endif
