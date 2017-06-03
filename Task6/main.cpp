#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <algorithm>


typedef unsigned int ui;

using std::cout;
using std::vector;
using std::pair;
using std::make_pair;
using std::ifstream;
using std::ofstream;
using std::set;
using std::find;


ui N, K, NdblSqr;

struct Edge {
    ui u, v;
    int cap, flow;

    Edge() : u(0), v(0), cap(0), flow(0) {}

    Edge(ui uu, ui vv) : u(uu), v(vv), cap(1), flow(0) {}

    Edge(ui uu, ui vv, int c, int f) : u(uu), v(vv), cap(c), flow(f) {}
};

struct Element {
    ui x, y;
    ui num;

    Element() : x(0), y(0) {}

    Element(ui x, ui y) : x(x), y(y), num(x*N + y) {}

    Element(ui n) : x(n / N), y(n % N), num(n) {}

    void numerate() {
        num = x * N + y;
    }

    bool operator==(const Element& el) {
        return num == el.num;
    }
    bool operator<(const Element& el) {
        return num < el.num;
    }
};
typedef Element El;


vector<int> q, d, ptr;
vector<ui> elX, elY, conX, conY;
vector<El> el, con;
vector<vector<ui>> G;
vector<Edge> E;
vector<int> pred;
vector<int> predLast;


void inputGraph(ifstream& fin) {
    fin >> N >> K;

    elX.resize(K, 0);
    elY.resize(K, 0);
    conX.resize(K, 0);
    conY.resize(K, 0);
    el.resize(K, Element());
    con.resize(K, Element());
    NdblSqr = 2*N*N;
    G.assign(NdblSqr + 2, vector<ui>(0));

    q.resize(NdblSqr + 2, 0);
    ptr.resize(NdblSqr + 2, 0);
    d.resize(NdblSqr + 2, -1);

    for (size_t i = 0; i < K; i++) {
        fin >> elX[i];
        el[i].x = elX[i];
    }
    for (size_t i = 0; i < K; i++) {
        fin >> elY[i];
        el[i].y = elY[i];
        el[i].numerate();
    }
    for (size_t i = 0; i < K; i++) {
        fin >> conX[i];
        con[i].x = conX[i];
    }
    for (size_t i = 0; i < K; i++) {
        fin >> conY[i];
        con[i].y = conY[i];
        con[i].numerate();
    }
}

void add_edge(ui a, ui b) {
    Edge e1(a, b, 1, 0 );
    Edge e2(b, a, 0, 0);
    G[a].push_back(E.size());
    E.push_back(e1);
    G[b].push_back(E.size());
    E.push_back(e2);
}
void addAdjacency(const pair<int, int>& shift, const Element& el) {

    ui Nsqr = NdblSqr/2;
    if (! ((el.x + shift.first < 0) || (el.x + shift.first > N - 1) || (el.y + shift.second < 0) || (el.y + shift.second > N - 1)) ) {
        Element low(el.num + Nsqr);
        Element high(el.x + shift.first, el.y + shift.second);
        high.numerate();
        add_edge(low.num, high.num);
    }

}

void makeGraph() {

    vector<pair<int, int>> shifts = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

    for (size_t i = 0; i < N; ++i) {
        ui iN = i*N;
        for (size_t j = 0; j < N; ++j) {
            Element el(i, j);
            add_edge(el.num, el.num + NdblSqr/2);
            for (auto shift : shifts)
                addAdjacency(shift, el);
        }
    }

    for (size_t i = 0; i < NdblSqr/2; ++i) {
        if (find(con.begin(), con.end(), Element(i)) != con.end())
            add_edge(NdblSqr, i); // s - v связи
        if (find(el.begin(), el.end(), Element(i)) != el.end())
            add_edge(i + NdblSqr/2, NdblSqr + 1); // v - t связи
    }

    pred.resize(NdblSqr + 1, -1);

}


const int INF = 1000000000;

bool bfs() {
    int qh=0, qt=0;
    q[qt++] = NdblSqr;
    d.assign(NdblSqr + 2, -1);
    d[NdblSqr] = 0;
    while (qh < qt && d[NdblSqr + 1] == -1) {
        int v = q[qh++];
        for (size_t i=0; i<G[v].size(); ++i) {
            int id = G[v][i],
                    to = E[id].v;
            if (d[to] == -1 && E[id].flow < E[id].cap) {
                q[qt++] = to;
                d[to] = d[v] + 1;
                if (to == NdblSqr + 1)
                    predLast.push_back(v);
                else
                    pred[to] = v;
            }
        }
    }
    return d[NdblSqr + 1] != -1;
}

int dfs (int v, int flow) {
    if (!flow)  return 0;
    if (v == NdblSqr + 1)  return flow;
    for (; ptr[v]<(int)G[v].size(); ++ptr[v]) {
        int id = G[v][ptr[v]],
                to = E[id].v;
        if (d[to] != d[v] + 1)  continue;
        int pushed = dfs (to, std::min(flow, E[id].cap - E[id].flow));
        if (pushed) {
            E[id].flow += pushed;
            E[id^1].flow -= pushed;
            return pushed;
        }
    }
    return 0;
}

int dinic() {
    int flow = 0;
    for (;;) {
        if (!bfs())  break;
        ptr.assign(NdblSqr + 2, 0);
        while (int pushed = dfs (NdblSqr, INF))
            flow += pushed;
    }
    return flow;
}

void printConnections(ofstream& fout) {

    int Nsqr = NdblSqr/2;
    int s = NdblSqr, t = NdblSqr + 1;
    Edge e;
    for (auto id : G[s]) {
        e = E[id];
        vector<int> path;
        while (e.flow == 1 && e.v != t) {
            if (e.v < Nsqr)
                path.push_back(e.v);
            for (auto i : G[e.v])
                if (E[i].flow == 1) {
                    e = E[i];
                    break;
                }
        }
        std::reverse(path.begin(), path.end());
        for (auto p : path)
            fout << p << " ";
        fout << std::endl;
    }
}


int main() {

    ifstream fin("input.txt");

    inputGraph(fin);
    fin.close();

    makeGraph();

    ofstream fout("output.txt");

    if (dinic() < K)
        fout << -1;
    else
        printConnections(fout);

    fout.close();

    return 0;
}