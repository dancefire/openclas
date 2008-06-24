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

namespace ictclas{

	class OpenClasSerializer : public Serializer{
	protected:
		struct dict_header{
			int dict_id;
			int word_count;
			int adjacency_word_count;
		};
		struct word_item_header{
			int id;
			int length;
			int tag_count;
		};
		struct tag_item{
			int tag;
			int frequency;
		};
		struct adjacency_word_item{
			int word1_id;
			int word2_id;
			int frequency;
		};
	public:
		void Serialize(ostream& stream, Dictionary& dict)
		{
			//	Write Dictionary Header
			dict_header dh;
			dh.dict_id = dict.ID();
			dh.word_count = (int)dict.Size();
			dh.adjacency_word_count = (int)dict.AdjacencySize();

			stream.write(reinterpret_cast<char *>(&dh), sizeof(dict_header));
			
			//	Write all word
			for(Dictionary::iterator iter = dict.begin(); iter != dict.end(); ++iter)
			{
				word_item_header wh;
				wh.id = iter->second.id;
				wh.length = (int)iter->second.word.length();
				wh.tag_count = (int)iter->second.tags.size();
				//	write word entry header
				stream.write(reinterpret_cast<char *>(&wh), sizeof(word_item_header));
				//	write word
				stream.write(reinterpret_cast<const char *>(iter->second.word.c_str()), (streamsize)iter->second.word.length()*2);
				//	Write all tag belong to this word
				for(vector<WordTag>::iterator it = iter->second.tags.begin(); it != iter->second.tags.end(); ++it)
				{
					tag_item t;
					t.tag = it->tag;
					t.frequency = it->frequency;
					stream.write(reinterpret_cast<char *>(&t), sizeof(tag_item));
				}
			}

			//	Write all adjacency word list
			for(Dictionary::adjacency_iterator iter = dict.adjacency_begin(); iter != dict.adjacency_end(); ++iter)
			{
				adjacency_word_item item;
				item.word1_id = iter->first.first;
				item.word2_id = iter->first.second;
				item.frequency = iter->second;
				//	write the adjacency word
				stream.write(reinterpret_cast<char *>(&item), sizeof(adjacency_word_item));
			}
		}
		void Deserialize(istream& stream, Dictionary& dict)
		{
			//	Read the header
			dict_header dh;
			stream.read(reinterpret_cast<char *>(&dh), sizeof(dict_header));
			if (dict.ID() <= 0)
			{
				//	 if the dictionary didn't have the id yet, we assign the dict id got from the dict file.
				dict.ID(dh.dict_id);
			}
			
			//	Read all words
			for(int i = 0; i < dh.word_count; ++i)
			{
				WordEntry we;
				word_item_header wh;
				//	read word entry header
				stream.read(reinterpret_cast<char *>(&wh), sizeof(word_item_header));
				we.id = wh.id;
				//	read word
				wchar_t* buf = new wchar_t[wh.length+1];
				//char* buf = new char[wh.length+1];
				stream.read(reinterpret_cast<char *>(buf), wh.length*2);
				buf[wh.length] = 0;
				we.word = buf;
				//	read all tag belong to this word
				for(int j = 0; j < wh.tag_count; ++j)
				{
					tag_item t;
					stream.read(reinterpret_cast<char *>(&t), sizeof(tag_item));
					WordTag wt;
					wt.tag = t.tag;
					wt.frequency = t.frequency;
					we.Add(wt);
				}
				dict.Add(we);
			}
			//	Read All adjacency word list
			for(int i = 0 ; i < dh.adjacency_word_count; ++i)
			{
				adjacency_word_item item;
				stream.read(reinterpret_cast<char *>(&item), sizeof(adjacency_word_item));
				dict.AddAdjacencyFrequency(item.word1_id, item.word2_id, item.frequency);
			}
		}
	};
}