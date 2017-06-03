#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <set>
#include <queue>


using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::pair;
using std::ifstream;
using std::ofstream;
using std::make_pair;


const double eps = 1E-3;


struct Line {
    double k;
    double b;
    enum Orientation {NORMAL, VERTICAL}orient;

    Orientation toOrient(int o) {
        return o ? VERTICAL : NORMAL;
    }

    Line() : k(0), b(0) {}

    Line(double kk, double bb, int orientation) : k(kk), b(bb), orient(toOrient(orientation)) {}

    const bool operator==(const Line l) const {
        return k == l.k && b == l.b & orient == l.orient;
    }

    //10000 - сигнал совпадения
    //100000 - сигнал параллельности
    pair<double, double> findIntersec(Line l) {
        bool f = orient == NORMAL, g = l.orient == NORMAL;
        if (f && g) {
            if (fabs(k - l.k) < eps && fabs(b - l.b) > eps)
                return make_pair(100000.0, 100000.0);
            if (fabs(k - l.k) < eps && fabs(b - l.b) < eps)
                return make_pair(10000.0, 10000.0);

            double x0 = (b - l.b) / (l.k - k);
            return make_pair(x0, k * x0 + b);
        }
        else {
            if (f ^ g) {
                if (f)
                    return make_pair(l.b, k * l.b + b);
                else
                    return make_pair(b, l.k * b + l.b);
            }
            else return fabs(b - l.b) < eps ? make_pair(10000.0, 10000.0) : make_pair(100000.0, 100000.0);
        }
    }

};

bool between(double a, double b1, double b2) {
    return b1 - a >= 0 && a - b2 >= 0 || a - b1 >= 0 && b2 - a >= 0;
}

bool compareDouble(double a, double b) {
    if (fabs(a - b) < eps)
        return false;
    else
        return a < b;
}

struct Point {
    double first;
    double second;
    
    Point() : first(0), second(0) {}

    Point(double a, double b) : first(a), second(b) {}

    Point(pair<double, double> pnt) : first(pnt.first), second(pnt.second) {}

    bool in(pair<pair<Point, Point>, Line> r) const {
        bool f;
        if (r.second.orient)
            f = fabs(r.second.b - first) < eps;
        else
            f =  fabs((first*r.second.k + r.second.b) - second) < eps;
        if (f)
            f = between(first, r.first.first.first, r.first.second.first) &&
                    between(second, r.first.first.second, r.first.second.second);
        return f;
    }

    Point& operator=(const Point& pnt) {
        first = pnt.first;
        second = pnt.second;
        return *this;
    }
    Point& operator=(const pair<double, double> pnt) {
        first = pnt.first;
        second = pnt.second;
        return *this;
    }
    const bool operator==(const Point& pnt) const {
        return compareDouble(fabs(first - pnt.first), eps) && compareDouble(fabs(second - pnt.second), eps);
    }
    const bool operator<(const Point& pnt) const {
        return (fabs(first - pnt.first) < eps ? compareDouble(second, pnt.second) : compareDouble(first, pnt.first));
    }
    Point operator-(const Point& p) {
        return Point(this->first - p.first, this->second - p.second);
    }
};


vector<vector<double>> d;
map<Point, pair<int, vector<int>>> vert;
vector<vector<int>> G;
vector<int> p;
vector<Point> point;
vector<pair<pair<Point, Point>, Line>> roads;
unsigned int N, vertNum;
int begin, end;
Point beg, en;


template<class T>
void push(vector<T>& v, T& value) {
    for (auto el : v)
        if (value == el)
            return;
    v.emplace_back(value);
}


void inputGraph(ifstream& fin) {
    fin >> N;
    roads.resize(N);

    int cnt = -1;
    int x1, x2, y1, y2, orient = 0;
    double k, b;
    for (int i = 0; i < N; i++) {
        fin >> x1 >> y1 >> x2 >> y2;
        orient = 0;
        if (x2 == x1) {
            orient = 1;
            b = x1;
            k = 0;
        }
        else if (y2 == y1) {
            b = y1;
            k = 0;
        }
        else {
            k = double(y2 - y1) / (x2 - x1);
            b = double(x2 * y1 - x1 * y2) / (x2 - x1);
        }

        Point p1 = Point(x1, y1);
        Point p2 = Point(x2, y2);
        if (p2 < p1)
            std::swap(p1, p2);
        roads[i] = make_pair(make_pair(p1, p2), Line(k, b, orient));
        if (vert.find(p1) == vert.end())
            vert.emplace(p1, make_pair(++cnt, vector<int>{i})), point.push_back(p1);
        else
            vert[p1].second.push_back(i);
        if (vert.find(p2) == vert.end())
            vert.emplace(p2, make_pair(++cnt, vector<int>{i})), point.push_back(p2);
        else
            vert[p2].second.push_back(i);
    }

    Point __nul(100000.0, 100000.0);
    Point __same(10000.0, 10000.0);
    
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            if (roads[i] == roads[j])
                continue;
            Point intersec = roads[i].second.findIntersec(roads[j].second);
            if (intersec == __nul)
                continue;
            if (intersec == __same) {
                Point aa1 = roads[i].first.first, bb1 = roads[i].first.second;
                if (aa1.in(roads[j]) || bb1.in(roads[j])) {
//Прямые совпадают, отрезки имеют общую часть
                }
                else continue;
            }

            if (intersec.in(roads[i]) && intersec.in(roads[j]))
                if (vert.find(intersec) == vert.end()) {
                    vert.emplace(intersec, make_pair(++cnt, vector<int>{i, j})), point.push_back(intersec);
                }
                else {
                    if (vert[intersec].second.front() == i)
                        push(vert[intersec].second, j);
                }
        }

    fin >> x1 >> y1 >> x2 >> y2;
    beg = Point(x1, y1);
    en = Point(x2, y2);
    fin.close();
}

void emplaceAfter(Point beg) {
    vector<int> tmp;
    for (int i = 0; i < N && tmp.empty(); i++)
        if (beg.in(roads[i]))
            tmp.push_back(i);
    vector<pair<Point, int>> adjacentVert;
    for (auto v : vert)
        if (find(v.second.second.begin(), v.second.second.end(), tmp[0]) != v.second.second.end()) {
            adjacentVert.push_back(make_pair(v.first, v.second.first));
        }
    int closest = adjacentVert[0].second;
    double dist = fabs(adjacentVert[0].first.first - beg.first) + fabs(adjacentVert[0].first.second - beg.second);
    for (auto v : adjacentVert)
        if (fabs(v.first.first - beg.first) + fabs(v.first.second - beg.second) < dist) {
            closest = v.second;
            dist = fabs(v.first.first - beg.first) + fabs(v.first.second - beg.second);
        }
    vert.emplace(beg, make_pair(vert.size(), tmp));
    point.push_back(beg);
    G.push_back(vector<int>{closest});
    G[closest].push_back(G.size() - 1);
}

void makeGraph() {

    vector<int> pr(N, -1);



    G.resize(point.size());
    vector<int> tmp;
    for (auto v : vert) {
        tmp = v.second.second;
        for (auto r : tmp) {
            if (pr[r] > -1)
                G[v.second.first].push_back(pr[r]), G[pr[r]].push_back(v.second.first);
            pr[r] = v.second.first;
        }
    }

    if (vert.find(beg) == vert.end())
        emplaceAfter(beg);
    if (vert.find(en) == vert.end())
        emplaceAfter(en);
    begin = vert[beg].first;
    end = vert[en].first;

    N = vert.size();
    p.resize(N, -1);
}

const int INF = 100000000;

double min(vector<double> dd) {
    double m = dd[0];
    for (auto el : dd)
        if (el < m)
            m = el;
    return m;
}


struct V {
    int v, prev;
    double w;

    V(int vv, int p, double ww) : v(vv), prev(p), w(ww) {}

    inline bool operator<(V v) const {
        return w > v.w;
    }
};

void dijkstra() {
    int s = begin; // стартовая вершина

    ofstream log("log.txt");

    d.assign(point.size(), vector<double>(point.size(), INF));
    d[s].assign(N, 0);
    std::priority_queue<V> q;
    vector<std::set<int>> used(point.size(), std::set<int>());
    q.push({s, -1, 0});

    while (!q.empty()) {
        V v = q.top();
        q.pop();
        log << "____" << v.v << "____" << endl;
        if (v.prev != -1) {
            if (used[v.v].find(v.prev) != used[v.v].end()) {
                log << v.prev << "-" << v.v << " is used;" << endl;
                continue;
            }
            used[v.v].insert(v.prev);
        }

        Point p1 = point[v.v] - point[v.prev];
        for (size_t j = 0; j < G[v.v].size(); ++j) {
            int to = G[v.v][j];
            double angle = 0;
            double dprev = 0;
            if (v.prev != -1 && v.prev != to) {
                Point p2 = point[to] - point[v.v];
                angle = (p1.first*p2.first + p1.second*p2.second) / sqrt(((p1.first*p1.first + p1.second*p1.second)
                                                                          * (p2.first*p2.first + p2.second*p2.second)));
                if (fabs(angle) > 1)
                    angle = angle < 0 ? -1 : 1;
                angle = acos(angle)*180.0/M_PI;
                dprev = d[v.v][v.prev];
            }
            log << v.prev << "-" << v.v << "-" << to << ": angle is " << angle;
            if (angle > 180 - eps)
                angle = 0;
            if (dprev + angle < d[to][v.v] && v.prev != to) {
                log << ", updating dist with " << dprev + angle;
                d[to][v.v] = dprev + angle;
                q.push({to, v.v, d[to][v.v]});
            }
            log << endl;
        }
    }

    log.close();
}

int main() {

    ifstream fin("input.txt");
    inputGraph(fin);
/*
    Point p1(18, 22), p2(18-0.00000000001, 22-0.00000000001);

    cout << (!(p1 < p2) && !(p2 < p1)) << endl;
*/
    makeGraph();

    dijkstra();

    ofstream fout("output.txt");
    double res = min(d[end]);
    fout << (res < INF ? res : -1);
    fout.close();

    return 0;
}