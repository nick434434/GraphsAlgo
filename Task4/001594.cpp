#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
//#include "getCPUTime.h"
#include <sys/time.h>

struct timeval tv1,tv2,dtv;
struct timezone tz;
void time_start() { gettimeofday(&tv1, &tz); }
long time_stop()
{ gettimeofday(&tv2, &tz);
    dtv.tv_sec= tv2.tv_sec -tv1.tv_sec;
    dtv.tv_usec=tv2.tv_usec-tv1.tv_usec;
    if(dtv.tv_usec<0) { dtv.tv_sec--; dtv.tv_usec+=1000000; }
    return dtv.tv_sec*1000+dtv.tv_usec/1000;
}



using namespace std;


unsigned int H, V, N;
int letters;
vector<pair<int, pair<int, int>>> horW, vecW, edges;
vector<pair<int, int>> crossings;

vector<int> p;

auto lambda = [](pair<int, pair<int, int>> a, pair<int, pair<int, int>> b)
{ return a.first < b.first; };



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

    sort(vecW.begin(), vecW.end(), lambda);
    sort(horW.begin(), horW.end(), lambda);
}

void makeGraph() {

    //log << 1 << endl;

    unsigned int k = 0;
    for (auto h : horW) {
        bool f = false;
        for (auto v : vecW) {
            if (isIn(h.first, v.second.first, v.second.second) && isIn(v.first, h.second.first, h.second.second)) {
                if (f) {
                    edges.push_back(make_pair(v.first - crossings[k - 1].second - 1, make_pair(k - 1, k)));
                }
                crossings.push_back(make_pair(h.first, v.first));
                letters--;
                k++;
                f = true;
            }
        }
    }

    for (auto c = crossings.begin(); c != crossings.end(); c++) {
        auto vb = lower_bound(vecW.begin(), vecW.end(), make_pair(c->second, make_pair(0, 0)), lambda);
        if (vb == vecW.end())
            continue;
        auto ve = upper_bound(vecW.begin(), vecW.end(), make_pair(c->second, make_pair(0, 0)), lambda);

        //log << vb->first << " " << vb->second.first << " " << vb->second.second << "; ";
        //log << ve->first << " " << ve->second.first << " " << ve->second.second << endl;

        bool f = true;
        for (auto v = vb; f && v != ve; v++) {
            if (isIn(c->first, v->second.first, v->second.second))
                for (auto it = c + 1; it != crossings.end(); it++)
                    if (it->second == v->first && isIn(it->first, v->second.first, v->second.second)) {
                        edges.push_back(make_pair(it->first - c->first - 1,
                                                  make_pair(c - crossings.begin(), it - crossings.begin())));
                        //log << it->first - c->first - 1 << " " << c - crossings.begin() << " " << it - crossings.begin() << endl;
                        f = false;
                        break;
                    }
        }
    }
    //log << endl;

}

void makeGraph2() {

    //log << 2 << endl;

    unsigned int k = 0;
    for (auto h : horW) {
        bool f = false;
        for (auto v : vecW) {
            if (isIn(h.first, v.second.first, v.second.second) && isIn(v.first, h.second.first, h.second.second)) {
                if (f) {
                    edges.push_back(make_pair(v.first - crossings[k - 1].second - 1, make_pair(k - 1, k)));
                }
                crossings.push_back(make_pair(h.first, v.first));
                letters--;
                k++;
                f = true;
            }
        }
    }
    int vef = 0;
    pair<int, int> ves = make_pair(0, 0);
    for (auto c = crossings.begin(); c < crossings.end(); c++) {
        auto v = vecW.begin();
        while (v->first != c->second && v != vecW.end())
            v++;
        if (v == vecW.end())
            continue;
        //log << v->first << " " << v->second.first << " " << v->second.second << "; ";

        bool f = true;
        while (f) {
            while (!isIn(c->first, v->second.first, v->second.second)) {
                v++;
                if (v == vecW.end()) {
                    f = false;
                    break;
                }
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
                        //log << it->first - c->first - 1 << " " << c - crossings.begin() << " " << it - crossings.begin() << endl;
                        break;
                    }
            v++;
            if (v == vecW.end())
                break;
            f = v->first == c->second;
        }
        //log << v->first << " " << v->second.first << " " << v->second.second << endl;
    }

}


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

    sort(g.begin(), g.end(), lambda);

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


int main() {


    time_start();
    inputWords("input.txt");
    cerr << time_stop()/1000.0 << endl;

    time_start();
    makeGraph();
    cerr << time_stop()/1000.0 << endl;


    time_start();
    Kruskal(edges);
    cerr << time_stop()/1000.0 << endl;

    ofstream fout("output.txt");
    fout << letters;
    fout.close();

    //cout << (getCPUTime() - start) << endl;

    return 0;
}