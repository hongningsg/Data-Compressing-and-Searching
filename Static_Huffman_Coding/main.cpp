#include "Static_Huffman_Coding.h"

using namespace std;

int main(int argc, char* argv[]) {
	Static_Huffman_Coding *hf = new Static_Huffman_Coding(1000000, false);
	string errormsg = "USAGE:\n -e [inputfile] [outputfile] for encode\n -d [inputfile] [outputfile] for decode\
				\n -s [inputfile] string for search\n";
	if (argc != 4) {
		cout << errormsg;
		return -1;
	}
	if ((string)argv[1] == "-e") {
		hf->encode(argv[2], argv[3]);
	}
	else if ((string)argv[1] == "-d") {
		hf->decode(argv[2], argv[3]);
	}
	else if ((string)argv[1] == "-s") {
		int f = hf->search(argv[3], argv[2]);
	  cout << f << endl;
	  return f;
	}
	else {
		cout << errormsg;
		return -1;
	}
	return 0;
}