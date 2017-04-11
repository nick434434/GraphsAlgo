

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <list>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <iomanip>


using std::vector;
using std::ifstream;
using std::stringstream;


vector<vector<int>> GG, bestWay;
vector<vector<double>> G;
vector<bool> used;
vector<int> ans;
unsigned int N;
int len = 0;

std::ofstream fout("output.txt");


void inputGraph(ifstream& fin) {
    fin >> N;
    G.resize(N);
    GG.resize(N);
    bestWay.resize(N);
    used.resize(N, false);
    ans.reserve(N);

    for (size_t i = 0; i < N; i++) {
        G[i].resize(N, -1);
        for (size_t j = 0; j < N; j++) {
            double tmp;
            fin >> G[i][j];
            if (G[i][j] != 0)
                GG[i].push_back(j);
        }
    }

    fin.close();
}
/*
for(int i=0; i<N; ++i)
for(int k=i+1; k<N; ++k)
for(int j=k+1; j<N; ++j)
if (a[i][j]<a[i][k]*a[k][j]) {
a[i][j] = a[i][k]*a[k][j];
b[i][j] = k;
*/

void dfs(int v) {
    used[v] = true;
    for (auto to : GG[v]) {
        if (!used[to])
            dfs(to);
    }
    ans.emplace_back(v);
}

void topological_sort() {
    for (int i = 0; i < N; ++i)
        used[i] = false;
    ans.clear();
    for (int i = 0; i < N; ++i)
        if (!used[i])
            dfs(i);
    std::reverse(ans.begin(), ans.end());
}


void findBestWays() {

    for(int i = 0; i < N; ++i)
        bestWay[i].resize(N);
    for (unsigned int i = 0; i < N; i++) {
        for (unsigned int k = i + 1; k < N; k++)
            for (unsigned int j = k + 1; j < N; j++) {
                double res = G[ans[i]][ans[k]] * G[ans[k]][ans[j]];
                if (G[ans[i]][ans[j]] < res) {
                    G[ans[i]][ans[j]] = res;
                    bestWay[ans[i]][ans[j]] = ans[k];
                }
            }
    }

}

void printBestWay(stringstream& ss, int v, int u) {

    if (bestWay[v][u] > 0) {

        int m = bestWay[v][u];
        printBestWay(ss, v, m);
        ss << m << " ";
        len++;
        printBestWay(ss, m, u);
    }

}

int main() {


    ifstream fin("input.txt");

    inputGraph(fin);

    topological_sort();

    findBestWays();

    fout << std::fixed << std::setprecision(2) << G[0][N - 1]<< " ";

    stringstream ss;
    len = 0;
    ss << 0 << " ";
    printBestWay(ss, 0, N - 1);
    ss << N - 1;

    fout << len + 2 << std::endl;
    fout << ss.str() << std::endl;

    fout.close();


    return 0;
}