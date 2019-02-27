#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <bitset>
#include <fstream>

using namespace std;

typedef struct SA_char {
	char text;
	int index;
	bool type;//true for S, false for L
	int rank;
	int distance;
};

vector<string> BWT_matrix(string inp);
string get_lasat_col(vector<string> inp);
void Suffix_Array(string inp, string indexfile, string bwtfile, char separater);
