#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <queue>
#include <map>

using std::cout;
using std::endl;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::find;
using std::pair;
using std::make_pair;
using std::queue;
using std::map;


enum Shifts {RIGHT, DOWN, LEFT, UP, NONE};

class Vertex {
public:
    int i1, j1, i2, j2;

    Vertex() {
        i1 = -1;
        j1 = -1;
        i2 = -1;
        j2 = -1;
    }

    Vertex(int a, int b, int c, int d) {
        i1 = a;
        j1 = b;
        i2 = c;
        j2 = d;
    }

    void move(Shifts shift, bool isSecond) {
        switch (shift) {
            case RIGHT: {
                j1 += (1 - (int)isSecond)*1; j2 += (int)isSecond*1;
                break;
            }
            case LEFT: {
                j1 += (1 - (int)isSecond)*(-1); j2 += (int)isSecond*(-1);
                break;
            }
            case UP: {
                i1 += (1 - (int)isSecond)*(-1); i2 += (int)isSecond*(-1);
                break;
            }
            case DOWN: {
                i1 += (1 - (int)isSecond)*1; i2 += (int)isSecond*1;
                break;
            }
            default: {
                i1 = -1;
                j1 = -1;
                i2 = -1;
                j2 = -1;
            }
        }
    }

    bool operator==(Vertex v) const {
        return (this->i1 == v.i1) && (this->i2 == v.i2) && (this->j1 == v.j1) && (this->j2 == v.j2);
    }

    bool operator!=(Vertex v) const {
        return !(*this == v);
    }

    bool operator<(Vertex v) const {
        long d1 = i1 + 100*i2 + 10000 *j1 + 1000000 * j2;
        long d2 = v.i1 + 100 * v.i2 + 10000 * v.j1 + 1000000 * v.j2;
        return d1 < d2;
    }
};


Shifts shifts[4] = {RIGHT, DOWN, LEFT, UP};
map<Vertex, int> vertecies;
vector<vector<int>> G1, G2, G;
int N, numV;
unsigned int R1, C1, R2, C2;




Shifts reverseShift(Shifts shift) {
    switch (shift) {
        case RIGHT: return LEFT;
        case LEFT: return RIGHT;
        case UP: return DOWN;
        case DOWN: return UP;
        default: return NONE;
    }
}

Vertex add(Vertex v1, Vertex v2) {
    return Vertex((v1.i1 + v2.i1 >= N) ? N - 1 : v1.i1 + v2.i1, (v1.j1 + v2.j1 >= N) ? N - 1 : v1.j1 + v2.j1,
                  (v1.i2 + v2.i2 >= N) ? N - 1 : v1.i2 + v2.i2, (v1.j2 + v2.j2 >= N) ? N - 1 : v1.j2 + v2.j2);
}

Vertex sub(Vertex v1, Vertex v2) {
    return Vertex((v1.i1 - v2.i1 < 0) ? 0 : v1.i1 - v2.i1, (v1.j1 - v2.j1 < 0) ? 0 : v1.j1 - v2.j1,
                  (v1.i2 - v2.i2 < 0) ? 0 : v1.i2 - v2.i2, (v1.j2 - v2.j2 < 0) ? 0 : v1.j2 - v2.j2);
}

vector<vector<int>> operator+(vector<vector<int>> lhs, vector<vector<int>> rhs) {
    vector<vector<int>> res = {};
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

void inputGraphs(ifstream& fin) {
    fin >> N >> R1 >> C1 >> R2 >> C2;
    G1.resize(N);
    for (size_t i = 0; i < N; i++) {
        G1[i].resize(N);
        for (size_t j = 0; j < N; j++) {
            fin >> G1[i][j];
        }
    }
    G2.resize(N);
    for (size_t i = 0; i < N; i++) {
        G2[i].resize(N);
        for (size_t j = 0; j < N; j++) {
            fin >> G2[i][j];
        }
    }
    fin.close();
}

Vertex move(Vertex v, Shifts shift) {
    switch (shift) {
        case RIGHT: {
            return add(v, Vertex(0, 1, 0, 1));
        }
        case DOWN: {
            return add(v, Vertex(1, 0, 1, 0));
        }
        case LEFT: {
            return sub(v, Vertex(0, 1, 0, 1));
        }
        case UP: {
            return sub(v, Vertex(1, 0, 1, 0));
        }
        default:
            return Vertex();
    }
}

void makeGraph() {
    int cnt = 0;

    for (size_t i1 = 0; i1 < N; i1++)
        for (size_t j1 = 0; j1 < N; j1++)
            for (size_t i2 = 0; i2 < N; i2++)
                for (size_t j2 = 0; j2 < N; j2++) {
                    int x = 0;
                    if (!G1[i1][j1] && !G2[i2][j2])
                        x = cnt++;
                    else
                        continue;
                    vertecies.emplace(Vertex(i1, j1, i2, j2), x);
                }
    numV = cnt;

    G.resize(numV);
    for (auto& v : vertecies) {
        G[v.second].reserve(4);

        for (auto& shift : shifts) {

            Vertex to = move(v.first, shift);
            if (to != Vertex()) {
                if (G1[to.i1][to.j1] == 0 && G2[to.i2][to.j2] == 0) {
                    G[v.second].push_back(vertecies[to]);
                    continue;
                }
                if (G1[to.i1][to.j1] == 1 && G2[to.i2][to.j2] == 0) {
                    to.move(reverseShift(shift), 0);
                    if (v.first != to)
                        G[v.second].push_back(vertecies[to]);
                    continue;
                }
                if (G1[to.i1][to.j1] == 0 && G2[to.i2][to.j2] == 1) {
                    to.move(reverseShift(shift), 1);
                    if (v.first != to)
                        G[v.second].push_back(vertecies[to]);
                    continue;
                }
            }
        }
    }
}

int bfs_numSteps(int s) {
    queue<int> q;
    q.push(s);
    vector<bool> used;
    vector<int> d, p;
    used.resize(numV, false);
    d.resize(numV, 0);
    p.resize(numV, -1);
    used[s] = true;
    p[s] = -1;
    d[s] = 0;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (auto to : G[v]) {
            if (!used[to]) {
                used[to] = true;
                q.push(to);
                d[to] = d[v] + 1;
                p[to] = v;
            }
        }
    }
    return d[0];
}


int main() {

    ifstream fin("input.txt");
    inputGraphs(fin);

    makeGraph();

    ofstream fout("output.txt");

    int s = vertecies[Vertex(R1, C1, R2, C2)];
    fout << bfs_numSteps(s);

    fout.close();

    return 0;
}