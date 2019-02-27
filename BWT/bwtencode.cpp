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

SA_char find_SA_char(vector<SA_char> SA,  int index) {
	return SA.at(SA.size() - index - 1);
}

void Suffix_Array(string inp, string indexfile, string bwtfile, char separater) {
	vector<SA_char> SA;
	int last_index = inp.length();
	for (int i = inp.length() - 1; i >= 0; i--) {
		SA_char new_char;
		new_char.index = i;
		new_char.text = inp[i];
		new_char.rank = -1; // indicate unranked;
		if (i == inp.length() - 1 || inp[i] < inp[i + 1]) {
			new_char.type = true;
			if (i == inp.length() - 1 && inp[i] > inp[0]) new_char.type = false;
		}
		else
		{
			new_char.type = false;
		}
		//cout << new_char.index << " " << new_char.text << " " << new_char.type<<endl;
		SA.push_back(new_char);
	}
	map<char, vector<int> > bucket;
	int distance_to_S = 0;
	int special_case = 1;
	for (int i = inp.length() - 1; i >= 0; i--) {
		SA_char *curr = &SA.at(i);
		if (bucket.find(curr->text) != bucket.end()) {
			bucket[curr->text].push_back(curr->index);
		}
		else
		{
			vector<int> tmp_list;
			tmp_list.push_back(curr->index);
			bucket[curr->text] = tmp_list;
		}
		if (curr->index > 0 && SA.at(i+1).type == true) {
			curr->distance = 1;
			distance_to_S = 1;
		}
		else
		{
			//curr->distance = ++distance_to_S;
			if (distance_to_S == 0) {
				//curr->distance = special_case++;
				curr->distance = 0;
			}
			else
			{
				curr->distance = ++distance_to_S;
			}
		}
		//cout << curr->text << " " << curr->index << " " << curr->distance << endl;
	}

	if (SA.at(0).type == true) special_case = 1;
	else
	{
		special_case = SA.at(0).distance + 1;
	}

	for (int i = inp.length() - 1; i >= 0; i--) {
		SA_char *curr = &SA.at(i);
		if (curr->distance == 0) {
			curr->distance = special_case++;
		}

		//cout << curr->text << " " << curr->index << " " << curr->distance << endl;
	}

	/*for (auto it : bucket)
	{
		cout << it.first <<" : ";
		for (vector<int>::iterator i = bucket[it.first].begin(); i != bucket[it.first].end(); ++i)
			cout << *i << ' ';
		cout << endl;
	}*/

	map<int, vector<vector<int> > > DList;
	int DListMax = 0;
	int SListMaxSize = 0;
	vector<vector<int> > SList;
	char prev;
	int counter = 0;
	for (map<char, vector<int> >::iterator it = bucket.begin(); it != bucket.end(); ++it)
	{
		for (vector<int>::iterator i = it->second.begin(); i != it->second.end(); ++i) {
			SA_char curr = find_SA_char(SA, *i);
			if (curr.type) {
				if (counter > 0 && curr.text == prev) {
					SList.at(SList.size()-1).push_back(*i);
				}
				else {
					vector<int> tmp;
					tmp.push_back(*i);
					SList.push_back(tmp);
				}
				SListMaxSize++;
				prev = curr.text;
				counter++;
			}
			
			//if (curr.distance == 0) continue;
			DListMax = max(DListMax, curr.distance);
			if (DList.find(curr.distance) != DList.end()) {
				if (find_SA_char(SA, DList[curr.distance].at(DList[curr.distance].size()-1).at(0)).text == curr.text) {
					DList[curr.distance].at(DList[curr.distance].size()-1).push_back(*i);
				}
				else {
					vector<int> tmp;
					tmp.push_back(*i);
					DList[curr.distance].push_back(tmp);
				}
			}
			else {
				vector<vector<int> > tmp_list;
				vector<int> tmp;
				tmp.push_back(*i);
				tmp_list.push_back(tmp);
				DList[curr.distance] = tmp_list;
			}
		}
	}

	/*for (auto it : DList) {
		cout << it.first << " : ";
		for (vector<vector<int>>::iterator i = it.second.begin(); i != it.second.end(); ++i) {
			cout << "[";
			for (vector<int>::iterator j = i->begin(); j != i->end(); j++) {
				cout << *j << " ";
			}
			cout << "] ";
		}
		cout << endl;
	}*/

	//for (vector<vector<int>>::iterator it = SList.begin(); it != SList.end(); ++it) {
	//	cout << "[";
	//	for (vector<int>::iterator i = it->begin(); i != it->end(); ++i) {
	//		cout << *i << " ";
	//	}
	//	cout << "] ";
	//}
	//cout << endl;

	int SPos = 0;
	int sorted_char = 0;
	for (int shifter = 1; shifter <= DListMax; shifter++)
	{
		SPos = 0;
	for (vector<vector<int> >::iterator lis = SList.begin(); lis != SList.end(); ++lis)
	{
		if (lis->size() > 1){
				if (lis->size() <= 1) break;
				for (vector<vector<int> >::iterator ele = DList[shifter].begin(); ele != DList[shifter].end(); ++ele) {
					vector<int> pickout;
					for (vector<int>::iterator DList_ele = ele->begin(); DList_ele != ele->end(); ++DList_ele) {
						int shouldins = *DList_ele - shifter;
						if (shouldins < 0) shouldins = SA.size() + shouldins;
						int erase_index = 0;
						for (vector<int>::iterator is_in = lis->begin(); is_in != lis->end(); ++is_in) {
							if (shouldins == *is_in) {
								pickout.push_back(shouldins);
								lis->erase(lis->begin() + erase_index);
								break;
							}
							erase_index++;
						}
					}
					if (pickout.size() > 0) {
						SList.insert(SList.begin() + (SPos++), pickout);
						lis = SList.begin() + (SPos);
						if (lis->size() == 0) {
							SList.erase(SList.begin() + SPos);
							lis = SList.begin() + (SPos--) - 1;
						}

					}	
				}
			}
		SPos++;
		}
	}

	//for (vector<vector<int>>::iterator it = SList.begin(); it != SList.end(); ++it) {
	//	cout << "[";
	//	for (vector<int>::iterator i = it->begin(); i != it->end(); ++i) {
	//		cout << *i << " ";
	//	}
	//	cout << "] ";
	//}
	//cout << endl;

	//cout << SList.size() << endl;


	int iteration_times = 0;
	vector<vector<int> > sft = SList;
	/*for (vector<vector<int>>::iterator lis = SList.begin(); lis != SList.end(); ++lis)
	{
		for (vector<int>::iterator i = lis->begin(); i != lis->end(); i++)
		{
			sft.push_back(*i);
		}
	}*/

	while (true)
	{
		int shifter;
		for (int p = 0; p < SList.size(); p++)
		{
			shifter = 1;
			while (SList[p].size() > 1)
			{
				for (int i = 0; i < SList.size(); i++)
				{
					vector<int> pickout;
					for (int j = 0; j < SList[i].size(); j++)
					{
						int canpick = 0;
						while (canpick < SList[p].size()) {
							int match = SList[i][j] - shifter;
							if (match < 0)
							{
								match += SA.size();
							}
							if (SList[p][canpick] == match)
							{
								pickout.push_back(match);
								SList[p].erase(SList[p].begin() + canpick);
							}
							else
							{
								canpick++;
							}
						}
					}
					if (pickout.size()>0)
					{
						SList.insert(SList.begin() + p, pickout);
						p++;
					}
					if (SList[p].size()==0)
					{
						SList.erase(SList.begin() + p);
						p--;
					}
				}
				shifter++;
			}
		}
		bool canbreak = true;
		for (vector<vector<int> >::iterator it = SList.begin(); it != SList.end(); it++)
		{
			if (it->size() > 1)
			{
				canbreak = false;
			}
		}
		if (canbreak)
		{
			break;
		}
	}
	
	/*for (vector<vector<int>>::iterator it = SList.begin(); it != SList.end(); ++it) {
	cout << "[";
	for (vector<int>::iterator i = it->begin(); i != it->end(); ++i) {
	cout << *i << " ";
	}
	cout << "] ";
	}
	cout << endl;*/



	map<char, vector<vector<int> > > sorted_bucket;
	for (map<char, vector<int> >::iterator bc = bucket.begin(); bc != bucket.end(); ++bc) {
		sorted_bucket[bc->first] = vector<vector<int> >();
		sorted_bucket[bc->first].push_back(vector<int >());
		sorted_bucket[bc->first].push_back(vector<int >());
	}
	bucket.clear();
	int filled = 0;
	for (vector<vector<int> >::iterator it = SList.begin(); it != SList.end(); ++it)
	{
		if (it->size() == 1) {
			SA_char *thischar = &SA.at(SA.size() - it->at(0) - 1);
			thischar->rank = 1;
			sorted_bucket[thischar->text].at(1).push_back(thischar->index);
			++filled;
		}	
	}
	while (filled < SA.size())
	{
		for (map<char, vector<vector<int> > >::iterator it = sorted_bucket.begin(); it != sorted_bucket.end(); ++it) {
			int itracker = 0;
			for (vector<vector<int> >::iterator i = sorted_bucket[it->first].begin(); i != sorted_bucket[it->first].end(); ++i) {
				int jtracker = 0;
				for (vector<int>::iterator j = i->begin(); j != i->end(); j++) {
					int next_fill = *j - 1;
					if (next_fill == -1) next_fill = SA.size() - 1;
					SA_char *thischar = &SA.at(SA.size() - next_fill - 1);
					if (thischar->rank == -1) {
						sorted_bucket[thischar->text].at(0).push_back(thischar->index);
						thischar->rank = 1;
						i = sorted_bucket[it->first].begin() + itracker;
						j = i->begin() + jtracker;
						filled++;
					}
					++jtracker;
				}	
				++itracker;
			}
		}
	}
	string outputstr = "";
	for (map<char, vector<vector<int> > >::iterator sot = sorted_bucket.begin(); sot != sorted_bucket.end(); ++sot) {
		for (vector<vector<int> >::iterator i = sorted_bucket[sot->first].begin(); i != sorted_bucket[sot->first].end(); ++i) {
			for (vector<int>::iterator j = i->begin(); j != i->end(); j++)
			{
				int thisind = *j - 1;
				if (thisind == -1) thisind = SA.size() - 1;
				char currtxt = find_SA_char(SA, thisind).text;
				outputstr += currtxt;
			}
		}
	}
	map<int, int> indexMap;
	int ind1 = 0;
	for (map<char, vector<vector<int> > >::iterator buc = sorted_bucket.begin(); buc != sorted_bucket.end(); ++buc)
	{
		for (int i = 0; i < buc->second.size(); i++)
		{
			for (int j = 0; j < buc->second[i].size(); j++)
			{
				int in = buc->second[i][j] - 1;
				if (in == -1) in = SA.size() - 1;
				char currtxt = find_SA_char(SA, in).text;
				if (currtxt == separater)
				{
					indexMap[in] = ind1;
				}
				ind1++;
			}
		}
	}
	
	ofstream bwtfout;
	bwtfout.open(bwtfile.c_str());
	bwtfout << outputstr;
	bwtfout.close();

	ofstream auxfile;
	auxfile.open(indexfile.c_str());
	for (map<int, int>::iterator i = indexMap.begin(); i != indexMap.end(); i++)
	{
		auxfile << i->second << " ";
	}
	auxfile.close();
	//for (auto it : sorted_bucket)
	//{
	//cout << it.first <<" : ";
	//for (vector<vector<int>>::iterator i = sorted_bucket[it.first].begin(); i != sorted_bucket[it.first].end(); ++i) {
	//for (vector<int>::iterator j = i->begin(); j != i->end(); j++)
	//{
	//cout << *j << ' ';
	//}
	//}

	//cout << endl;
	//}

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

	string inp = argv[3];
	ifstream ifile;
	ifile.open(inp.c_str());
	char c;
	string p = "";
	while (ifile.get(c))
	{
		p += c;
	}
	string bwtfile = argv[4];
	string auxfile = bwtfile;
	auxfile += ".aux";
	/*for (int i = 0; i < auxfile.size()-3; i++)
	{
		auxfile += bwtfile[i];
	}
	auxfile += 'a';
	auxfile += 'u';
	auxfile += 'x';*/
	Suffix_Array(p, auxfile, bwtfile, separater);
	//cout << get_lasat_col(BWT_matrix(inp5)) << endl;
	return 0;
}