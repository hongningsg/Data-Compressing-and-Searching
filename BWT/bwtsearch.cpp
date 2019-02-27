#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>
#include <set>
#include <map>
#include <bitset>
#include <fstream>
using namespace std;

vector<bool> get_B(string inp) {
	char prev = inp[0];
	vector<bool> B;
	B.push_back(true);
	if (inp.size() == 1) return B;
	for (int i = 1; i < inp.size(); i++) {
		char curr = inp[i];
		if (curr == prev) {
			B.push_back(false);
		}
		else
		{
			B.push_back(true);
		}
		prev = curr;
	}
	return B;
}

vector<char> get_S(string inp) {
	char prev = inp[0];
	vector<char> S;
	S.push_back(prev);
	if (inp.size() == 1) return S;
	for (int i = 1; i < inp.size(); i++) {
		char curr = inp[i];
		if (curr != prev) {
			S.push_back(curr);
			prev = curr;
		}
	}
	return S;
}

map<char, int> get_S_prime(vector<char> S) {
	map<char, int> S_p;
	sort(S.begin(), S.end());
	char prev = S.at(0);
	S_p[prev] = 0;
	int total = 1;
	for (vector<char>::iterator it = S.begin() + 1; it != S.end(); ++it) {
		if (*it != prev) {
			S_p[*it] = total;
			prev = *it;
		}
		total++;
	}
	return S_p;
}

int rank1(vector<int> B, int pos) {
	int count1 = 1;
	for (vector<int>::iterator it = B.begin() + 1; it != B.end(); ++it) {
		if (*it == pos) return count1;
		if (*it > pos) return count1 - 1;
		count1++;
	}
	return count1 - 1;
}

int select1(vector<bool> B, int num) {
	int total = 0;
	int pos = 0;
	for (vector<bool>::iterator it = B.begin() + 1; it != B.end(); it++) {
		if (*it == true) total++;
		if (total == num) return pos;
		++pos;
	}
	return pos;
}

int ranks(string file, char s, int pos) {
	int index = 1;
	int count = 0;
	ifstream fin(file.c_str(), ifstream::binary);
	char c;
	char prev;
	fin.get(prev);
	if (prev == s) count++;
	if (pos == 0) return count;
	while (index < pos + 1) {
		fin.get(c);
		//cout << c << endl;
		if (c != prev) {
			prev = c;
			if (c == s) count++;
			index++;
		}
	}
	fin.close();
	return count;
}

char L(string file, int pos) {
	ifstream fin(file.c_str(), ifstream::binary);
	char l;
	for (int i = 0; i <= pos; i++) {
		fin.get(l);
	}
	return l;
}

int indexof(map<char, int> Cs, char c) {
	int index = 0;
	for (map<char, int>::iterator it = Cs.begin(); it != Cs.end(); ++it) {
		if (it->first == c) return index;
		index++;
	}
	return -1;
}

vector<int> backward_search(string inp, string query) {
	vector<int> selectB;
	vector<int> selectBp;
	selectBp.push_back(0);
	map<char, vector<bool> > Bp;
	map<char, int> Cs;
	ifstream fin(inp.c_str(), ifstream::ate | ifstream::binary);
	const int file_length = fin.tellg();
	fin.seekg(0, fin.beg);
	char prev, curr;
	int index = 0;
	fin.get(prev);
	selectB.push_back(0);
	selectB.push_back(index++);
	vector<bool> tmp;
	tmp.push_back(true);
	Bp[prev] = tmp;
	while (fin.get(curr))
	{
		bool tf;
		if (curr != prev) {
			selectB.push_back(index);
			prev = curr;
			tf = true;
		}
		else {
			tf = false;
		}
		if (Bp.find(curr) != Bp.end()) {
			Bp[curr].push_back(tf);
		}
		else {
			Bp[curr].push_back(tf);
		}
		index++;
	}
	int total = 0;
	index = 0;
	for (map<char, vector<bool> >::iterator it = Bp.begin(); it != Bp.end(); ++it) {
		Cs[it->first] = total;
		for (vector<bool>::iterator bool_it = it->second.begin(); bool_it != it->second.end(); ++bool_it) {
			if (*bool_it) {
				++total;
				selectBp.push_back(index);
			}
			index++;
		}
		Bp[it->first].clear();
	}
	Bp.clear();

	int i = query.size() - 1;
	char l;
	char c = query[i];
	int charind = indexof(Cs, c);
	if (charind == -1) {
		vector<int> re;
		re.push_back(-1);
		re.push_back(-2);
		return re;
	}
	//map<char, int>::iterator it = next(Cs.begin(), charind);
	map<char, int>::iterator it = Cs.begin();
	for (int i = 0; i < charind; i++)
	{
		++it;
	}
	int first = selectBp.at(it->second + 1);
	it++;
	int last;
	if (it == Cs.end()) last = file_length - 1;
	else last = selectBp.at(it->second + 1) - 1;
	//cout << first << "   " << last << endl;
	while ((first <= last) && (i > 0)) {
		//cout << "i: " << i << endl;
		c = query[i - 1];
		if (first != 0) {
			l = L(inp, first - 1);
		}

		charind = indexof(Cs, c);
		if (charind == -1) {
			vector<int> re;
			re.push_back(-1);
			re.push_back(-2);
			return re;
		}
		it = Cs.begin();
		for (int i = 0; i < charind; i++)
		{
			++it;
		}
		if (first == 0 || l != c) {
			first = selectBp.at(it->second + 1 + ranks(inp, c, rank1(selectB, first - 1) - 1));
		}
		else {
			while (true)
			{
				l = L(inp, first);
				if (l == c) {
					first = selectBp.at(it->second + ranks(inp, c, rank1(selectB, first) - 1)) + first - selectB.at(rank1(selectB, first));
					break;
				}
				else
				{
					first++;
				}

			}
		}
		l = L(inp, last);
		if (c != l) {
			int lasti = it->second + 1 + ranks(inp, c, rank1(selectB, last) - 1);
			if (lasti >= selectBp.size()) last = file_length - 1;
			else last = selectBp.at(it->second + 1 + ranks(inp, c, rank1(selectB, last) - 1)) - 1;
		}
		else {
			while (true)
			{
				if (l == c) {
					last = selectBp.at(it->second + ranks(inp, c, rank1(selectB, last) - 1)) + last - selectB.at(rank1(selectB, last));
					break;
				}
				else
				{
					last--;
				}
				l = L(inp, last);
			}
		}

		//cout << first << " " << last << endl;
		i--;
	}
	if (last < first) {
		vector<int> re;
		re.push_back(-1);
		re.push_back(-2);
		return re;
	}
	else
	{

	}return vector<int>{first, last};
}

void BWS_n(string inp, string query, char separater, string inpfile, bool typen) {
	vector<int> selectB;
	vector<int> selectBp;
	selectBp.push_back(0);
	map<char, vector<bool> > Bp;
	map<char, int> Cs;
	map<int, bool> find_sp;
	vector<int> sp;
	ifstream fin(inp.c_str(), ifstream::ate | ifstream::binary);
	const int file_length = fin.tellg();
	fin.seekg(0, fin.beg);
	char prev, curr;
	int index = 0;
	fin.get(prev);
	selectB.push_back(0);
	selectB.push_back(index++);
	vector<bool> tmp;
	tmp.push_back(true);
	Bp[prev] = tmp;

	if (prev == separater) sp.push_back(0);
	while (fin.get(curr))
	{
		bool tf;
		//cout << curr << " " << index << endl;
		if (curr == separater) {
			sp.push_back(index);
		}
		if (curr != prev) {
			selectB.push_back(index);
			prev = curr;
			tf = true;
		}
		else {
			tf = false;
		}
		if (Bp.find(curr) != Bp.end()) {
			Bp[curr].push_back(tf);
		}
		else {
			Bp[curr] = vector<bool>{ tf };
		}
		index++;
	}

	for (vector<int>::iterator it = sp.begin(); it != sp.end(); ++it) {
		find_sp[*it] = false;
	}

	int total = 0;
	index = 0;
	for (map<char, vector<bool> >::iterator it = Bp.begin(); it != Bp.end(); ++it) {
		Cs[it->first] = total;
		for (vector<bool>::iterator bool_it = it->second.begin(); bool_it != it->second.end(); ++bool_it) {
			if (*bool_it) {
				++total;
				selectBp.push_back(index);
			}
			index++;
		}
		Bp[it->first].clear();
	}
	Bp.clear();

	int i = query.size() - 1;
	char l;
	char c = query[i];
	int charind = indexof(Cs, c);
	bool nonfound = false;
	if (charind == -1) {
		charind = 0;
		nonfound = true;
	}
	map<char, int>::iterator it = Cs.begin();
	for (int i = 0; i < charind; i++)
	{
		++it;
	}
	int first = selectBp.at(it->second + 1);
	it++;
	int last;
	if (it == Cs.end()) last = file_length - 1;
	else last = selectBp.at(it->second + 1) - 1;

	while ((first <= last) && (i > 0)) {
		c = query[i - 1];
		if (first != 0) {
			l = L(inp, first - 1);
		}

		charind = indexof(Cs, c);
		if (charind == -1) {
			nonfound = true;
			charind = 0;
			break;
		}
		it = Cs.begin();
		for (int i = 0; i < charind; i++)
		{
			++it;
		}
		if (first == 0 || l != c) {
			first = selectBp.at(it->second + 1 + ranks(inp, c, rank1(selectB, first - 1) - 1));
		}
		else {
			while (true)
			{
				l = L(inp, first);
				if (l == c) {
					first = selectBp.at(it->second + ranks(inp, c, rank1(selectB, first) - 1)) + first - selectB.at(rank1(selectB, first));
					break;
				}
				else
				{
					first++;
				}

			}
		}
		l = L(inp, last);
		if (c != l) {
			int lasti = it->second + 1 + ranks(inp, c, rank1(selectB, last) - 1);
			if (lasti >= selectBp.size()) last = file_length - 1;
			else last = selectBp.at(it->second + 1 + ranks(inp, c, rank1(selectB, last) - 1)) - 1;
		}
		else {
			while (true)
			{
				if (l == c) {
					last = selectBp.at(it->second + ranks(inp, c, rank1(selectB, last) - 1)) + last - selectB.at(rank1(selectB, last));
					break;
				}
				else
				{
					last--;
				}
				l = L(inp, last);
			}
		}
		i--;
	}
	//cout << first << " " << last << endl;
	while (first <= last) {
		int temp = first;
		while (true) {
			l = L(inp, temp);
			//cout << l << endl;
			if (l == separater) {
				find_sp[temp] = true;
				break;
			}
			temp = selectBp.at(Cs[l] + ranks(inp, l, rank1(selectB, temp) - 1)) + temp - selectB.at(rank1(selectB, temp));
		}
		first++;
	}

	//cout << "output : " << endl;
	if (typen) {
		if (nonfound) {
			cout << "0" << endl;
		}
		else {
			int total = 0;
			for (map<int, bool>::iterator it = find_sp.begin(); it != find_sp.end(); it++)
			{
				if (it->second) total++;
			}
			cout << total << endl;
		}
	}
	else
	{
		if (!nonfound) {
			ifstream fin;
			fin.open(inpfile.c_str());
			char readchar;
			vector<int> indexnumbers;
			string numbers = "";
			while (fin.get(readchar))
			{
				if (readchar == ' ')
				{
					indexnumbers.push_back(atoi(numbers.c_str()));
					numbers = "";
				}
				else {
					numbers += readchar;
				}
			}
			fin.close();
			int rankind = 1;

			int i = indexnumbers.size() - 1;
			while (i != indexnumbers.size() - 2)
			{
				if (find_sp[indexnumbers[i]]) {
					cout << rankind << endl;
				}
				i++;
				rankind++;
				if (i == indexnumbers.size()) i = 0;
			}
			if (find_sp[indexnumbers[i]]) {
				cout << rankind << endl;
			}
		}
	}
}

void BWS_i(string inp, string query, char separater, string inpfile) {
	vector<int> selectB;
	vector<int> selectBp;
	selectBp.push_back(0);
	map<char, vector<bool> > Bp;
	map<char, int> Cs;
	map<int, bool> find_sp;
	vector<int> sp;
	ifstream fin(inp.c_str(), ifstream::ate | ifstream::binary);
	const int file_length = fin.tellg();
	fin.seekg(0, fin.beg);
	char prev, curr;
	int index = 0;
	fin.get(prev);
	selectB.push_back(0);
	selectB.push_back(index++);
	vector<bool> tmp;
	tmp.push_back(true);
	Bp[prev] = tmp;

	if (prev == separater) sp.push_back(0);
	while (fin.get(curr))
	{
		bool tf;
		//cout << curr << " " << index << endl;
		if (curr == separater) {
			sp.push_back(index);
		}
		if (curr != prev) {
			selectB.push_back(index);
			prev = curr;
			tf = true;
		}
		else {
			tf = false;
		}
		if (Bp.find(curr) != Bp.end()) {
			Bp[curr].push_back(tf);
		}
		else {
			Bp[curr].push_back(tf);// = vector<bool>{ tf };
		}
		index++;
	}

	//for (auto it : sp) {
	//	find_sp[it] = false;
	//}
	//for (auto it: Bp)
	//{
	//	cout << it.first << " : ";
	//	for (auto i : it.second)
	//	{
	//		cout << i;
	//	}
	//	cout << endl;
	//}


	int total = 0;
	index = 0;
	for (map<char, vector<bool> >::iterator it = Bp.begin(); it != Bp.end(); ++it) {
		Cs[it->first] = total;
		for (vector<bool>::iterator bool_it = it->second.begin(); bool_it != it->second.end(); ++bool_it) {
			if (*bool_it) {
				++total;
				selectBp.push_back(index);
			}
			index++;
		}
		Bp[it->first].clear();
	}
	Bp.clear();

	/*for (auto i : selectBp) {
	cout << i << endl;
	}*/
	/*for (auto i : Cs) {
	cout << i.first << " : " << i.second << endl;
	}*/

	string firstc = "", secondc = "";
	bool splitat = false;
	for (int i = 0; i < query.size(); i++)
	{
		if (splitat)
		{
			secondc += query[i];
		}
		else
		{
			if (query[i] == ' ')
			{
				splitat = true;
			}
			else
			{
				firstc += query[i];
			}
		}
	}
	int firstnum = atoi(firstc.c_str());
	int secondnum = atoi(secondc.c_str());

	ifstream fin2;
	fin2.open(inpfile.c_str());
	char readchar;
	vector<int> indexnumbers;
	string numbers = "";
	while (fin2.get(readchar))
	{
		if (readchar == ' ')
		{
			indexnumbers.push_back(atoi(numbers.c_str()));
			numbers = "";
		}
		else {
			numbers += readchar;
		}
	}
	fin2.close();
	vector<int> pickout;
	for (int pi = firstnum - 1; pi < secondnum; pi++)
	{
		if (pi < 0)
		{
			pi = 0;
		}
		if (pi > indexnumbers.size() - 1)
		{
			pi = indexnumbers.size() - 1;
		}
		pickout.push_back(indexnumbers[pi]);
	}
	for (int i = 0; i < pickout.size(); i++)
	{
		int temp = pickout[i];
		char l = separater;
		string outp = "";
		while (true) {
			//cout << Cs[l] << endl;
			temp = selectBp.at(Cs[l] + ranks(inp, l, rank1(selectB, temp) - 1)) + temp - selectB.at(rank1(selectB, temp));
			l = L(inp, temp);
			//cout << temp << l << endl;
			if (l == separater) {
				break;
			}
			outp += l;
		}
		reverse(outp.begin(), outp.end());
		cout << outp << endl;
	}
}

int main(int argc, char* argv[]) {
	string spstr = argv[1];
	char separater;
	if (spstr.size() == 2) {
		if (spstr[1] == 'n') {
			separater = '\n';
		}
		else if (spstr[1] == 't')
		{
			separater = '\t';
		}
	}
	else
	{
		separater = spstr[0];
	}

	string inp = argv[2];
	string command = argv[4];
	string query = argv[5];

	string auxfile = inp;
	auxfile += ".aux";
	/*for (int i = 0; i < auxfile.size() - 3; i++)
	{
		auxfile += inp[i];
	}
	auxfile += 'a';
	auxfile += 'u';
	auxfile += 'x';*/

	if (command == "-m")
	{
		vector<int> bw = backward_search(inp, query);
		cout << bw[1] - bw[0] + 1 << endl;
	}
	else if (command == "-n") {
		BWS_n(inp, query, separater, auxfile, true);
	}
	else if (command == "-a") {
		BWS_n(inp, query, separater, auxfile, false);
	}
	else
	{
		BWS_i(inp, query, separater, auxfile);
	}
	return 0;
}