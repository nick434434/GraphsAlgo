#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>


using namespace std;


unsigned int H, V, N;
int letters;
vector<pair<int, pair<int, int>>> horW, vecW, edges;
vector<pair<int, int>> crossings;

vector<int> p;

int dsu_get(int v) {
    return (v == p[v]) ? v : (p[v] = dsu_get (p[v]));
}

void dsu_unite(int a, int b) {
    a = dsu_get (a);
    b = dsu_get (b);
    if (a != b)
        p[a] = b;
}

void Kruskal(vector<pair<int, pair<int, int>>>& g) {

    unsigned int n = crossings.size();
    p.resize(n);
    for (int i = 0; i < n; i++)
        p[i] = i;

    sort(g.begin(), g.end(),
         [](pair<int, pair<int, int>> a, pair<int, pair<int, int>> b)
         { return a.first < b.first; }
    );

    int cost = 0;
    bool f = false;
    int j = 0;
    for (int i = 0; i < g.size() && j < n; i++) {
        int a = g[i].second.first,  b = g[i].second.second,  l = g[i].first;
        f = l > 0;
        if (dsu_get(a) == dsu_get(b)) {
            if (l == 0) {
                letters = -1;
                f = false;
                break;
            }
        }
        else {
            if (j == n - 1) {
                j++;
                continue;
            }
            cost += l;
            j++;
            dsu_unite(a, b);
        }
    }

    if (f) {
        letters += cost;
        for (auto e : g)
            letters -= e.first;
    }

}


bool isIn(int a, int b1, int b2) {
    return b1 <= a && a <= b2;
}

void swap(int& a, int& b) {
    int t = a;
    a = b;
    b = t;
}

void inputWords(string filename) {
    ifstream f(filename);
    f >> H >> V;
    horW.resize(H);
    vecW.resize(V);
    letters = 0;

    for (size_t i = 0; i < H; i++) {
        f >> horW[i].first >> horW[i].second.first >> horW[i].second.second;
        if (horW[i].second.first > horW[i].second.second)
            swap(horW[i].second.first, horW[i].second.second);
        letters += abs(horW[i].second.second - horW[i].second.first) + 1;
    }
    for (size_t i = 0; i < V; i++) {
        f >> vecW[i].second.first >> vecW[i].second.second >> vecW[i].first;
        if (vecW[i].second.first > vecW[i].second.second)
            swap(vecW[i].second.first, vecW[i].second.second);
        letters += abs(vecW[i].second.second - vecW[i].second.first) + 1;
    }
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

void makeGraph() {

    unsigned int k = 0;
    for (auto h : horW) {
        bool f = false;
        for (auto v : vecW) {
            if (isIn(h.first, v.second.first, v.second.second) && isIn(v.first, h.second.first, h.second.second)) {
                if (f) {
                    edges.push_back(make_pair(v.first - crossings[k - 1].second - 1, make_pair(k - 1, k)));
                }
                if (find(crossings.begin(), crossings.end(), make_pair(h.first, v.first)) == crossings.end()) {
                    crossings.push_back(make_pair(h.first, v.first));
                    letters--;
                    k++;
                    f = true;
                }
            }
        }
    }

    for (auto c = crossings.begin(); c < crossings.end(); c++) {
        auto v = vecW.begin();
        while (v->first != c->second && v != vecW.end())
            v++;
        if (v == vecW.end())
            continue;

        bool f = true;
        while (f) {
            while (!isIn(c->first, v->second.first, v->second.second)) {
                v++;
                if (v->first > c->second) {
                    f = false;
                    break;
                }
            }
            if (f)
                for (auto it = c + 1; it < crossings.end(); it++)
                    if (it->second == v->first && isIn(it->first, v->second.first, v->second.second)) {
                        edges.push_back(make_pair(it->first - c->first - 1,
                                                  make_pair(c - crossings.begin(), it - crossings.begin())));
                        break;
                    }
            v++;
            f = v->first == c->second;
        }
    }

}



int main() {

    inputWords("input.txt");

    makeGraph();

    Kruskal(edges);


    ofstream fout("output.txt");
    fout << letters;
    fout.close();

    return 0;
}