#include <iostream>
#include <vector>
#include <fstream>

using std::cout;
using std::endl;
using std::vector;
using std::ifstream;
using std::ofstream;


vector<vector<int>> G1, G2;
unsigned int N;
unsigned int R1, C1, R2, C2;


vector<vector<int>> operator+(vector<vector<int>> lhs, vector<vector<int>> rhs) {
    vector<vector<int>> res = {0};
    size_t n = lhs.size();

    if (n != rhs.size())
        return res;
    
    res.resize(n);
    for (size_t i = 0; i < n; i++) {
        res[i].resize(n);
        for (size_t j = 0; j < n; j++)
            res[i][j] = lhs[i][j] + rhs[i][j];
    }
    
    return res;
}

void inputGraphs(ifstream fin) {
    fin >> N >> R1 >> C1 >> R2 >> C2;
    G1.resize(N);
    for (size_t i = 0; i < N; i++) {
        G1[i].resize(N);
        for (size_t j = 0; j < N; j++)
            fin >> G1[i][j];
    }
    G2.resize(N);
    for (size_t i = 0; i < N; i++) {
        G2[i].resize(N);
        for (size_t j = 0; j < N; j++)
            fin >> G1[i][j];
    }
    fin.close();
}

unsigned int tryFind() {
    vector<vector<int>> G3 = G1 + G2;

    unsigned int numSteps = 0;

    for (size_t i = 1; i < N; i++)
        numSteps += 2 + G3[0][N - i - 1] + G3[i][N - 1];
    numSteps += G3[N - 1][N - 1] + 1;

    return numSteps;
}


int main() {

    ifstream fin("input.txt");
    inputGraphs(fin);

    ofstream fout("output.txt");
    fout << tryFind();

    return 0;
}