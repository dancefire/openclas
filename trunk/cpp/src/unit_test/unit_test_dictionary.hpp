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
#ifndef _OPENCLAS_UNIT_TEST_DICTIONARY_HPP_
#define _OPENCLAS_UNIT_TEST_DICTIONARY_HPP_

#include <openclas/dictionary.hpp>
#include <boost/test/floating_point_comparison.hpp>

BOOST_AUTO_TEST_SUITE( dictionary )

using namespace openclas;

/*******************************************************************
*
*	DictEntry
*
********************************************************************/

BOOST_AUTO_TEST_CASE( test_DictEntry_add )
{
	DictEntry entry;
	entry.word = L"TestName";

	BOOST_CHECK_EQUAL( entry.tags.size(), 0 );

	entry.add(1, 10);

	BOOST_CHECK_EQUAL( entry.tags.size(), 1 );
	BOOST_CHECK_EQUAL( entry.tags[0].tag, 1 );
	BOOST_CHECK_EQUAL( entry.tags[0].weight, 10 );
	
	entry.add(2, 20);

	BOOST_CHECK_EQUAL( entry.tags.size(), 2 );
	BOOST_CHECK_EQUAL( entry.tags[0].tag, 1 );
	BOOST_CHECK_EQUAL( entry.tags[0].weight, 10 );
	BOOST_CHECK_EQUAL( entry.tags[1].tag, 2 );
	BOOST_CHECK_EQUAL( entry.tags[1].weight, 20 );

	entry.add(1, 30);

	BOOST_CHECK_EQUAL( entry.tags.size(), 2 );
	BOOST_CHECK_EQUAL( entry.tags[0].tag, 1 );
	BOOST_CHECK_EQUAL( entry.tags[0].weight, 30 );
	BOOST_CHECK_EQUAL( entry.tags[1].tag, 2 );
	BOOST_CHECK_EQUAL( entry.tags[1].weight, 20 );
}

BOOST_AUTO_TEST_CASE( test_DictEntry_remove )
{
    DictEntry entry;

	entry.add(1, 10);
	BOOST_CHECK_EQUAL( entry.tags.size(), 1 );

	//	remove the first and only one.
	entry.remove(1);
	BOOST_CHECK_EQUAL( entry.tags.size(), 0 );

	entry.add(1, 10);
	entry.add(2, 20);
	entry.add(3, 30);
	BOOST_CHECK_EQUAL( entry.tags.size(), 3 );

	//	remove in random order
	entry.remove(2);
	BOOST_CHECK_EQUAL( entry.tags.size(), 2 );

	//	remove unexisted tag
	entry.remove(4);
	BOOST_CHECK_EQUAL( entry.tags.size(), 2 );

	//	remove the last in order one
	entry.remove(3);
	BOOST_CHECK_EQUAL( entry.tags.size(), 1 );

	//	remove the only one
	entry.remove(1);
	BOOST_CHECK_EQUAL( entry.tags.size(), 0 );

	//	further remove, should not cause an error.
	entry.remove(1);
	BOOST_CHECK_EQUAL( entry.tags.size(), 0 );
}

BOOST_AUTO_TEST_CASE( test_DictEntry_get_forward_weight )
{
	DictEntry entry;
	entry.forward[L"TestA"] = 0.135;
	entry.forward[L"TestB"] = 0.246;
	
	BOOST_CHECK_CLOSE( entry.get_forward_weight(L"TestA"), 0.135, 0.00001 );
	BOOST_CHECK_CLOSE( entry.get_forward_weight(L"TestB"), 0.246, 0.00001 );
	BOOST_CHECK_CLOSE( entry.get_forward_weight(L"NotExist"), 0., 0.00001 );
}

BOOST_AUTO_TEST_CASE( test_DictEntry_get_backward_weight )
{
	DictEntry entry;
	entry.backward[L"TestA"] = 0.246;
	entry.backward[L"TestB"] = 0.135;
	
	BOOST_CHECK_CLOSE( entry.get_backward_weight(L"TestA"), 0.246, 0.00001 );
	BOOST_CHECK_CLOSE( entry.get_backward_weight(L"TestB"), 0.135, 0.00001 );
	BOOST_CHECK_CLOSE( entry.get_backward_weight(L"NotExist"), 0., 0.00001 );
}


/*******************************************************************
*
*	WordIndexer
*
********************************************************************/

static const wchar_t* INDEX_STRING[] = {
	L"",	L"A",	L"AB",	L"ABC",	L"ACDEFG",
	L"ACC",	L"ACD",	L"BCC",	L"BCD",	L"C"
};

BOOST_AUTO_TEST_CASE( test_WordIndexer )
{
    WordIndexer indexer;
	DictEntry entry1;
	DictEntry entry2;

	//	add the words
	//		const wchar_t*
	for (int i = 0; i < 4; ++i)
	{
		indexer.add(INDEX_STRING[i], &entry1);
	}
	//		std::wstring
	for (int i = 4; i < 8; ++i)
	{
		std::wstring str(INDEX_STRING[i]);
		indexer.add(str, &entry1);
	}
	//		std::wstring::iterator
	for (int i = 8; i < 10; ++i)
	{
		std::wstring str(INDEX_STRING[i]);
		indexer.add(str.begin(), str.end(), &entry1);
	}

	//	modify the first 5 words in INDEX_STRING to entry2.
	for (int i = 0; i < 5; ++i)
	{
		std::wstring str(INDEX_STRING[i]);
		indexer.add(str.begin(), str.end(), &entry2);
	}

	//	get the entry and verify it
	//		the first 5 in INDEX_STRING should be &entry2
	for (int i = 0; i < 5; ++i)
	{
		std::wstring str(INDEX_STRING[i]);
		DictEntry* entry_ptr = indexer.get(str.begin(), str.end());
		BOOST_CHECK_EQUAL( entry_ptr, &entry2 );
	}

	//		the last 5 in INDEX_STRING should be &entry1
	for (int i = 5; i < 10; ++i)
	{
		std::wstring str(INDEX_STRING[i]);
		DictEntry* entry_ptr = indexer.get(str.begin(), str.end());
		BOOST_CHECK_EQUAL( entry_ptr, &entry1 );
	}

	//	get Not Exist entry, should return 0
	BOOST_CHECK_EQUAL( indexer.get(L"NotExist"), static_cast<DictEntry*>(0) );

	//	Test find_prefixes function
	std::vector<DictEntry*> prefix_list = indexer.prefix(L"ACDEFGHK");
	BOOST_REQUIRE_EQUAL( prefix_list.size(), 4 );
	BOOST_CHECK_EQUAL( prefix_list[0], &entry2 );	//	L""
	BOOST_CHECK_EQUAL( prefix_list[1], &entry2 );	//	L"A"
	BOOST_CHECK_EQUAL( prefix_list[2], &entry1 );	//	L"ACD"
	BOOST_CHECK_EQUAL( prefix_list[3], &entry2 );	//	L"ACDEFG"

	//	remove L""
	BOOST_CHECK_EQUAL( indexer.get(L""), &entry2 );
	indexer.remove(L"");
	BOOST_CHECK_EQUAL( indexer.get(L""), static_cast<DictEntry*>(0) );

	//	remove L"AB"
	BOOST_CHECK_EQUAL( indexer.get(L"A"), &entry2 );
	BOOST_CHECK_EQUAL( indexer.get(L"AB"), &entry2 );
	BOOST_CHECK_EQUAL( indexer.get(L"ABC"), &entry2 );
	BOOST_CHECK_EQUAL( indexer.get(L"ACC"), &entry1 );
	BOOST_CHECK_EQUAL( indexer.get(L"ACD"), &entry1 );
	indexer.remove(L"AB");
	BOOST_CHECK_EQUAL( indexer.get(L"A"), &entry2 );
	BOOST_CHECK_EQUAL( indexer.get(L"AB"), static_cast<DictEntry*>(0) );
	BOOST_CHECK_EQUAL( indexer.get(L"ABC"), &entry2 );
	BOOST_CHECK_EQUAL( indexer.get(L"ACC"), &entry1 );
	BOOST_CHECK_EQUAL( indexer.get(L"ACD"), &entry1 );

	//	remove again
	indexer.remove(L"AB");
	BOOST_CHECK_EQUAL( indexer.get(L"A"), &entry2 );
	BOOST_CHECK_EQUAL( indexer.get(L"AB"), static_cast<DictEntry*>(0) );
	BOOST_CHECK_EQUAL( indexer.get(L"ABC"), &entry2 );
	BOOST_CHECK_EQUAL( indexer.get(L"ACC"), &entry1 );
	BOOST_CHECK_EQUAL( indexer.get(L"ACD"), &entry1 );

	//	remove all
	for (int i = 0; i < 10; ++i)
	{
		std::wstring str(INDEX_STRING[i]);
		if (str != L"" && str != L"AB")
		{
			BOOST_CHECK_NE( indexer.get(INDEX_STRING[i]), static_cast<DictEntry*>(0) );
			indexer.remove(INDEX_STRING[i]);
			BOOST_CHECK_EQUAL( indexer.get(INDEX_STRING[i]), static_cast<DictEntry*>(0) );
		}
	}
}


/*******************************************************************
*
*	Dictionary
*
********************************************************************/


/*****************   Word   *****************/
BOOST_AUTO_TEST_CASE( test_Dictionary_add_word )
{
    Dictionary dict;
	const std::wstring word1(L"WordA");
	const std::wstring word2(L"WordB");

	//	Add word
	DictEntry* entry1 = dict.add_word(word1);
	BOOST_CHECK( entry1->word == word1 );
	DictEntry* entry2 = dict.add_word(word2);
	BOOST_CHECK( entry2->word == word2 );

	entry1->add(123, 100);

	//	Add exist word, should return the existing entry
	DictEntry* entry3 = dict.add_word(word1);
	BOOST_REQUIRE_EQUAL( entry3->tags.size(), 1 );
	BOOST_CHECK_EQUAL( entry3->tags[0].tag, 123 );
	BOOST_CHECK_EQUAL( entry3->tags[0].weight, 100 );

	//	Get
	DictEntry* entry4 = dict.get_word(word1.begin(), word1.end());
	BOOST_CHECK_EQUAL( entry4, entry1 );
	//	get_word() const;
	const Dictionary& const_dict = dict;
	const DictEntry* entry5 = const_dict.get_word(word1.begin(), word1.end());
	BOOST_CHECK_EQUAL( entry5, const_cast<DictEntry*>(entry1) );

	//	Get NonExist
	std::wstring NonExist(L"NonExist");
	DictEntry* entry6 = dict.get_word(NonExist.begin(), NonExist.end());
	BOOST_CHECK_EQUAL( entry6, static_cast<DictEntry*>(0) );

	//	words();
	BOOST_CHECK_EQUAL( dict.words().size(), 2 );

	//	Remove NonExist
	BOOST_CHECK_NO_THROW( dict.remove_word( NonExist ) );

	//	Remove normal entry
	DictEntry* entry7 = dict.get_word(word1);
	BOOST_CHECK_NE( entry7, static_cast<DictEntry*>(0) );
	dict.remove_word(word1);
	entry7 = dict.get_word(word1);
	BOOST_CHECK_EQUAL( entry7, static_cast<DictEntry*>(0) );

	//	Remove the last entry
	DictEntry* entry8 = dict.get_word(word2);
	BOOST_CHECK_NE( entry8, static_cast<DictEntry*>(0) );
	dict.remove_word(word2);
	entry8 = dict.get_word(word2);
	BOOST_CHECK_EQUAL( entry8, static_cast<DictEntry*>(0) );
	BOOST_CHECK_EQUAL( dict.words().size(), 0 );
}

BOOST_AUTO_TEST_CASE( test_Dictionary_prefix )
{
    Dictionary dict;
	dict.add_word(L"A");
	dict.add_word(L"B");
	dict.add_word(L"BC");
	dict.add_word(L"ABC");
	dict.add_word(L"ABCD");
	dict.add_word(L"ABDD");
	dict.add_word(L"ABCDEF");

	std::vector<DictEntry*> result = dict.prefix(L"ABCDEFGHIJKL");

	BOOST_REQUIRE_EQUAL( result.size(), 4 );
	BOOST_CHECK( result[0]->word == L"A" );
	BOOST_CHECK( result[1]->word == L"ABC" );
	BOOST_CHECK( result[2]->word == L"ABCD" );
	BOOST_CHECK( result[3]->word == L"ABCDEF" );
}

/*****************   Tag   *****************/

BOOST_AUTO_TEST_CASE( test_Dictionary_init_tag_dict )
{
    Dictionary dict;
	dict.init_tag_dict(10);
	BOOST_CHECK_EQUAL( dict.tags().size(), 10 );
	BOOST_CHECK_EQUAL( dict.tags_transit().size(), 100 );

	dict.init_tag_dict(1000);
	BOOST_CHECK_EQUAL( dict.tags().size(), 1000 );
	BOOST_CHECK_EQUAL( dict.tags_transit().size(), 1000*1000 );
}

BOOST_AUTO_TEST_CASE( test_Dictionary_tag_weight )
{
    Dictionary dict;
	dict.init_tag_dict(4);
	//	tags()
	BOOST_REQUIRE_EQUAL( dict.tags().size(), 4 );
	//	add
	dict.add_tag_weight(0, 10);
	dict.add_tag_weight(1, 2);
	dict.add_tag_weight(2, 4);
	dict.add_tag_weight(3, 5);
	//	get
	BOOST_CHECK_EQUAL( dict.get_tag_weight(0), 10 );
	BOOST_CHECK_EQUAL( dict.get_tag_weight(1), 2 );
	BOOST_CHECK_EQUAL( dict.get_tag_weight(2), 4 );
	BOOST_CHECK_EQUAL( dict.get_tag_weight(3), 5 );
	//		not exist
	BOOST_CHECK_EQUAL( dict.get_tag_weight(4), 0 );
	//	add more than once
	dict.add_tag_weight(0, 1000);
	BOOST_CHECK_EQUAL( dict.get_tag_weight(0), 1000 );
	//	remove
	for (int i = 0; i < 4; ++i)
	{
		BOOST_CHECK_NE( dict.get_tag_weight(i), 0 );
		dict.remove_tag_weight(i);
		BOOST_CHECK_EQUAL( dict.get_tag_weight(i), 0 );
	}
}

/*****************   Tag transit   *****************/
BOOST_AUTO_TEST_CASE( test_Dictionary_get_tag_transit_index )
{
	Dictionary dict;
	dict.init_tag_dict(20);
	BOOST_CHECK_EQUAL( dict.get_tag_transit_index(5, 8), (5*20)+8 );
	BOOST_CHECK_EQUAL( dict.get_tag_transit_index(0, 0), 0 );
	BOOST_CHECK_EQUAL( dict.get_tag_transit_index(19, 19), 399 );
}

BOOST_AUTO_TEST_CASE( test_Dictionary_tag_transit )
{
	Dictionary dict;
	dict.init_tag_dict(4);
	BOOST_REQUIRE_EQUAL( dict.tags_transit().size(), 4*4 );

	dict.add_tag_transit_weight(0, 2, 10);
	BOOST_CHECK_EQUAL( dict.get_tag_transit_weight(0, 2), 10 );

	dict.add_tag_transit_weight(5, 22); //	(1,1)
	BOOST_CHECK_EQUAL( dict.get_tag_transit_weight(1, 1), 22 );

	dict.remove_tag_transit_weight(1, 1);
	BOOST_CHECK_EQUAL( dict.get_tag_transit_weight(1, 1), 0 );

	dict.remove_tag_transit_weight(0, 2);
	BOOST_CHECK_EQUAL( dict.get_tag_transit_weight(0, 2), 0 );

	dict.remove_tag_transit_weight(2, 2);
	BOOST_CHECK_EQUAL( dict.get_tag_transit_weight(2, 2), 0 );
}

BOOST_AUTO_TEST_SUITE_END()

#endif //	_OPENCLAS_UNIT_TEST_DICTIONARY_HPP_
