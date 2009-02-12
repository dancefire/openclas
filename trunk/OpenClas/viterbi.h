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


��Ȩ���� 2007-2009 Dancefire (dancefire@gmail.com)��
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
*	$Id$
*
*	a C++ implementatiton of Viterbi algorithm, which is described 
*	in Wikipedia in Python as an example.
*	http://en.wikipedia.org/wiki/Viterbi_algorithm
*/

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
