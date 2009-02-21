#pragma once
#ifndef _OPENCLAS_UNIT_TEST_SERIALIZATION_HPP_
#define _OPENCLAS_UNIT_TEST_SERIALIZATION_HPP_

#include <openclas/serialization.hpp>

BOOST_AUTO_TEST_SUITE( serialization )

/*******************************************************************
*
*				namespace openclas::ict
*
********************************************************************/

using namespace openclas;
using namespace openclas::ict;

static const char* core_dict_name = "data\\coreDict.dct";
static const char* bigram_dict_name = "data\\BigramDict.dct";
static const char* core_tag_name = "data\\lexical.dct";
static const char* people_dict_basename = "data\\nr";
static const char* place_dict_basename = "data\\ns";
static const char* org_dict_basename = "data\\tr";
static const char* dict_ext = ".dct";
static const char* tag_ext = ".ctx";

BOOST_AUTO_TEST_CASE( test_Serialization_ICT_get_special_word_tag )
{
	BOOST_CHECK_EQUAL( get_special_word_tag(L"始##始"), WORD_TAG_BEGIN );
	BOOST_CHECK_EQUAL( get_special_word_tag(L"末##末"), WORD_TAG_END );
	BOOST_CHECK_EQUAL( get_special_word_tag(L"未##串"), WORD_TAG_NX );
	BOOST_CHECK_EQUAL( get_special_word_tag(L"未##地"), WORD_TAG_NS );
	BOOST_CHECK_EQUAL( get_special_word_tag(L"未##人"), WORD_TAG_NR );
	BOOST_CHECK_EQUAL( get_special_word_tag(L"未##时"), WORD_TAG_T );
	BOOST_CHECK_EQUAL( get_special_word_tag(L"未##数"), WORD_TAG_M );
	BOOST_CHECK_EQUAL( get_special_word_tag(L"未##它"), WORD_TAG_N );
	BOOST_CHECK_EQUAL( get_special_word_tag(L"未##团"), WORD_TAG_NT );
	BOOST_CHECK_EQUAL( get_special_word_tag(L"未##专"), WORD_TAG_NZ );
	BOOST_CHECK_EQUAL( get_special_word_tag(L"未##xx"), WORD_TAG_UNKNOWN );
}

BOOST_AUTO_TEST_CASE( test_Serialization_ICT_get_tag_from_pos )
{
    BOOST_CHECK_EQUAL( get_tag_from_pos(29696), WORD_TAG_T );
    BOOST_CHECK_EQUAL( get_tag_from_pos(27904), WORD_TAG_M );
    BOOST_CHECK_EQUAL( get_tag_from_pos(30464), WORD_TAG_W );
    BOOST_CHECK_EQUAL( get_tag_from_pos('n'*256+'x'), WORD_TAG_NX );
}

BOOST_AUTO_TEST_CASE( test_Serialization_ICT_get_gb2312_array )
{
	shared_array<wchar_t> GB2312_ARRAY = get_gb2312_array();
	GB2312_ARRAY[0] = L'啊';
}

BOOST_AUTO_TEST_CASE( test_Serialization_ICT_load_from_dct )
{
	Dictionary dict;
	
	BOOST_CHECK_EQUAL( dict.words().size(), 0 );
	load_words_from_dct(dict, core_dict_name);
	BOOST_CHECK_NE( dict.words().size(), 0 );

	load_words_transit_from_dct(dict, bigram_dict_name);
	BOOST_CHECK_NE( dict.words().size(), 0 );
}

BOOST_AUTO_TEST_CASE( test_Serialization_ICT_load_tags_from_ctx )
{
    BOOST_FAIL( "Test is not ready yet" );
}

BOOST_AUTO_TEST_CASE( test_Serialization_ICT_load_from_file )
{
	//Dictionary dict;
	//
	//BOOST_CHECK_EQUAL( dict.words().size(), 0 );
	//ict::load_from_file(dict, core_tag_name, core_dict_name, bigram_dict_name);
	//BOOST_CHECK_NE( dict.words().size(), 0 );
}

BOOST_AUTO_TEST_CASE( test_Serialization_ICT_save_words_to_dct )
{
    BOOST_FAIL( "Test is not ready yet" );
}

BOOST_AUTO_TEST_CASE( test_Serialization_ICT_save_words_transit_to_dct )
{
    BOOST_FAIL( "Test is not ready yet" );
}

BOOST_AUTO_TEST_CASE( test_Serialization_ICT_save_tags_to_ctx )
{
    BOOST_FAIL( "Test is not ready yet" );
}

BOOST_AUTO_TEST_CASE( test_Serialization_ICT_save_to_file )
{
    BOOST_FAIL( "Test is not ready yet" );
}


/*******************************************************************
*
*				namespace openclas
*
********************************************************************/

static const char* dict_name = "dict.ocd";

BOOST_AUTO_TEST_CASE( test_Serialization )
{
	Dictionary dict;
	dict.init_tag_dict(10);
	dict.add_tag_weight(1, 231);
	dict.add_tag_transit_weight(1, 2, 123);
	dict.add_word(L"ABCD")->add(0, 100);
	dict.add_word(L"AB")->add(1, 200);
	dict.get_word(L"AB")->forward[L"ABCD"] = 1013;

	openclas::save_to_file(dict, dict_name);

	Dictionary dict2;
	openclas::load_from_file(dict2, dict_name);

	BOOST_CHECK_EQUAL( dict2.tags().size(), 10 );
	BOOST_CHECK_EQUAL( dict2.get_tag_weight(1), 231 );
	BOOST_CHECK_EQUAL( dict2.get_tag_transit_weight(1, 2), 123 );

	BOOST_CHECK_EQUAL( dict2.words().size(), 2 );
	DictEntry* entry1 = dict2.get_word(L"ABCD");
	BOOST_REQUIRE_NE( entry1, static_cast<DictEntry*>(0) );
	BOOST_REQUIRE_EQUAL( entry1->tags.size(), 1 );
	BOOST_CHECK_EQUAL( entry1->tags[0].tag, 0 );
	BOOST_CHECK_EQUAL( entry1->tags[0].weight, 100 );
	
	DictEntry* entry2 = dict2.get_word(L"AB");
	BOOST_REQUIRE_NE( entry2, static_cast<DictEntry*>(0) );
	BOOST_REQUIRE_EQUAL( entry2->tags.size(), 1 );
	BOOST_CHECK_EQUAL( entry2->tags[0].tag, 1 );
	BOOST_CHECK_EQUAL( entry2->tags[0].weight, 200 );
	BOOST_CHECK_EQUAL( entry2->forward[L"ABCD"], 1013 );
}

BOOST_AUTO_TEST_SUITE_END()

#endif //	_OPENCLAS_UNIT_TEST_SERIALIZATION_HPP_
