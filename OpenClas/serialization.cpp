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
*	$Id: utility.cpp 13 2009-01-25 01:04:13Z Dancefire $
*/

#include "common.h"
#include "serialization.h"
#include "tag.h"

#include <fstream>

namespace openclas {

	namespace ict {

		enum pku::WordTag get_special_word_tag(const std::wstring& word)
		{
			for (int i = 0 ; i < SPECIAL_WORD_COUNT; ++i)
				if (word == SpecialWordString[i])
					return SpecialWordTagIndex[i];

			//	nothing matched
			return pku::WORD_TAG_UNKNOWN;
		}

		enum pku::WordTag get_tag_from_pos_value(int pos)
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
			std::wstring tag_string(widen(buf, locale_gbk));
			for (int i = 0; i < pku::WORD_TAG_COUNT; ++i)
			{
				if (tag_string == pku::WORD_TAG_NAME[i]){
					return static_cast<enum pku::WordTag>(i);
				}
			}
			//	cannot find the tag
			std::cerr << "Cannot the find the corresponding tag. [" << buf << "]" << std::endl;
			return pku::WORD_TAG_UNKNOWN;
		}

		shared_array<wchar_t> get_gb2312_array()
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

		void load_entries_from_dict(Dictionary& dict, const std::string& filename, bool is_transit)
		{
			std::ifstream in(filename.c_str(), std::ios_base::in | std::ios_base::binary);

			shared_array<wchar_t> GB2312_ARRAY = get_gb2312_array();

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
						word_content.append(buf_ptr.get(), buf_ptr.get() + header.length);
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
								first = pku::get_special_word_string(get_special_word_tag(first));

							if (second.find(L'#') != std::wstring::npos)
								second = pku::get_special_word_string(get_special_word_tag(second));

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
						if (word_content.find(L'#') != std::wstring::npos)
							word_content = pku::get_special_word_string(get_special_word_tag(word_content));

						DictEntry* entry = dict.add_word(word_content);
						if (entry)
							entry->add(get_tag_from_pos_value(header.pos), header.weight);
					}
				}
			}
		}

		void load_words_from_dict(Dictionary& dict, const std::string& filename)
		{
			load_entries_from_dict(dict, filename, false);
		}

		void load_words_transit_from_dict(Dictionary& dict, const std::string& filename)
		{
			load_entries_from_dict(dict, filename, true);
		}

		void load_tags_from_dict(Dictionary& dict, const std::string& filename)
		{
		}

		void save_words_to_dict(Dictionary& dict, const std::string& filename)
		{
		}

		void save_words_transit_to_dict(Dictionary& dict, const std::string& filename)
		{
		}

		void save_tags_to_dict(Dictionary& dict, const std::string& filename)
		{
		}
	}


}