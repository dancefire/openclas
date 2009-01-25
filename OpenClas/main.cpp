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

#include <iostream>
#include <fstream>
#include <iomanip>

#include <sstream>

#include <hash_map>
#include <map>
#include <string>
#include <xhash>
#include <ctime>

int dict_findallmatches_loop_counter = 0;
int dict_findallmatches_wasteloop_counter = 0;
int dict_findallmatches_counter = 0;
int dict_unigram_counter = 0;
int dict_bigram_counter = 0;

#include "graph.h"

//#include "k_shortest_path.h"
#include "myKbest.h"

#include "dictionary.h"
#include "utility.h"
#include "Segment.h"
#include "ict_serializer.h"
#include "openclas_serializer.h"


#include "tag.h"

using namespace std;
using namespace ictclas;

void output_graphviz_header(const string header, ostream& out)
{
	out << "digraph " << header << "{" << endl;
	out << "\tgraph [ rankdir=LR ]" <<endl;
	out << "\tsize=\"30,20\"" << endl;
	out << "\tnode [ shape = doublecircle fontname=\"simhei.ttf\"];" << endl;
	out << "\tedge [ fontname=\"simsun.ttc\"];" << endl;
}

struct SegmentPerformanceCounter{
	int word_count;
	int atom;
	int unigram;
	int bigram;
	int n_shortest_path;
};

void test_segment_silent(const wstring& sentence, Dictionary& dict, SegmentPerformanceCounter& spc)
{
	clock_t tick;
	spc.word_count += (int)sentence.length();
	//	原子切分
	tick = clock();
	Segment<double> seg;
	vector<Atom> atom_list;
	seg.AtomSegment(sentence, atom_list);
	spc.atom += clock() - tick;
	//	形成一元图
	tick = clock();
	Segment<double>::word_graph_t word_graph((int)atom_list.size());
	seg.Connect(atom_list, word_graph, dict);
	spc.unigram += clock() - tick;
	//	形成二元图
	tick = clock();
	Segment<double>::word_index_t word_index_table;
	word_graph.sort();
	map< int, Segment<double>::word_t> index_word_table;
	for(Segment<double>::word_graph_t::edge_iterator iter = word_graph.begin(); iter != word_graph.end(); ++iter)
	{
		int index = (int)word_index_table.size();
		word_index_table[pair<int,int>(iter->begin, iter->end)] = index;
		index_word_table[index] = *iter;
	}
	Segment<double>::word_graph_t adjacency_word_graph( (int)word_index_table.size());
	seg.GenerateAdjacencyWordGraph(dict, word_graph, word_index_table, adjacency_word_graph, 0.1);
	spc.bigram += clock() - tick;
	//	计算N组最短路径
	tick = clock();
	typedef KShortestPath<double, Segment<double>::word_graph_t>  ksp_t;
	ksp_t k_shortest_path(adjacency_word_graph);
	ksp_t::result_t result;
	k_shortest_path.Find(1, result);
	spc.n_shortest_path += clock() - tick;
}
void test_segment(const wstring& sentence, Dictionary& dict, const string& basename)
{
	locale loc(Encoding::get_codepage_string(Encoding::GBK));
	cout.imbue(loc);
	cout << "==========================  Test Begin ===========================" << endl;
	Segment<double> seg;
	vector<Atom> atom_list;
	//	原始句子
	cout << "原始句子" << endl;
	//Encoding::setlocale(Encoding::GBK);
	cout << Encoding::to_string(sentence, loc) <<endl << endl;

	//	原子切分后
	cout << "原子切分后 Segment::AtomSegment()" << endl;
	cout << "\t" << endl;
	seg.AtomSegment(sentence, atom_list);
	int index = 0;
	ofstream of_atom((basename + string("_atom.dot.txt")).c_str());
	output_graphviz_header("AtomSegment", of_atom);
	for(vector<Atom>::iterator iter = atom_list.begin(); iter != atom_list.end(); ++iter)
	{
		++index;
		of_atom << "\tAtom" << index << " [ label = \"" << Encoding::to_string(iter->word, loc) << "\" ];" << endl;
		cout << index << "\t" << Encoding::to_string(iter->word, loc) << "(" << iter->type << ")" << endl;
	}
	of_atom << "}" << endl;
	of_atom.close();
	cout << endl << endl;

	//	形成一元图
	ofstream of_unigram((basename + string("_unigram.dot.txt")).c_str());
	output_graphviz_header("UnigramGraph", of_unigram);

	Segment<double>::word_graph_t word_graph((int)atom_list.size());
	cout << "形成一元图 Segment::Connection()" << endl;
	seg.Connect(atom_list, word_graph, dict);
	for(Segment<double>::word_graph_t::edge_iterator iter = word_graph.begin(); iter != word_graph.end(); ++iter)
	{
		of_unigram << "\t" << iter->begin << " -> " << iter->end << " [ label = \"" << Encoding::to_string(iter->word, loc) << "\" ];" << endl;
		//of_unigram << "\tUnigram" << index++ << " [ label = \"" << Encoding::to_string(iter->word, loc) << "\" ];" << endl;
		cout << "\t" << dec<< "(" << iter->begin << "," << iter->end << ")\t" << " Freq:" << iter->weight << ",\tTag:" << iter->tag << "\t" << Encoding::to_string(iter->word, loc) << " " << endl;
	}
	of_unigram << "}" << endl;
	of_unigram.close();
	cout << endl << endl;

	//	形成二元图

	cout << "形成二元图 Segment::GenerateAdjacencyWordGraph()" << endl;
	Segment<double>::word_index_t word_index_table;
	word_graph.sort();
	map< int, Segment<double>::word_t> index_word_table;
	for(Segment<double>::word_graph_t::edge_iterator iter = word_graph.begin(); iter != word_graph.end(); ++iter)
	{
		int index = (int)word_index_table.size();
		word_index_table[pair<int,int>(iter->begin, iter->end)] = index;
		index_word_table[index] = *iter;
	}
	Segment<double>::word_graph_t adjacency_word_graph( (int)word_index_table.size());
	seg.GenerateAdjacencyWordGraph(dict, word_graph, word_index_table, adjacency_word_graph, 0.1);

	ofstream of_bigram((basename + string("_bigram.dot.txt")).c_str());
	output_graphviz_header("BigramGraph", of_bigram);

	for(Segment<double>::word_graph_t::edge_iterator iter = adjacency_word_graph.begin(); iter != adjacency_word_graph.end(); ++iter)
	{
		of_bigram << "\t" << iter->begin << " -> " << iter->end << " [ label = \"" << Encoding::to_string(iter->word, loc) << "\", weight=" << iter->weight << " ];" << endl;
		cout << "\t" << dec<< "(" << iter->begin << "," << iter->end << ")\t" << " Freq:" << iter->weight << ",\tTag:" << iter->tag << "\t" << Encoding::to_string(iter->word, loc) << " " << endl;
	}
	of_bigram << "}" << endl;
	of_bigram.close();
	cout << endl << endl;

	//	计算N组最短路径

	cout << "计算N组最短路径 KShortestPath" << endl;

	typedef KShortestPath<double, Segment<double>::word_graph_t>  ksp_t;
	ksp_t k_shortest_path(adjacency_word_graph);
	ksp_t::result_t result;
	k_shortest_path.Find(3, result);
	for(ksp_t::result_t::iterator iter = result.begin(); iter != result.end(); ++iter)
	{
		cout << "[weight=" << iter->first << "] \t";
		for(ksp_t::path_id_list_t::iterator it = iter->second.begin(); it != iter->second.end(); ++it)
		{
			cout << *it << "(" << Encoding::to_string(index_word_table[*it].word, loc) << "), ";
		}
		cout << endl;
	}

	cout << "============================= Test End  =============================" << endl;
}

void test_k_shortest_path()
{
	typedef Edge<double> MyEdge;
	typedef KShortestPath<double> MyKShortestPath;

	AdjacencyList<double> g(7);
	g.add(Edge<double>(0, 1, 1));
	g.add(Edge<double>(1, 2, 1));
	g.add(Edge<double>(1, 3, 2));
	g.add(Edge<double>(2, 3, 1));
	g.add(Edge<double>(2, 4, 1));
	g.add(Edge<double>(3, 4, 1));
	g.add(Edge<double>(3, 6, 2));
	g.add(Edge<double>(4, 5, 1));
	g.add(Edge<double>(4, 6, 3));
	g.add(Edge<double>(5, 6, 1));

	for(AdjacencyList<double>::edge_iterator iter = g.begin(); iter != g.end() ; iter++)
	{
		cout << "[" << iter->begin << ", " << iter->end << "] (" << iter->weight << ")" << endl;
	}

	MyKShortestPath k_shortest_path(g);
	MyKShortestPath::result_t result;
	k_shortest_path.Find(3, result);
	/**************** debug output ***************/
	for(MyKShortestPath::result_t::iterator iter = result.begin(); iter != result.end(); ++iter)
	{
		cout << "[weight=" << iter->first << "] \t";
		for(MyKShortestPath::path_id_list_t::iterator it = iter->second.begin(); it != iter->second.end(); ++it)
		{
			cout << *it << ", ";
		}
		cout << endl;
	}
	/**************** debug finish ***************/
}

void test_ict_deserialize(Dictionary& dict, const char* filename)
{
	ICT_Serializer ictser;
	cout << "Begin reading coreDict.dct...";
	ifstream dict_file(filename, ios_base::in | ios_base::binary);
	if(dict_file.fail())
	{
		cerr << "Cannot open coreDict.dct for reading." << endl;
		return;
	}
	ictser.Deserialize(dict_file, dict);
	cout << "OK" << endl;
	dict_file.close();
}

void test_text_output_dictionary(Dictionary& dict, const char* outfile)
{
	cout << "Begin writing the dict to file...";
	ofstream new_dict(outfile, ios_base::out | ios_base::app);
	if(new_dict.fail())
	{
		cerr << "Cannot open '" << outfile << "' for write." << endl;
		return;
	}
	int count = 0;
	locale loc(Encoding::get_codepage_string(Encoding::GBK));
	cout.imbue(loc);
	new_dict.imbue(loc);
	Encoding::setlocale(Encoding::GBK);
	new_dict << endl << "Writing the Dictionary : " << endl << endl;
	map<int, int> tag_count;
	vector<string> tag_table;
	for(Dictionary::iterator iter = dict.begin(); iter != dict.end(); ++iter)
	{
		bool bLowTag = false;
		for(vector<WordTag>::iterator it = iter->second.tags.begin(); it != iter->second.tags.end(); ++it)
		{
			if (it->tag < 50)
			{
				bLowTag = true;
				break;
			}
		}
		//if(iter->first.find(L"#") != wstring::npos || bLowTag)
		//{
			//if(iter->second.tags.size() < 1 || iter->second.tags.at(0).tag <= 25)
			//if(iter->second.tags.size() < 1 || (iter->second.tags.at(0).tag < 20 || iter->second.tags.at(0).tag > 21) )
			//{
			//	continue;
			//}
			//cout << "Key: " << Encoding::to_string(iter->first) << "\t";
			//new_dict << "Key: " << Encoding::to_string(iter->first) << "\t ID=" << iter->second.id << endl;
			for(vector<WordTag>::iterator it = iter->second.tags.begin(); it != iter->second.tags.end(); ++it)
			{
				++tag_count[it->tag];
				//cout << "\t(Tag=" << it->tag << ", Frequency=" << it->frequency << ")" << endl;
				//new_dict << "Key: " << Encoding::to_string(iter->first) << "\t ID=" << setw(10) << iter->second.id << "\t(Tag=" << it->tag << ", Frequency=" << it->frequency << ")" << endl;

				ostringstream ss;
				ss << "[" << setw(3) << it->tag << "]" << "\t Key: " << Encoding::to_string(iter->first) << "\t ID=" << setw(10) << iter->second.id << "\t(Tag=" << it->tag << ", Frequency=" << it->frequency << ")";
				tag_table.push_back(ss.str());
			}
		//}
		//if ( (count++) % 100 == 0)
		//{
		//	cout << ".";
		//}
	}
	cout << "OK" << endl;

	sort(tag_table.begin(), tag_table.end());
	for(vector<string>::iterator iter = tag_table.begin(); iter != tag_table.end(); ++iter)
	{
		new_dict << *iter << endl;
	}

	new_dict << "Finished." << endl;
	new_dict.close();

	cout << "Tag Count:" << endl;
	for(map<int, int>::iterator iter = tag_count.begin(); iter != tag_count.end(); ++iter)
	{
		char tag = static_cast<char>(iter->first);
		if(tag >= 0 && tag <= 25){
			tag = 'A' + tag;
		}else{
			tag = '*';
		}
		cout << "Tag = " << tag << "(" << setw(3) << iter->first << "), Count = " << setw(5) << iter->second << endl;
	}


}

void test_dictionary(Dictionary& dict)
{
	dict.Add(WordEntry(L"中国"));
	WordEntry we1(L"中国");
	we1.Add(WordTag(1, 123));
	we1.Add(WordTag(2, 12));
	we1.Add(WordTag(5, 1523));
	dict.Add(we1);
	WordEntry we2(L"汉语");
	we2.Add(WordTag(15, 23));
	we2.Add(WordTag(12, 234));
	we2.Add(WordTag(5, 4212));
	dict.Add(we2);
	WordEntry we3(L"红烧肉");
	we3.Add(WordTag(15, 23));
	we3.Add(WordTag(12, 234));
	we3.Add(WordTag(5, 4212));
	dict.Add(we3);

	Encoding::setlocale(Encoding::GBK);

	cout << "(" << we1.id << ") [" << Encoding::to_string(we1.word) << "]" << endl;
	cout << "(" << we2.id << ") [" << Encoding::to_string(we2.word) << "]" << endl;
	cout << "(" << we3.id << ") [" << Encoding::to_string(we3.word) << "]" << endl;

	dict.AddAdjacencyFrequency(L"中国", L"汉语", 123);
	dict.AddAdjacencyFrequency(L"汉语", L"中国", 876);
	dict.AddAdjacencyFrequency(L"中国", L"红烧肉", 2007);


	int f1 = dict.GetAdjacencyFrequency(L"中国", L"汉语");
	int f2 = dict.GetAdjacencyFrequency(L"汉语", L"中国");

	cout << "f1=" << f1 << ", f2=" << f2 << endl;

	int f11 = dict.GetAdjacencyFrequency(1, 2);
	int f22 = dict.GetAdjacencyFrequency(2, 1);
	int f31 = dict.GetAdjacencyFrequency(3, 1);
	int f13 = dict.GetAdjacencyFrequency(1, 3);
	int f32 = dict.GetAdjacencyFrequency(3, 2);
	cout << "f11=" << f11 << ", f22=" << f22 << endl;
	cout << "f31=" << f31 << ", f13=" << f13 << ", f32=" << f32 << endl;
}

void dict_ict_to_open(int argc, const char** argv, Dictionary& dict)
{
	if(argc < 4){
		cerr << "Invalid argument. " << argc << endl;
		return;
	}

	const char* ict_word_dict = argv[1];
	const char* ict_adjacency_dict = argv[2];
	const char* openclas_dict = argv[3];

	cout << "Reading ICTCLAS dictionary file [" << ict_word_dict << "]" << endl;
	ifstream ict_dict_stream(ict_word_dict, ios_base::in | ios_base::binary);
	ICT_Serializer ictserial;


	Encoding::clear_tick();
	clock_t tick = clock();
	ictserial.Deserialize(ict_dict_stream, dict);
	cout << endl << "Successfully read ICTCLAS dictionary." << "Time:(" << (clock() - tick) << ")" << endl;
	ict_dict_stream.close();
	Encoding::output_tick();

	cout << "Reading ICTCLAS Bigram (adjacency word) dictionary. [" << ict_adjacency_dict << "]" << endl << endl;
	ifstream ict_adj_stream(ict_adjacency_dict, ios_base::in | ios_base::binary);
	tick = clock();
	Encoding::clear_tick();
	ictserial.Deserialize(ict_adj_stream, dict, true);
	cout << endl << "Successfully read ICTCLAS Bigram (adjacency word) dictionary." << (clock() - tick) << ")" << endl << endl;
	Encoding::output_tick();
	ict_adj_stream.close();

	cout << "Writing the OpenClas dictionary file.[" << openclas_dict << "]" << endl;
	ofstream openclas_stream(openclas_dict, ios_base::out | ios_base::binary);
	OpenClasSerializer ocs;
	tick = clock();
	ocs.Serialize(openclas_stream, dict);
	cout << endl << "Successfully write OpenClas dictionary file. Time:(" << (clock() - tick) << ")" << endl << endl;
	openclas_stream.close();

	Dictionary confirm_dict;
	cout << "Read the OpenClas dictionary file for confirm. [" << openclas_dict << "]" << endl;
	ifstream i_openclas_stream(openclas_dict, ios_base::in | ios_base::binary);
	ocs.Deserialize(i_openclas_stream, confirm_dict);
	tick = clock();
	cout << endl << "Successfully read OpenClas dictionary file. Time:(" << (clock() - tick) << ")" << endl << endl;
	i_openclas_stream.close();

	cout << "Writing the OpenClas dictionary file.[" << openclas_dict << "]" << endl;
	ofstream oos("double_check.dict", ios_base::out | ios_base::binary);
	tick = clock();
	ocs.Serialize(oos, confirm_dict);
	cout << endl << "Successfully write OpenClas dictionary file. Time:(" << (clock() - tick) << ")" << endl << endl;
	oos.close();


	if(dict.Size() == confirm_dict.Size())
	{
		cout << "The Word dictionary is correct read same size. (" << confirm_dict.Size() << ")" << endl;
	}else{
		cout << "The word dictionary is different. Original: " << dict.Size() << ", Read: " << confirm_dict.Size() << endl;
	}

	if(dict.AdjacencySize() == confirm_dict.AdjacencySize())
	{
		cout << "The Adjacency dictionary is correct read same size. (" << confirm_dict.AdjacencySize() << ")" << endl;
	}else{
		cout << "The Adjacency dictionary is different. Original: " << dict.AdjacencySize() << ", Read: " << confirm_dict.AdjacencySize() << endl;
	}
}

//
//BOOL CALLBACK EnumLocalesProc(
//  LPTSTR lpLocaleString 
//)
//{
//	wstring str(lpLocaleString);
//	Encoding::setlocale(Encoding::GBK);
//	cout << Encoding::to_string(str) << endl;
//	return true;
//}
//
//BOOL CALLBACK EnumCodePagesProc(
//  LPTSTR lpCodePageString   
//)
//{
//	Encoding::setlocale(Encoding::GBK);
//	cout << Encoding::to_string(lpCodePageString) << endl;
//	return true;
//}


void test_hash_value(Dictionary& dict)
{
	map<size_t, int> hash_value_table;

	cout << endl;

	Encoding::setlocale(Encoding::GBK);
	for(Dictionary::iterator iter = dict.begin(); iter != dict.end(); ++iter)
	{
		size_t hash_v = stdext::hash_value(iter->first);
		hash_value_table[hash_v]++;
		//	hacked
		if (hash_v == 0x98e9dda8) {
			cout << "Collision word: '" << Encoding::to_string(iter->first) << "', Hash value = 0x" << hex << hash_v << endl;
		}
	}
	cout << endl;

	map<int, int> stats_table;

	for(map<size_t, int>::iterator iter = hash_value_table.begin(); iter != hash_value_table.end(); ++iter)
	{
		if (iter->second > 1){
			cout << "The hash value's collision is more than 1. Hash value = 0x" << hex<< iter->first << ", count = " << dec << iter->second << endl;
		}
		stats_table[iter->second]++;
	}
	cout << endl;

	cout << "Hash value stats:" << endl;
	for(map<int, int>::iterator iter = stats_table.begin(); iter != stats_table.end(); ++iter)
	{
		cout << "\t Hash value collision = " << iter->first << ",\t the count of the hash_value = " << dec << iter->second << endl;
	}



	map<size_t, int> pair_hvtable;
	for(Dictionary::adjacency_iterator iter = dict.adjacency_begin(); iter != dict.adjacency_end(); ++iter)
	{
		size_t hash_v = stdext::hash_value(iter->first);
		pair_hvtable[hash_v]++;
	}
	cout << endl;

	map<int, int> pair_stats_table;

	for(map<size_t, int>::iterator iter = pair_hvtable.begin(); iter != pair_hvtable.end(); ++iter)
	{
		if (iter->second > 1){
//			cout << "The hash value's collision is more than 1. Hash value = 0x" << hex<< iter->first << ", count = " << dec << iter->second << endl;
		}
		pair_stats_table[iter->second]++;
	}
	cout << endl;

	cout << "Hash value stats:" << endl;
	for(map<int, int>::iterator iter = pair_stats_table.begin(); iter != pair_stats_table.end(); ++iter)
	{
		cout << "\t Hash value collision = " << iter->first << ",\t the count of the hash_value = " << dec << iter->second << endl;
	}
}
void test_read_openclas_dict(int argc, const char* argv[], Dictionary& dict){
	if(argc >= 2){
		OpenClasSerializer ocs;

		const char* filename = argv[1];
		cout << "Read the OpenClas dictionary file. [" << filename << "]" << endl;
		ifstream iopenstream(filename, ios_base::in | ios_base::binary);
		clock_t tick = clock();
		ocs.Deserialize(iopenstream, dict);
		cout << endl << "Successfully read OpenClas dictionary file. Time:(" << (clock() - tick) << ")" << endl << endl;
		iopenstream.close();
		cout << "The Word dictionary size is " << dict.Size() << " records." << endl;
		cout << "The Adjacency dictionary size is " << dict.AdjacencySize() << " records." << endl;

	}
}

void test_sentence_segment(Dictionary& dict)
{
	const wchar_t* sample[] = {
		L"19９5年底ｇoｏgｌｅ在1月份大会上说的确实在理。",
		L"这个门的把手坏了好几天了",
		L"你把手抬高一点儿",
		L"这个动作的要领其实很简单",
		L"下午我要领工资，恐怕赶不回去",
		L"办独生子女证，一对夫妻一次性交一百元钱",
		L"我知道你不知道我知道你不知道我知道你不知道",
		L"已经分房和尚未分房的同志",
		L"馆内陈列着周恩来和邓颖超生前使用过的物品"
	};
	const wchar_t* sample2 = L"19９5年底ｇoｏgｌｅ在1月份大会上说的确实在理。这个门的把手坏了好几天了。你把手抬高一点儿。这个动作的要领其实很简单。下午我要领工资，恐怕赶不回去。办独生子女证，一对夫妻一次性交一百元钱。我知道你不知道我知道你不知道我知道你不知道。已经分房和尚未分房的同志。";
	try{
		SegmentPerformanceCounter spc = SegmentPerformanceCounter();
		clock_t tick = clock();
		for(int count = 0; count < 4*1000; ++count)
		{
			//test_segment_silent(sample2, dict, spc);
			for(int i = 0; i < 9; ++i)
			{
				test_segment_silent(sample[i], dict, spc);
				//string base("sentence");
				//base += static_cast<char>('0' + i);
				//test_segment(sample[i], dict, base);
			}
		}
		double total = (clock() - tick);
		cout << endl;
		cout << "Summary:" << endl << endl;
		cout << "Total time: " << total << " ms" << endl;
		cout << "Wordcount: " << spc.word_count << " wchar_t" << "\t(" << spc.word_count * sizeof(wchar_t) << " bytes)" << endl;
		cout << "Performance: " << static_cast<double>(spc.word_count * sizeof(wchar_t)/total) << " KB/s" << endl;
		cout << endl;
		cout << "\tAtom: " << spc.atom << " ms" << endl;
		cout << "\tUnigram: " << spc.unigram << " ms" << endl;
		cout << "\t  Dict::Unigram find all prefix matches: " << dict_findallmatches_counter << " ms" << endl;
		//cout << "\t  Dict::Unigram waste loop against to TST: " << dict_findallmatches_wasteloop_counter << endl;
		//cout << "\t  Dict::Unigram loop should be: " << dict_findallmatches_loop_counter << endl;
		//cout << "\t  Dict::Unigram possible minimized to : " << dict_findallmatches_counter / (dict_findallmatches_wasteloop_counter/dict_findallmatches_loop_counter) << " ms" << endl;
		cout << "\t    Dict::Unigram find: " << dict_unigram_counter << " ms" << endl;
		cout << "\tBigram: " << spc.bigram << " ms" << endl;
		cout << "\t  Dict::Bigram find: " << dict_bigram_counter << " ms" << endl;
		cout << "\tNShortestPath: " << spc.n_shortest_path << " ms" << endl;
	}catch(std::exception e){
		cerr << endl << "Exception: " << e.what() << endl;
	}

}

void test_stats_dict(Dictionary& dict)
{
	int zero_freq_item = 0;
	int zero_freq_word = 0;
	map<int, int> length_count;
	for(Dictionary::iterator iter = dict.begin(); iter != dict.end(); ++iter)
	{
		WordEntry& we = iter->second;
		
		bool zero_freq = false;
		for(vector<WordTag>::iterator it = we.tags.begin(); it != we.tags.end(); ++it)
		{
			if(it->frequency == 0){
				++zero_freq_item;
				zero_freq = true;
			}
		}
		if(zero_freq){
			++zero_freq_word;
		}

		++length_count[static_cast<int>(we.word.length())];
	}

	//	output
	cout << "Found tag with 0 frequency: " << zero_freq_item << endl;
	cout << "Found word contain 0 frequency tag: " << zero_freq_word << endl;
	cout << endl << "The count by length: " << endl;
	for(map<int, int>::iterator iter = length_count.begin(); iter != length_count.end(); ++iter)
	{
		cout << "Length = " << iter->first << "\t Count = " << iter->second << endl;
	}
}

void test_openclas()
{
	openclas::Graph<int, int> g(10);
	g.add_edge(1,2);
	g.add_edge(2,3);
	g.add_edge(3,9);
	g.add_edge(3,4);
	g.add_edge(2,4);
}

int main(int argc, const char** argv)
{
	test_openclas();
	//test_k_shortest_path();

	//Dictionary dict1;
	//test_dictionary(dict1);

	//	Serialization
	//Dictionary mydict;
	//test_ict_deserialize(mydict, "coreDict.dct");
	//test_text_output_dictionary(mydict, "newdict.txt");

	//cout << static_cast<short>('U'-'A') << ", " << static_cast<short>('V'-'A') << endl;
	//cout << static_cast<char>('A'+44) << endl;
	Dictionary dict;

	//dict_ict_to_open(argc, argv);
	test_read_openclas_dict(argc, argv, dict);

	//	test hash_value
	//test_hash_value(dict);
	
	//	test sentences segment
	test_sentence_segment(dict);

	//test_text_output_dictionary(dict, "output.txt");
	
	//test_stats_dict(dict);

	//char* loc_name = setlocale(LC_ALL, ".65001");
	//if(loc_name)
	//	cout << "hello " << loc_name << endl;
	//EnumSystemLocales(EnumLocalesProc, LCID_INSTALLED);
	//EnumSystemCodePages(EnumCodePagesProc,CP_INSTALLED);
	return 0;
}