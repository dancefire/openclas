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
*	$Date$
*	$Revision$
*	$Author$
*	$Id$
*/

#pragma once

#include <iostream>
#include <string>

#include "serializer.h"
#include "utility.h"

namespace ictclas{

	class ICT_Serializer : public Serializer{
	protected:
		struct word_info_t{
			int frequency;
			int length;
			int tag;
		};
		struct word_item_t{
			word_info_t info;
			char* word;
		};
		struct word_array_t{
			int count;
			word_item_t* words;
		};
		enum CharacterCount{
			GB2312_COUNT = 6768
		};
	public:
		void Serialize(ostream& stream, Dictionary& dict)
		{
		}
		void Deserialize(istream& stream, Dictionary& dict)
		{
			Deserialize(stream, dict, false);
		}
		void Deserialize(istream& stream, Dictionary& dict, bool bIsAdjacency)
		{
			if (dict.ID() <= 0){
				//	no dict id yet
				dict.ID(Dictionary::DEFAULT_DICTIONARY_ID);
			}

			int read_count = 0;

			locale prev_loc = Encoding::setlocale(Encoding::GBK);

			for(int id = 0; id < GB2312_COUNT; ++id)
			{
				///////////////////////////////TEST///////////////////
				//if( id > 500)
				//	return;
				/////////////////////////////REMOVE///////////////////
				if (id % 10 == 0)
					cerr << ".";
				//	read the word which initial is gb2312(id).
				int count;
				stream.read(reinterpret_cast<char *>(&count), sizeof(int));
				if(stream.fail()){
					cerr << "Read count fail." << read_count << endl;
					return;
				}
				read_count += sizeof(int);

				for(int i = 0; i < count; ++i)
				{
					word_item_t item;
					stream.read(reinterpret_cast<char *>(&(item.info)), sizeof(word_info_t));
					if(stream.fail()){
						cerr << "Read item.info fail. (read_count=" << read_count << ", tellg()=" << stream.tellg() << ")" << endl;
						return;
					}
					read_count += sizeof(word_info_t);
					//	add the word to dict
					wstring word = Encoding::to_wstring(gb2312_id_to_string(id));
					if (item.info.length > 0) {
						item.word = new char[item.info.length+1];
						stream.read(item.word, item.info.length);
						if(stream.fail()){
							cerr << "Read word fail. (" << read_count << ")" << endl;
							return;
						}
						read_count += sizeof(word_info_t);
						item.word[item.info.length] = 0;
						word += Encoding::to_wstring(item.word, item.info.length);
						
						delete item.word;
					}
					WordEntry we(word);
					//	determine the id
					if (word.find(L"#") != wstring::npos)
					{
						//	find # means it's a possible special word
						//	we use GetSpecialStringIndex() to get the index
						int index = Utility::GetSpecialStringIndex(word);
						if (index < 0)
						{
							//	cannot find the special string, treat it as normal word
							//	0 means the id will defined by Dictionary
							we.id = 0;
						}else{
							we.id = index;
						}
					}

					if (bIsAdjacency) {
						//	The word is in format "A@B", we need split them
						size_t pos = word.find(L"@");
						if(pos != wstring::npos)
						{
							wstring first = word.substr(0, pos);
							wstring second = word.substr(pos+1);
							//cerr << "A@B: [" << Encoding::to_string(first, Encoding::GBK) << "] => [" << Encoding::to_string(second, Encoding::GBK) << "] : " << item.info.frequency << endl;
							dict.AddAdjacencyFrequency(first, second, item.info.frequency);
						}else{
							cerr << "Cannot find '@' in Adjacency table" << endl;
						}
					}else{
						we.Add(WordTag(item.info.tag, item.info.frequency));
						dict.Add(we);
					}
				}
			}
			Encoding::setlocale(prev_loc);
		}
	protected:
		void dict_add_word(Dictionary& dict, WordEntry& we, bool& bModified)
		{

		}
		string gb2312_id_to_string(int id)
		{
			string str("  ");
			str[0] = (char)(id/0x5E + 0xB0);
			str[1] = (char)(id%0x5E + 0xA1);
			return str;
		}
	};
}