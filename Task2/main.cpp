#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <queue>


using std::cout;
using std::endl;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::find;
using std::pair;
using std::make_pair;
using std::queue;


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

    const bool operator==(Vertex v) {
        return (this->i1 == v.i1) && (this->i2 == v.i2) && (this->j1 == v.j1) && (this->j2 == v.j2);
    }

    const bool operator!=(Vertex v) {
        return !(*this == v);
    }
};

Shifts shifts[4] = {RIGHT, DOWN, LEFT, UP};
vector<Vertex> vertecies;
vector<vector<int>> G1, G2, G;
int N;
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
        for (size_t j = 0; j < N; j++)
            fin >> G1[i][j];
    }
    G2.resize(N);
    for (size_t i = 0; i < N; i++) {
        G2[i].resize(N);
        for (size_t j = 0; j < N; j++)
            fin >> G2[i][j];
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
    vertecies.resize(N*N*N*N, Vertex());
    int cnt = 0;

    for (size_t i1 = 0; i1 < N; i1++)
        for (size_t j1 = 0; j1 < N; j1++)
            for (size_t i2 = 0; i2 < N; i2++)
                for (size_t j2 = 0; j2 < N; j2++) {
                    auto it = find(vertecies.begin(), vertecies.end(), Vertex(i1, j1, i2, j2));
                    int x = 0;
                    if (it != vertecies.end())
                        x = it - vertecies.begin();
                    else
                        x = cnt++;
                    vertecies[x] = Vertex(i1, j1, i2, j2);
                }

    G.resize(N*N*N*N);
    for (size_t v = 0; v < N*N*N*N; v++) {
        G[v].reserve(4);
        if (G1[vertecies[v].i1][vertecies[v].j1] == 1 || G2[vertecies[v].i2][vertecies[v].j2] == 1)
            continue;

        for (auto& shift : shifts) {
            Vertex to = move(vertecies[v], shift);
            if (to != Vertex()) {
                if (G1[to.i1][to.j1] == 0 && G2[to.i2][to.j2] == 0) {
                    G[v].push_back(find(vertecies.begin(), vertecies.end(), to) - vertecies.begin());
                    continue;
                }
                if (G1[to.i1][to.j1] == 1 && G2[to.i2][to.j2] == 0) {
                    to.move(reverseShift(shift), 0);
                    if (vertecies[v] != to)
                        G[v].push_back(find(vertecies.begin(), vertecies.end(), to) - vertecies.begin());
                    continue;
                }
                if (G1[to.i1][to.j1] == 0 && G2[to.i2][to.j2] == 1) {
                    to.move(reverseShift(shift), 1);
                    if (vertecies[v] != to)
                        G[v].push_back(find(vertecies.begin(), vertecies.end(), to) - vertecies.begin());
                    continue;
                }
            }
        }
    }
}

int bfs_numSteps(int s) {
    queue<int> q;
    q.push(s);
    vector<bool> used(N*N*N*N, false);
    vector<int> d(N*N*N*N), p(N*N*N*N);
    used[s] = true;
    p[s] = -1;
    d[s] = 0;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (auto to : G[v]) {
            if (!used[to]) {
                used[to] = true;
                q.push (to);
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

    int s = find(vertecies.begin(), vertecies.end(), Vertex(R1, C1, R2, C2)) - vertecies.begin();
    fout << bfs_numSteps(s);

    fout.close();

    return 0;
}