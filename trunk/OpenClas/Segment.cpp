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
				bool adding_node = true;
				//	Exception cases
				//	case: [\.+-гогнгл][0-9]+
				if (current_type == SYMBOL_TYPE_NUMBER && is_inside(previous_symbol, NUMBER_PREFIXS))
					adding_node = false;
				//	case: [\d][\d] or [\w][\w]
				if ( (previous_type == current_type) && (previous_type == SYMBOL_TYPE_LETTER || previous_type == SYMBOL_TYPE_NUMBER) )
					adding_node = false;

				if (adding_node) {
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
