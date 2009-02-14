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
*
*	a C++ implementatiton of Viterbi algorithm, which is described 
*	in Wikipedia in Python as an example.
*	http://en.wikipedia.org/wiki/Viterbi_algorithm
*/

#pragma once
#ifndef _OPENCLAS_VITERBI_HPP_
#define _OPENCLAS_VITERBI_HPP_

#include <vector>
#include <iostream>

namespace openclas {

	template<typename T, typename Container = T*>
	class matrix_wrapper {
	public:
		matrix_wrapper(size_t rows, size_t columns, const Container& data) : m_row(rows), m_column(columns), m_data(data) {}
		const T& operator() (size_t row, size_t column)
		{
			if (row < m_row && column < m_column)
				return m_data[row * m_column + column];
			else
				throw std::out_of_range("out of range of matrix");
		}
	protected:
		size_t m_row;
		size_t m_column;
		const Container& m_data;
	};

	template <typename ValueType>
	struct viterbi_info {
		ValueType prob;
		ValueType v_prob;
		std::vector<size_t> v_path;
		viterbi_info() : prob(0), v_prob(0), v_path() {}
		void set(ValueType prob, ValueType v_prob, size_t v_path_index)
		{
			this->prob = prob;
			this->v_prob = v_prob;
			this->v_path.clear();
			this->v_path.push_back(v_path_index);
		}
	};

	template <typename ValueType, typename ContainerStart, typename ContainerTrans, typename ContainerEmit>
	void forward_viterbi(size_t number_of_states,
		size_t number_of_observations,
		const std::vector<size_t>& obs_s,
		const ContainerStart& start_p,
		const ContainerTrans& trans_probability,
		const ContainerEmit& emit_probability,
		viterbi_info<ValueType>& result
		)
	{
		matrix_wrapper<ValueType, ContainerTrans> trans_p(number_of_states, number_of_states, trans_probability);
		matrix_wrapper<ValueType, ContainerEmit> emit_p(number_of_states, number_of_observations, emit_probability);

		typedef viterbi_info<ValueType> viterbi_info_type;
		std::vector<viterbi_info_type> T(number_of_states);
		for (size_t state = 0; state < number_of_states; ++state)
		{
			//           prob.           V. prob.        V. path  
			T[state].set(start_p[state], start_p[state], state);
		}

		for (std::vector<size_t>::const_iterator output = obs_s.begin(); output != obs_s.end(); ++output)
		{
			std::vector<viterbi_info_type> U(number_of_states);
			for (size_t next_state = 0; next_state < number_of_states; ++next_state)
			{
				viterbi_info_type& next = U[next_state];
				for(size_t source_state = 0; source_state < number_of_states; ++source_state)
				{
					viterbi_info_type source = T[source_state];
					double p = emit_p(source_state, *output) * trans_p(source_state, next_state);
					source.prob *= p;
					source.v_prob *= p;
					next.prob += source.prob;
					if (source.v_prob > next.v_prob)
					{
						next.v_path.assign(source.v_path.begin(), source.v_path.end());
						next.v_path.push_back(next_state);
						next.v_prob = source.v_prob;
					}
				}
			}
			T = U;
		}
		// apply sum/max to the final states:
		for (size_t state = 0; state < number_of_states; ++state)
		{
			viterbi_info_type& t = T[state];
			result.prob += t.prob;	/*total*/
			if (t.v_prob > result.v_prob)	 /*valmax*/
			{
				result.v_path = t.v_path;
				result.v_prob = t.v_prob;
			}
		}
	}

	template <typename ValueType>
	void print(const viterbi_info<ValueType>& result, const char** state_name)
	{
		std::cout << "Total probability is " << result.prob << std::endl << "The Viterbi path is [";
		for (std::vector<size_t>::const_iterator iter = result.v_path.begin(); iter != result.v_path.end(); ++iter)
		{
			if (iter != result.v_path.begin())
				std::cout << ", ";
			std::cout<< "'" << state_name[*iter] << "'";
		}
		std::cout << "]" << std::endl << "  with probability " << result.v_prob << std::endl;
	}
}

#endif	//	_OPENCLAS_VITERBI_HPP_
