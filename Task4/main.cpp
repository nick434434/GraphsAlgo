#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>


using namespace std;


unsigned int H, V, N;
vector<pair<int, int>> edges;
vector<pair<int, pair<int, int>>> crossings, horW, vecW;


void inputWords(string filename) {
    ifstream f(filename);
    f >> H >> V;
    horW.resize(H);
    vecW.resize(V);


    for (size_t i = 0; i < H; i++)
        f >> horW[i].first >> horW[i].second.first >> horW[i].second.second;
    for (size_t i = 0; i < V; i++)
        f >> vecW[i].second.first >> vecW[i].second.second >> vecW[i].first;

    f.close();

    sort(vecW.begin(), vecW.end(),
         [](pair<int, pair<int, int>> a, pair<int, pair<int, int>> b)
         { return a.first < b.first; }
    );
    sort(horW.begin(), horW.end(),
         [](pair<int, pair<int, int>> a, pair<int, pair<int, int>> b)
         { return a.first < b.first; }
    );
}


int main() {

    inputWords("input.txt");

    ofstream fout("output.txt");
    fout << -1;
    fout.close();

    return 0;
}