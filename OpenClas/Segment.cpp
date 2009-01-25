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
#include "common.h"
#include "Segment.h"

namespace openclas {

	Segment::Segment(const wstring& sentence)
		: m_sentence(sentence)
	{
	}

	void Segment::construct_symbol_graph()
	{
		init_symbol_graph();
		connect_symbol_graph();
	}

	void Segment::init_symbol_graph()
	{
		m_symbol_graph.reset(new SymbolSegmentGraph());

		//	Add Begin Symbol
		m_symbol_graph->add_node(SymbolSegmentGraph::node_value_type(SYMBOL_TYPE_BEGIN));

		//	Add sentence.
		size_t			index_begin = 0;
		char_type		current_symbol = char_type();
		enum SymbolType	current_type = SYMBOL_TYPE_UNKNOWN;

		for(size_t i = 0; i < m_sentence.size(); ++i)
		{
			char_type		previous_symbol = current_symbol;
			enum SymbolType	previous_type = current_type;
			current_symbol = m_sentence[i];
			current_type = get_symbol_type(current_symbol);

			if (i != 0) {
				bool pending = false;
				//	Exception cases
				//	case: [\.+-������][0-9]+
				if (current_type == SYMBOL_TYPE_NUMBER && is_inside(previous_symbol, NUMBER_PREFIXS))
					pending = true;
				//	case: [\d][\d] or [\w][\w]
				if ( (previous_type == current_type) && (previous_type == SYMBOL_TYPE_LETTER || previous_type == SYMBOL_TYPE_NUMBER) )
					pending = true;

				if (!pending) {
					m_symbol_graph->add_node(SymbolSegmentGraph::node_value_type(previous_type, index_begin, i - index_begin));
					index_begin = i;
				}
			}

			if (i == m_sentence.size() - 1)
			{
				m_symbol_graph->add_node(SymbolSegmentGraph::node_value_type(current_type, index_begin, i - index_begin + 1));
			}
		}

		//	Add End Symbol
		m_symbol_graph->add_node(SymbolSegmentGraph::node_value_type(SYMBOL_TYPE_END));
	}

	void Segment::connect_symbol_graph()
	{
	}
}
