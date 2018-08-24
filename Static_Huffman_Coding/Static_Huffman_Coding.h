#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <queue>
#include <vector>
#include <bitset>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

class Static_Huffman_Coding {
private:
	map<char, int> dictionary;
	map<char, string> Code;
	size_t memory_use;
	int max_codelen;
	string rest_bits;
	bool dynamic;

	struct HuffmanTreeNode {
		char c;
		int freq;

		HuffmanTreeNode *left;
		HuffmanTreeNode *right;
		HuffmanTreeNode(char c, int freq) {
			this->left = NULL;
			this->right = NULL;
			this->c = c;
			this->freq = freq;
		}
	};

	struct compare {
		bool operator()(HuffmanTreeNode* left, HuffmanTreeNode* right)

		{
			return (left->freq > right->freq);
		}
	};

	priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, compare> HuffmanTree;

	void countChar(char *buffer, int buffer_size);
	void freq_count(string inputfile);
	void code_char(HuffmanTreeNode *HuffmanTree, string prefix);
	void header_encode(ofstream& Huffman_output);
	void envolope(string inputfile, ofstream& Huffman_output);
	void headerRead(ifstream& fin);
	void HuffmanTreeBuilder();
	void Max_codelen();
	int HuffmanTreeheight(HuffmanTreeNode* root);
	void content_decode(ifstream& fin, const int filelen, ofstream& Huffman_output);
	int content_search(ifstream& fin, const int filelen, string obj);

public:
	Static_Huffman_Coding(size_t memory_use, bool dyna);
	~Static_Huffman_Coding();

	void encode(string inputfile, string outputfile);
	void decode(string inputfile, string outputfile);
	int search(string inputfile, string obj);
};