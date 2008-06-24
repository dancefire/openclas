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


	��Ȩ���� 2007 Dancefire (dancefire@gmail.com)��
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