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
#ifndef _OPENCLAS_UNIT_TEST_VITERBI_HPP_
#define _OPENCLAS_UNIT_TEST_VITERBI_HPP_

#include <openclas/viterbi.hpp>
#include <boost/test/floating_point_comparison.hpp>

BOOST_AUTO_TEST_SUITE( viterbi )

using namespace openclas;

enum State { Rainy, Sunny };
enum Observation { Walk, Shop, Clean };
const char* StateName[] = { "Rainy", "Sunny" };
const char* ObservationName[] = { "walk", "shop", "clean" };

const int number_of_states = 2;
const int number_of_observations = 3;	//not for sequence, it's number of total obs

const int observation_sequence[] = { Walk, Shop, Clean };
const int number_of_observation_sequence = 3;

double start_probability[] = {/*Rainy*/ 0.6, /*Sunny*/ 0.4};
double transition_probability[] = {
	/*Rainy : */ /*Rainy*/ 0.7, /*Sunny*/ 0.3, 
	/*Sunny : */ /*Rainy*/ 0.4, /*Sunny*/ 0.6
};
double emission_probability[] = {
	/*Rainy : */ /*walk*/ 0.1, /*shop*/ 0.4, /*clean*/ 0.5,
	/*Sunny : */ /*walk*/ 0.6, /*shop*/ 0.3, /*clean*/ 0.1
};

BOOST_AUTO_TEST_CASE( test_viterbi_array )
{

	std::vector<size_t> obs(observation_sequence, observation_sequence + number_of_observation_sequence);
	viterbi_info<double> result;
	forward_viterbi(number_of_states,
		number_of_observations,
		obs,
		start_probability,
		transition_probability,
		emission_probability,
		result);

	BOOST_CHECK_CLOSE( result.prob, 0.033612, 0.00001 );
	BOOST_CHECK_CLOSE( result.v_prob, 0.009408, 0.00001 );
	BOOST_CHECK_EQUAL( result.v_path.size(), 4 );
	BOOST_CHECK_EQUAL( result.v_path[0], Sunny );
	BOOST_CHECK_EQUAL( result.v_path[1], Rainy );
	BOOST_CHECK_EQUAL( result.v_path[2], Rainy );
	BOOST_CHECK_EQUAL( result.v_path[3], Rainy );

	//print(result, StateName);
	/*
	Example output:

	Total probability is 0.033612
	The Viterbi path is ['Sunny', 'Rainy', 'Rainy', 'Rainy']
	with probability 0.009408
	*/
}

BOOST_AUTO_TEST_CASE( test_viterbi_vector )
{

	std::vector<size_t> obs(observation_sequence, observation_sequence + number_of_observation_sequence);
	viterbi_info<double> result;
	//	Example of using STL container for start_p, tran_p and emit_p
	std::vector<double> start_p(start_probability, start_probability + number_of_states);
	std::vector<double> tran_p(transition_probability, transition_probability + (number_of_states * number_of_states));
	std::vector<double> emit_p(emission_probability, emission_probability + (number_of_states * number_of_observations));
	forward_viterbi(number_of_states,
		number_of_observations,
		obs,
		start_p, tran_p, emit_p, result);
	BOOST_CHECK_CLOSE( result.prob, 0.033612, 0.00001 );
	BOOST_CHECK_CLOSE( result.v_prob, 0.009408, 0.00001 );
	BOOST_CHECK_EQUAL( result.v_path.size(), 4 );
	BOOST_CHECK_EQUAL( result.v_path[0], Sunny );
	BOOST_CHECK_EQUAL( result.v_path[1], Rainy );
	BOOST_CHECK_EQUAL( result.v_path[2], Rainy );
	BOOST_CHECK_EQUAL( result.v_path[3], Rainy );

	//print(result, StateName);
	/*
	Example output:

	Total probability is 0.033612
	The Viterbi path is ['Sunny', 'Rainy', 'Rainy', 'Rainy']
	with probability 0.009408
	*/
}


BOOST_AUTO_TEST_SUITE_END()

//	_OPENCLAS_UNIT_TEST_VITERBI_HPP_
#endif
