/*
 * Copyright (c) 2007-2010 Tao Wang <dancefire@gmail.org>
 * See the file "LICENSE.txt" for usage and redistribution license requirements
 *
 *	$Id$
 */

#pragma once
#ifndef _OPENCLAS_SEGMENT_HPP_
#define _OPENCLAS_SEGMENT_HPP_

#include "common.hpp"
#include "utility.hpp"
#include "dictionary.hpp"
#include "k_shortest_path.hpp"

#include <boost/utility.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <limits>		//	for std::numeric_limits<double>::max()
#include <vector>
#include <stdexcept>	//	for std::logic_error

namespace boost {
	enum vertex_desc_t { vertex_desc = 1000 };
	enum graph_terminal_t { graph_terminal = 3000 };
	BOOST_INSTALL_PROPERTY(vertex, desc);
	BOOST_INSTALL_PROPERTY(graph, terminal);
}

namespace openclas {

	using namespace boost;

	struct WordInformation {
		enum WordTag tag;
		double weight;
		size_t offset;
		size_t length;
		bool is_recorded;
		size_t index;
		const DictEntry* entry;
		WordInformation()
			: tag(WORD_TAG_UNKNOWN), weight(0), offset(0), length(0), is_recorded(false), index(0), entry(0)
		{}
		bool operator== (const WordInformation& other) const
		{
			return (
				this->tag == other.tag
				&& this->weight == other.weight
				&& this->offset == other.offset
				&& this->length == other.length
				&& this->is_recorded == other.is_recorded
				&& this->index == other.index
				&& this->entry == other.entry
				);
		}
	};

	typedef property<vertex_index_t, size_t,
		property<vertex_distance_t, double, 
		property<vertex_predecessor_t, size_t,
		property<vertex_desc_t, WordInformation> > > > VertexProperty;

	typedef property<edge_weight_t, double> EdgeProperty;

	typedef property<graph_terminal_t, std::pair<size_t, size_t> > GraphProperty;

	typedef adjacency_list<vecS, vecS, directedS, 
		VertexProperty, EdgeProperty, GraphProperty> WordGraph;

	class Segment{
	public:
		typedef std::map< size_t, std::vector<WordInformation> > out_table_type;
		typedef std::vector<shared_ptr<WordGraph> > graph_list_type;
		typedef struct {
			double weight;
			std::vector<WordInformation> words;
		} segment_type;
	public:
		static std::wstring segment_to_string(const std::wstring& text, segment_type& seg)
		{
			std::wostringstream oss;
			bool empty = true;
			for(std::vector<WordInformation>::iterator iSeg = seg.words.begin(); iSeg != seg.words.end(); ++iSeg)
			{
				if (!empty)
					oss << " ";

				if (iSeg->tag != WORD_TAG_BEGIN && iSeg->tag != WORD_TAG_END) {
					std::wstring word(text.begin() + iSeg->offset, text.begin() + iSeg->offset + iSeg->length);
					oss << word << "/" << WORD_TAG_NAME[iSeg->tag];
					empty = false;
				}
			}
			return oss.str();
		}


		static std::vector<segment_type> segment(const std::wstring& text, const Dictionary& dict, int k = 1)
		{
			//	create sub-graphs based on given text.
			graph_list_type graphs = create_graphs(text, dict);
			return segment(graphs, k);
		}

		static std::vector<segment_type> segment(graph_list_type& graphs, int k = 1)
		{
			//	calculate k-shortest paths for each graph.
			std::vector<std::vector<path_type> > subgraph_path_lists(graphs.size());
			for (size_t i = 0; i < graphs.size(); ++i)
			{
				WordGraph& graph = *graphs[i];
				//	get the k best path
				graph_property<WordGraph, graph_terminal_t>::type
					gterminal = get_property(graph, graph_terminal);
				std::vector<path_type> results;
				dag_k_shortest_paths(graph, gterminal.first, gterminal.second, results, k);

				//	Get words for each result
				std::vector<path_type>& segments_list = subgraph_path_lists[i];
				for (std::vector<path_type>::iterator iPath = results.begin(); iPath != results.end(); ++iPath)
				{
					segments_list.push_back(*iPath);
				}

			}

			//	construct overall k-shortest paths.
			std::vector<std::vector<path_type> > overall_k_shortest_paths = get_overall_k_shortest_path(subgraph_path_lists, k);

			//	construct k-shortest path in segment_type list;
			std::vector<segment_type> segs;
			for(size_t i = 0; i < overall_k_shortest_paths.size(); ++i)
			{
				segment_type seg;
				seg.weight = 0;
				for (size_t j = 0; j < overall_k_shortest_paths[i].size(); ++j)
				{
					path_type& path = overall_k_shortest_paths[i][j];
					seg.weight += path.weight;

					//	get the WordInformation
					property_map<WordGraph, vertex_desc_t>::type
						vprop_map = get(vertex_desc, *graphs[j]);
					for (std::vector<size_t>::iterator iPath = path.nodelist.begin(); iPath != path.nodelist.end(); ++iPath)
					{
						std::vector<size_t>::iterator iNextPath = iPath;
						if (++iNextPath != path.nodelist.end())
							seg.words.push_back(vprop_map[*iPath]);
					}
				}
				segs.push_back(seg);
			}

			return segs;
		}

		static graph_list_type create_graphs(const wstring& text, const Dictionary& dict)
		{
			graph_list_type sub_graphs;

			//	return empty graph list if the text is empty.
			if (text.empty())
				return sub_graphs;

			std::vector<WordInformation> atoms;
			split_atoms(text, atoms);
			out_table_type out_table;
			create_out_table(text, dict, atoms, out_table);
			create_graphs(text, dict, out_table, sub_graphs);

			return sub_graphs;
		}

	protected:
		///	Input:	text,
		///	Output:	atoms
		static void split_atoms(const wstring& text, std::vector<WordInformation>& atoms)
		{
			//	Add text.
			size_t		index_begin = 0;
			wchar_t		current_symbol = wchar_t();
			enum SymbolType	current_type = SYMBOL_TYPE_UNKNOWN;

			for(size_t i = 0; i < text.size(); ++i)
			{
				wchar_t		previous_symbol = current_symbol;
				enum SymbolType	previous_type = current_type;
				current_symbol = text[i];
				current_type = get_symbol_type(current_symbol);

				if (i != 0) {
					bool pending = false;
					//	Exception cases
					//	case: [\.+-．－＋][0-9]+
					if (current_type == SYMBOL_TYPE_NUMBER && exist(previous_symbol, NUMBER_PREFIXS))
						pending = true;
					//	case: [\d][\d] or [\w][\w]
					if ( (previous_type == current_type) && (previous_type == SYMBOL_TYPE_LETTER || previous_type == SYMBOL_TYPE_NUMBER) )
						pending = true;

					if (!pending) {
						WordInformation word = create_word(previous_type, index_begin, i - index_begin);
						atoms.push_back(word);
						index_begin = i;
					}
				}

				if (i == text.size() - 1)
				{
					WordInformation word = create_word(current_type, index_begin, i - index_begin + 1);
					atoms.push_back(word);
				}
			}
		}

		static void merge_atoms(const wstring& /*text*/, std::vector<WordInformation>& /*atoms*/)
		{
			//	TODO: implement the following:
			//		[may be better adjust it by dict's weight]
			//	get_bias_case_1():	don't split 月份
			//	get_continue_case_1(): 	([0-9０-９]+[年月])/([末内中底前间初])
		}

		static void get_special_word_info(const Dictionary& dict, WordInformation& item)
		{
			std::wstring special_word = get_special_word_string(item.tag);
			const DictEntry* entry = dict.get_word(special_word);
			if (entry)
			{
				item.entry = entry;
				item.weight = 0;
				for (std::vector<TagEntry>::const_iterator iTag = entry->tags.begin(); iTag != entry->tags.end(); ++iTag)
					item.weight += iTag->weight;
			}else{
				std::ostringstream out;
				out << "Dictionary does not contain the entry for special word \"" << narrow(special_word, locale_platform) << "\"";
				throw std::logic_error(out.str());
			}
		}

		///	Input:	text, dict, atoms
		///	Output:	out_table_type
		static void create_out_table(const wstring& text, const Dictionary& dict, const std::vector<WordInformation>& atoms, out_table_type& out_table)
		{
			//	initialize from atoms
			for (size_t i = 0; i < atoms.size(); ++i)
			{
				WordInformation atom = atoms[i];
				out_table[atoms[i].offset].push_back(atom);
			}

			//	initialize from dictionary
			for (size_t i = 0; i < atoms.size(); ++i)
			{
				const WordInformation& atom = atoms[i];
				if (atom.is_recorded)
				{
					//	look up dictionary for prefixes of given sequence.
					std::vector<DictEntry*> entries = dict.prefix(text.begin() + atom.offset, text.end());
					for(std::vector<DictEntry*>::iterator iter = entries.begin(); iter != entries.end(); ++iter)
					{
						//	find the word list of the given offset.
						size_t word_length = (*iter)->word.length();
						//	make sure the found word will ended at the offset which is a begin of one of atoms.
						out_table_type::iterator it = out_table.find(atom.offset + word_length);
						if (word_length >= atom.length && it != out_table.end())
						{
							//	Construct the word
							WordInformation item;

							//	attach the Dictionary entry
							item.entry = *iter;
							//	calculate the weight
							item.weight = 0;
							//	sum all tags weights as the item's weight
							for (std::vector<TagEntry>::iterator iTag = (*iter)->tags.begin(); iTag != (*iter)->tags.end(); ++iTag)
								item.weight += iTag->weight;

							//	use the tag if the word has the only tag
							if ((*iter)->tags.size() == 1)
								item.tag = static_cast<enum WordTag>((*iter)->tags[0].tag);

							item.is_recorded = true;
							item.offset = atom.offset;
							item.length = word_length;

							if (item.length == atom.length)
							{
								//	refine the atom.
								out_table[atom.offset].at(0) = item;
							}else{
								//	add new word to both offset array and wordlist
								out_table[atom.offset].push_back(item);
							}
						}
					}
				}else{
					//	not recorded
					get_special_word_info(dict, out_table[atom.offset].at(0));
				}
			}
		}

		///	Input:	text, dict, out_table
		///	Output:	graph_list
		static void create_graphs(const wstring& text, const Dictionary& dict, out_table_type& out_table, graph_list_type& sub_graphs)
		{
			//	split candidate graph (out_table) into several sub-graphs.
			//	The split point should be the node with multiple out-edges, and no edge cross over the node.
			out_table_type::iterator prev_iOut = out_table.begin();
			size_t max_offset = 0;
			for (out_table_type::iterator iOut = out_table.begin(); iOut != out_table.end(); ++iOut)
			{
				//	Get the next iterator
				out_table_type::iterator iNext = iOut;
				++iNext;

				if (iOut != out_table.begin()	//	never split the first node
					&& max_offset == iOut->first	//	no over edge
					&& iOut->second.size() == 1	//	current offset only have a single word
					&& (iNext != out_table.end() && iNext->second.size() > 1)	//	next offset have more than one words.
					)
				{
					//	multiple out-edges, and no over edge, so split here
					create_graph(text, dict, out_table, prev_iOut, iOut, sub_graphs);
					prev_iOut = iOut;
				}

				for (std::vector<WordInformation>::iterator iWord = iOut->second.begin(); iWord != iOut->second.end(); ++iWord)
				{
					size_t target_offset = iWord->offset + iWord->length;
					if (target_offset > max_offset)
						max_offset = target_offset;
				}
			}
			//	reach the last node
			create_graph(text, dict, out_table, prev_iOut, out_table.end(), sub_graphs);
		}

		///	Input:	text, dict, out_table, (begin, end)
		///	Output:	graph
		static void create_graph(const wstring& text, const Dictionary& dict, 
			out_table_type& out_table, 
			out_table_type::iterator begin, out_table_type::iterator end,
			graph_list_type& sub_graphs)
		{
			//	create graph
			size_t word_count = 0;
			for (out_table_type::iterator iter = begin; iter != end; ++iter)
			{
				word_count += iter->second.size();
			}

			if (begin == out_table.begin())
				++word_count;	//	One more node for [Begin] for first sub-graph

			++word_count;	//	One more node for the last node, it will be either end, or [End] of whole graph

			shared_ptr<WordGraph> graph_ptr(new WordGraph(word_count));
			WordGraph& graph = *graph_ptr;

			//	attach vertex information
			property_map<WordGraph, vertex_desc_t>::type
				vprop_map = get(vertex_desc, graph);

			size_t current_index = 0;
			//		[Begin]
			if (begin == out_table.begin())
			{
				//	put [begin] to graph as the first node
				WordInformation word_begin;
				word_begin.tag = WORD_TAG_BEGIN;
				//	the index of other nodes should increase one, since [Begin] is insert into the first one.
				word_begin.index = current_index++;
				word_begin.entry = 0;
				get_special_word_info(dict, word_begin);
				vprop_map[word_begin.index] = word_begin;
			}
			//		Internal node
			for (out_table_type::iterator iter = begin; iter != end; ++iter)
			{
				for (std::vector<WordInformation>::iterator it = iter->second.begin(); it != iter->second.end(); ++it)
				{
					size_t index = current_index++;
					vprop_map[index] = *it;
					vprop_map[index].index = index;
					it->index = index;
				}
			}
			//		[End]
			if (end == out_table.end()) {
				//	put [End] to graph as the last node
				WordInformation word_end;
				word_end.tag = WORD_TAG_END;
				word_end.index = current_index++;
				word_end.offset = text.size();
				word_end.entry = 0;
				get_special_word_info(dict, word_end);
				vprop_map[word_end.index] = word_end;
			}else{
				//	put 'end' to graph as the last node.
				WordInformation& word_end = end->second.front();
				word_end.index = current_index++;
				vprop_map[word_end.index] = word_end;
				vprop_map[word_end.index].index = word_end.index;
			}

			//	adding edges
			property_map<WordGraph, vertex_index_t>::type
				vindex_map = get(vertex_index, graph);

			for (size_t i = 0; i < num_vertices(graph) - 1; ++i)
			{
				WordInformation prop = vprop_map[i];
				size_t next_offset = prop.offset + prop.length;

				//	add all edges begin from the end of current word
				if (next_offset < text.size())
				{
					out_table_type::iterator iter_out_next = out_table.find(next_offset);
					std::vector<WordInformation>& next_wordlist = iter_out_next->second;
					for (std::vector<WordInformation>::iterator iter = next_wordlist.begin(); iter != next_wordlist.end(); ++iter)
					{
						add_edge_to_graph(prop, *iter, graph, text);
					}
				}else{
					//	next_offset == text.size()
					WordInformation& prop_end = vprop_map[num_vertices(graph)-1];
					add_edge_to_graph(prop, prop_end, graph, text);
				}
			}

			//	graph terminal
			graph_property<WordGraph, graph_terminal_t>::type&
				gterminal = get_property(graph, graph_terminal);

			gterminal.first = 0;
			if (end == out_table.end()) {
				gterminal.second = num_vertices(graph) - 1;
			}else{
				gterminal.second = end->second.front().index;
			}

			sub_graphs.push_back(graph_ptr);
		}

		static void add_edge_to_graph(const WordInformation& prop, const WordInformation& prop_next, WordGraph& graph, const std::wstring& text)
		{
			std::wstring next_word;
			if (prop_next.is_recorded)
				next_word = text.substr(prop_next.offset, prop_next.length);
			else
				next_word = get_special_word_string(prop_next.tag);

			double adjacency_weight = 0;
			if (prop.entry)
				adjacency_weight = prop.entry->get_forward_weight(next_word);

			double weight = calculate_transit_weight(prop.weight, adjacency_weight);
			
			if (!prop.is_recorded)
			{
				weight += prop.weight;
			}

			//	add the edge with weight
			add_edge(prop.index, prop_next.index, weight, graph);
		}

		///	Calculate the possibility
		///	0 < smoothing < 1
		///		A = smoothing * P(Ci-1)
		///		B = (1-smoothing) * P(Ci|Ci-1)
		///		frequency = - Log( A + B );
		static double calculate_transit_weight(double current_weight, double adjacency_weight, double smoothing = 0.1)
		{
			double P1 = (1 + current_weight) / (MAX_FREQUENCE+80000);
			double A = smoothing * P1;

			double t = 1/(double)MAX_FREQUENCE;
			double P2 = (((1-t) * adjacency_weight) / (1+current_weight)) + t;
			double B = (1 - smoothing) * P2;

			double weight = - ::log( A + B );
			return weight;
		}

		static WordInformation create_word(enum SymbolType type, size_t offset, size_t length)
		{
			WordInformation word;
			word.offset = offset;
			word.length = length;
			word.is_recorded = true;
			if (type != SYMBOL_TYPE_CHINESE)
			{
				word.weight = 0;
				word.is_recorded = false;
				switch(type){
					case SYMBOL_TYPE_INDEX:
					case SYMBOL_TYPE_NUMBER:
						//	number
						word.tag = WORD_TAG_M;
						break;
					case SYMBOL_TYPE_LETTER:
					case SYMBOL_TYPE_SINGLE:
						//	nouns of english, or etc.
						word.tag = WORD_TAG_NX;
						break;
					case SYMBOL_TYPE_PUNCTUATION:
						//	punctuation
						word.tag = WORD_TAG_W;
						break;
					default:
						word.tag = WORD_TAG_UNKNOWN;
						word.is_recorded = true;
						break;
				}
			}
			return word;
		}
		static std::vector<std::vector<path_type> > get_overall_k_shortest_path(std::vector<std::vector<path_type> >& subgraph_path_lists, int k = 1)
		{
			std::vector<std::vector<path_type> > overall_k_shortest_paths;
			size_t subgraph_count = subgraph_path_lists.size();
			std::vector<size_t> index_list(subgraph_count, 0);

			for (int a = 0; a < k; ++a)
			{
				double min_weight = std::numeric_limits<double>::max();
				size_t min_index = 0;
				bool found_next = false;
				//	fill the j-th best path in
				std::vector<path_type> path_list;
				for (size_t i = 0; i < subgraph_count; ++i)
				{
					size_t index = index_list[i];
					path_list.push_back(subgraph_path_lists[i][index]);
					//	find which sub-graph contains the next candidate path.
					if ((index+1) < subgraph_path_lists[i].size() && subgraph_path_lists[i][index+1].weight < min_weight)
					{
						min_weight = subgraph_path_lists[i][index+1].weight;
						min_index = i;
						found_next = true;
					}
				}
				overall_k_shortest_paths.push_back(path_list);
				//	if nothing found, then there is no more candidates path.
				if (found_next) {
					++index_list[min_index];
				}else{
					break;
				}
			}
			return overall_k_shortest_paths;
		}

	};
}
//	_OPENCLAS_SEGMENT_HPP_
#endif
