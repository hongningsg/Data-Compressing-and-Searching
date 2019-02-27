#include <string>
#include <iostream>
#include <vector>
#include "bwtencode.h"
#include "bwtsearch.h"

using namespace std;
int main(int argc, char* argv[]) {
	string spstr = argv[1];
	char separater;
	if (spstr.size() == 2) {
		if (spstr[1] == 'n') {
			separater = '\n';
		}
		else if(spstr[1] == 't')
		{
			separater = '\t';
		}
		else
		{
			separater = spstr[0];
		}
	}


	//Suffix_Array(inp5, "out.bwt", "index.aux", '|');
	//cout << get_lasat_col(BWT_matrix(inp5)) << endl;

	/*vector<int> bw = backward_search("file.txt", "ta");
	cout << bw[1] - bw[0] + 1 << endl;*/

	//BWS_n("file.txt", "in ", '|', "out.bwt", true);
	BWS_i("file.txt", "2 2", '|', "out.bwt");
	
	system("pause");
	return 0;
}
