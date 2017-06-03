
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <iomanip>


using std::vector;
using std::ifstream;
using std::stringstream;


vector<vector<int>> GG;
vector<vector<double>> G;
vector<bool> used;
vector<int> ans, bestWay;
unsigned int N;
int nMinus1 = -1, zero = -1;

std::ofstream fout("output.txt");


void inputGraph(ifstream& fin) {
    fin >> N;
    G.assign(N, vector<double>(N, -1));
    GG.resize(N);
    bestWay.resize(N);
    used.resize(N, false);
    ans.reserve(N);

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            double tmp;
            fin >> G[i][j];
            if (G[i][j] != 0)
                GG[i].push_back(j);
        }
    }

    fin.close();
}

void dfs(int v) {
    used[v] = true;
    for (auto to : GG[v]) {
        if (!used[to])
            dfs(to);
    }
    ans.emplace_back(v);
}

void findBorders() {
    for (unsigned int i = 0; i < N; i++) {
        if (ans[i] == 0)
            zero = i;
        if (ans[i] == N - 1)
            nMinus1 = i;
    }
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

void reorder() {
    vector<vector<double>> tmp;
    tmp.assign(N, vector<double>(N, 0));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            tmp[i][j] = G[ans[i]][ans[j]];
    G = tmp;
}

void findBestWays() {

    for (unsigned int k = zero + 1; k < N; k++)
        for (unsigned int j = k + 1; j < N; j++) {
            double res = G[zero][k] * G[k][j];
            if (G[zero][j] < res) {
                G[zero][j] = res;
                bestWay[j] = k;
            }
        }

}

void printBestWay() {

    vector<int> result;
    result.reserve(N);
    result.emplace_back(N - 1);
    int v = nMinus1;
    while (v) {
        v = bestWay[v];
        if (v != 0)
            result.emplace_back(ans[v]);
    }
    result.emplace_back(0);

    fout << result.size() << std::endl;

    std::reverse(result.begin(), result.end());
    for (auto r : result)
        fout << r << " ";

}

int main() {

    ifstream fin("input.txt");

    inputGraph(fin);

    topological_sort();

    reorder();

    findBorders();

    findBestWays();

    fout << std::fixed << std::setprecision(2) << G[zero][nMinus1]<< " ";

    printBestWay();

    fout.close();

    return 0;
}