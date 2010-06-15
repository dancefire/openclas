/*
 * Copyright (c) 2007-2010 Tao Wang <dancefire@gmail.org>
 * See the file "LICENSE.txt" for usage and redistribution license requirements
 *
 *	$Id$
 */

#define BOOST_TEST_MODULE OpenCLAS_Unit_Test
#define BOOST_TEST_MAIN

#include <boost/test/included/unit_test.hpp>

using namespace boost::unit_test;


static const wchar_t* sample[] = {
	L"",
	L"English Words",
	L"他说的确实在理。",
	L"19９5年底ｇoｏgｌｅ在1月份大会上说的确实在理。",
	L"这个门的把手坏了好几天了",
	L"你把手抬高一点儿",
	L"这个动作的要领其实很简单",
	L"下午我要领工资，恐怕赶不回去",
	L"办独生子女证，一对夫妻一次性交一百元钱",
	L"我知道你不知道我知道你不知道我知道你不知道",
	L"已经分房和尚未分房的同志",
	L"馆内陈列着周恩来和邓颖超生前使用过的物品",
	L"张华平喜欢看书",
	L"舒展喜欢看书"
};

const int sample_count = 14;

static const char* mini_dict_base_name = "data/mini";

#include "unit_test_dictionary.hpp"
#include "unit_test_k_shortest_path.hpp"
#include "unit_test_segment.hpp"
#include "unit_test_serialization.hpp"
#include "unit_test_utility.hpp"
#include "unit_test_viterbi.hpp"
#include "unit_test_longtime.hpp"
