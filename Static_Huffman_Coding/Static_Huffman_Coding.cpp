#include "Static_Huffman_Coding.h"

using namespace std;

void Static_Huffman_Coding::countChar(char *buffer, int buffer_size)
{
	for (int i = 0; i < buffer_size; i++) {
		try {
			this->dictionary[buffer[i]] ++;
		}
		catch (const exception& e) {
			this->dictionary[buffer[i]] = 1;
		}
	}
	
}

Static_Huffman_Coding::Static_Huffman_Coding(size_t memory_use, bool dyna)
{
	this->dynamic = dyna;
	this->memory_use = memory_use;
}

Static_Huffman_Coding::~Static_Huffman_Coding(){}

void Static_Huffman_Coding::freq_count(string inputfile)
{
	size_t memory_use = this->memory_use;
	ifstream fin(inputfile.c_str(), ifstream::ate | ifstream::binary);
	const int file_length = fin.tellg();
	if (memory_use == 0) memory_use = file_length;
	char *buffer = new char[memory_use];
	const int full_buffer = file_length / (int)memory_use;
	const int last_buffer = file_length % (int)memory_use;
	fin.seekg(0, fin.beg);
	int readtime = 0;
	while (fin)
	{
		fin.read(buffer, memory_use);
		size_t count = fin.gcount();
		if (!count)
			break;
		int Filledbuffer = readtime < full_buffer ? memory_use : last_buffer;
		readtime++;
		this->countChar(buffer, Filledbuffer);
	}
	delete[] buffer;
	fin.close();
}

void Static_Huffman_Coding::code_char(HuffmanTreeNode *HuffmanTree, string prefix)
{
	if (HuffmanTree == NULL) return;
	if (HuffmanTree->c != NULL) this->Code[HuffmanTree->c] = prefix;	
	code_char(HuffmanTree->left, prefix + "0");
	code_char(HuffmanTree->right, prefix + "1");
}

void Static_Huffman_Coding::header_encode(ofstream& Huffman_output)
{
	string header = "";
	if (this->dynamic) {
		for (map<char, int>::const_iterator it = this->dictionary.begin(); it != this->dictionary.end(); ++it) {
			if (it->second < 255) {
				string binary = bitset<8>(it->second).to_string();
				header += it->first;
				header += char(strtol(binary.c_str(), NULL, 2));
				header += char(strtol("00000000", NULL, 2));
			}
			else if (it->second < 65336) {
				string binary = bitset<16>(it->second).to_string();
				string high = binary.substr(0, 8);
				string low = binary.substr(8, 8);
				header += it->first;
				header += char(strtol(high.c_str(), NULL, 2));
				header += char(strtol(low.c_str(), NULL, 2));
				header += char(strtol("00000000", NULL, 2));
			}
			else if (it->second < 16777216) {
				string binary = bitset<24>(it->second).to_string();
				string high = binary.substr(0, 8);
				string med = binary.substr(8, 8);
				string low = binary.substr(16, 8);
				header += it->first;
				header += char(strtol(high.c_str(), NULL, 2));
				header += char(strtol(med.c_str(), NULL, 2));
				header += char(strtol(low.c_str(), NULL, 2));
				header += char(strtol("00000000", NULL, 2));
			}
			else {
				string binary = bitset<24>(it->second).to_string();
				string high = binary.substr(0, 8);
				string med1 = binary.substr(8, 8);
				string med2 = binary.substr(16, 8);
				string low = binary.substr(24, 8);
				header += it->first;
				header += char(strtol(high.c_str(), NULL, 2));
				header += char(strtol(med1.c_str(), NULL, 2));
				header += char(strtol(med2.c_str(), NULL, 2));
				header += char(strtol(low.c_str(), NULL, 2));
				header += char(strtol("00000000", NULL, 2));
			}
		}
	}
	else
	{
		string binary;
		for (map<char, int>::const_iterator it = this->dictionary.begin(); it != this->dictionary.end(); ++it) {
			binary = bitset<16>(it->second).to_string();
			string high = binary.substr(0, 8);
			string med = binary.substr(8, 8);
			string low = binary.substr(16, 8);
			header += it->first;
			header += char(strtol(high.c_str(), NULL, 2));
			header += char(strtol(med.c_str(), NULL, 2));
			header += char(strtol(low.c_str(), NULL, 2));
		}
		if (header.length() == 1024) header = header.substr(0, 1021);
		string high = binary.substr(0, 8);
		string low = binary.substr(8, 8);
		header += char(strtol(high.c_str(), NULL, 2));
		header += char(strtol(low.c_str(), NULL, 2));
	}
	
	Huffman_output << header;
	for (int i = header.length(); i < 1023; i++) {
		Huffman_output << char(strtol("00000000", NULL, 2));
	}
}

void Static_Huffman_Coding::envolope(string inputfile, ofstream& Huffman_output)
{
	size_t memory_use = this->memory_use;
	ifstream fin(inputfile.c_str(), ifstream::ate | ifstream::binary);
	const int file_length = fin.tellg();
	if (memory_use == 0) memory_use = file_length;
	char *buffer = new char[memory_use];
	const int full_buffer = file_length / (int)memory_use;
	const int last_buffer = file_length % (int)memory_use;
	fin.seekg(0, fin.beg);
	int readtime = 0;
	char *newchar = new char[8];
	int chari = 0;
	while (fin)
	{
		fin.read(buffer, memory_use);
		size_t count = fin.gcount();
		if (!count)
			break;
		int Filledbuffer = readtime < full_buffer ? memory_use : last_buffer;
		readtime++;
		for (int i = 0; i < Filledbuffer; i++) {
			string Huffman_code = this->Code[buffer[i]];
			for (int j = 0; j < Huffman_code.length(); j++) {				
				newchar[chari] = Huffman_code[j];
				++chari;
				if (chari == 8) {
					chari = 0;
					Huffman_output << char(strtol(newchar, NULL, 2));
				}
			}
		}
	}
	int remainder = 0;
	if (chari > 0) {
		remainder = 8 - chari;
		for (int i = chari; i < 8; i++) {
			newchar[i] = '0';
		}
		Huffman_output << char(strtol(newchar, NULL, 2));
	}
	Huffman_output << char(remainder + '0');
	delete[] buffer;
	fin.close();
}

void Static_Huffman_Coding::headerRead(ifstream& fin)
{
	char *buffer = new char[1023];
	fin.read(buffer, 1023);
	int readIndex = 0;
	char currKey;
	bool isKey = true;
	while (readIndex < 1023) {
		if (isKey) {
			currKey = buffer[readIndex++];
			if ((!this->dynamic) && currKey == NULL) break;
			isKey = false;
		}
		else
		{
			int decimal = 0;
			if (this->dynamic) {
				if (buffer[readIndex] == NULL) break;
				string value;
				int strlen = 0;
				while (buffer[readIndex] != NULL) {
					value += buffer[readIndex++];
					++strlen;
				}
				if (strlen == 1) {
					decimal = (int)bitset<8>(value[0]).to_ulong();
				}
				else if (strlen == 2) {
					string high = bitset<8>(value[0]).to_string();
					string low = bitset<8>(value[1]).to_string();
					decimal = (int)bitset<16>(high + low).to_ulong();
				}
				else if (strlen == 3) {
					string high = bitset<8>(value[0]).to_string();
					string med = bitset<8>(value[1]).to_string();
					string low = bitset<8>(value[2]).to_string();
					decimal = (int)bitset<16>(high + med + low).to_ulong();
				}
				else {
					string high = bitset<8>(value[0]).to_string();
					string med1 = bitset<8>(value[1]).to_string();
					string med2 = bitset<8>(value[2]).to_string();
					string low = bitset<8>(value[3]).to_string();
					decimal = (int)bitset<16>(high + med1 + med2 + low).to_ulong();
				}
				++readIndex;
			}
			else
			{
				if (readIndex == 1221) {
					string high = bitset<8>(buffer[readIndex++]).to_string();
					string low = bitset<8>(buffer[readIndex++]).to_string();
					decimal = (int)bitset<16>(high + low).to_ulong();
				}
				else {
					string high = bitset<8>(buffer[readIndex++]).to_string();
					string med = bitset<8>(buffer[readIndex++]).to_string();
					string low = bitset<8>(buffer[readIndex++]).to_string();
					decimal = (int)bitset<16>(high + med + low).to_ulong();
				}
				
			}
			this->dictionary[currKey] = decimal;
			isKey = true;		
		}
	}
	this->HuffmanTreeBuilder();
}

void Static_Huffman_Coding::HuffmanTreeBuilder()
{
	priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, compare> HuffmanTree;
	for (map<char, int>::const_iterator it = this->dictionary.begin(); it != this->dictionary.end(); ++it) {
		HuffmanTree.push(new HuffmanTreeNode(it->first, it->second));
	}
	while (HuffmanTree.size() > 1)
	{
		HuffmanTreeNode *leftNode = HuffmanTree.top();
		HuffmanTree.pop();
		HuffmanTreeNode *rightNode = HuffmanTree.top();
		HuffmanTree.pop();
		HuffmanTreeNode *currNode = new HuffmanTreeNode(NULL, leftNode->freq + rightNode->freq);
		currNode->left = leftNode;
		currNode->right = rightNode;
		HuffmanTree.push(currNode);
	}
	this->HuffmanTree = HuffmanTree;
}

void Static_Huffman_Coding::Max_codelen()
{
	this->max_codelen = this->HuffmanTreeheight(this->HuffmanTree.top());
}

int Static_Huffman_Coding::HuffmanTreeheight(HuffmanTreeNode * root)
{
	if (!root) return 0;
	int leftHeight = HuffmanTreeheight(root->left);
	int rightHeight = HuffmanTreeheight(root->right);
	return max(leftHeight, rightHeight) + 1;
}

void Static_Huffman_Coding::content_decode(ifstream & fin, const int filelen, ofstream& Huffman_output)
{
	char *buffer = new char[this->memory_use];
	const int full_buffer_times = filelen / (int)this->memory_use;
	const int last_buffer = filelen % (int)this->memory_use;
	int readtime = 0;
	HuffmanTreeNode *root = this->HuffmanTree.top();
	HuffmanTreeNode *curr = root;
	string decode_queue;
	while (fin) {
		fin.read(buffer, memory_use);
		size_t count = fin.gcount();
		if (!count) break;
		int Filledbuffer = readtime < full_buffer_times ? memory_use : last_buffer;
		if (readtime == full_buffer_times) {
			for (int i = 0; i < Filledbuffer - 2; i++) {
				decode_queue = bitset<8>(buffer[i]).to_string();
				for (int j = 0; j < 8; j++) {
					if (decode_queue[j] == '0') curr = curr->left;
					else curr = curr->right;
					if (curr->c != NULL) {
						Huffman_output << curr->c;
						curr = root;
					}
				}
			}
			decode_queue = bitset<8>(buffer[Filledbuffer - 2]).to_string();	
			int lastbits = (int)buffer[Filledbuffer - 1] - '0';
			for (int i = 0; i < 8 - lastbits; i++) {
				if (decode_queue[i] == '0') curr = curr->left;
				else curr = curr->right;
				if (curr->c != NULL) {
					Huffman_output << curr->c;
					curr = root;
				}
			}
		}
		else {
			for (int i = 0; i < Filledbuffer; i++) {
				decode_queue = bitset<8>(buffer[i]).to_string();
				for (int j = 0; j < 8; ++j) {
					if (decode_queue[j] == '0') curr = curr->left;
					else curr = curr->right;
					if (curr->c != NULL) {
						Huffman_output << curr->c;
						curr = root;
					}
				}
			}
		}
		
		readtime++;
	}
	fin.close();
	Huffman_output.close();
}

int Static_Huffman_Coding::content_search(ifstream & fin, const int filelen, string obj)
{
	int total_find = 0;
	char *buffer = new char[this->memory_use];
	const int full_buffer_times = filelen / (int)this->memory_use;
	const int last_buffer = filelen % (int)this->memory_use;
	int readtime = 0;
	HuffmanTreeNode *root = this->HuffmanTree.top();
	HuffmanTreeNode *curr = root;
	string decode_queue;
	string last_record;
	while (fin) {
		fin.read(buffer, memory_use);
		size_t count = fin.gcount();
		string decoded_buffer;
		decoded_buffer += last_record;
		if (!count) break;
		int Filledbuffer = readtime < full_buffer_times ? memory_use : last_buffer;
		if (readtime == full_buffer_times) {
			for (int i = 0; i < Filledbuffer - 2; i++) {
				decode_queue = bitset<8>(buffer[i]).to_string();
				for (int j = 0; j < 8; j++) {
					if (decode_queue[j] == '0') curr = curr->left;
					else curr = curr->right;
					if (curr->c != NULL) {
						decoded_buffer += curr->c;
						curr = root;
					}
				}
			}
			decode_queue = bitset<8>(buffer[Filledbuffer - 2]).to_string();
			int lastbits = (int)buffer[Filledbuffer - 1] - '0';
			for (int i = 0; i < 8 - lastbits; i++) {
				if (decode_queue[i] == '0') curr = curr->left;
				else curr = curr->right;
				if (curr->c != NULL) {
					decoded_buffer += curr->c;
					curr = root;
				}
			}
		}
		else {
			for (int i = 0; i < Filledbuffer; i++) {
				decode_queue = bitset<8>(buffer[i]).to_string();
				for (int j = 0; j < 8; ++j) {
					if (decode_queue[j] == '0') curr = curr->left;
					else curr = curr->right;
					if (curr->c != NULL) {
						decoded_buffer += curr->c;
						if (i = Filledbuffer - 1) {
							last_record = decoded_buffer.substr(decoded_buffer.length()- obj.length() + 1);
						}
						curr = root;
					}
				}
			}
		}
		
		while (true) {
			int find_pos = (int)decoded_buffer.find(obj);
			if (find_pos == -1) break;
			else
			{
				++total_find;
				decoded_buffer = decoded_buffer.substr(find_pos + 1);
			}
		}
		readtime++;
	}
	fin.close();
	this->dictionary.clear();
	return total_find;
}

void Static_Huffman_Coding::encode(string inputfile, string outputfile)
{
	this->freq_count(inputfile);
	this->HuffmanTreeBuilder();
	ofstream Huffman_output(outputfile.c_str(), ios::out | ios::binary);
	this->code_char(this->HuffmanTree.top(), "");
	this->header_encode(Huffman_output);
	this->dictionary.clear();
	this->envolope(inputfile, Huffman_output);
	this->Code.clear();
	Huffman_output.close();
}

void Static_Huffman_Coding::decode(string inputfile, string outputfile)
{
	size_t memory_use = this->memory_use;
	ifstream fin(inputfile.c_str(), ifstream::ate | ifstream::binary);
	ofstream Huffman_output(outputfile.c_str(), ios::out | ios::binary);
	const int file_length = fin.tellg();
	fin.seekg(0, fin.beg);
	if (memory_use == 0) memory_use = file_length;
	this->headerRead(fin);
	this->content_decode(fin, file_length - 1023, Huffman_output);
	this->dictionary.clear();
	Huffman_output.close();
}

int Static_Huffman_Coding::search(string inputfile, string obj)
{
	size_t memory_use = this->memory_use;
	ifstream fin(inputfile.c_str(), ifstream::ate | ifstream::binary);
	const int file_length = fin.tellg();
	fin.seekg(0, fin.beg);
	if (memory_use == 0) memory_use = file_length;
	this->headerRead(fin);
	return this->content_search(fin, file_length - 1023, obj);
}
