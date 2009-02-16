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

#endif //	_OPENCLAS_UNIT_TEST_VITERBI_HPP_
