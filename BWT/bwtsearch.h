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

vector<int> backward_search(string inp, string query);
void BWS_n(string inp, string query, char separater, string inpfile, bool typen);
void BWS_i(string inp, string query, char separater, string inpfile);