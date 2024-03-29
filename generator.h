#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <functional>
#include <iostream>
#include <random>
#include <vector>

#define _debug(x) cerr << #x << " = " << (x) << " "
#define _debugln(x) cerr << #x << " = " << (x) << endl
#define _debugf(...) fprintf(stderr, __VA_ARGS__)
#define _wlog(fmt, ...) \
    fprintf(stderr, "<%s:%d> " fmt "\n", __func__, __LINE__, ## __VA_ARGS__)

namespace gen {

using namespace std;

using T = long long;

#define endline() putchar('\n')

void write(T x) {
    if (x < 0) putchar('-'), x = -x;
    T y = 10, len = 1;
    while (y <= x) y *= 10, len++;
    while (len--) y /= 10, putchar(x / y + 48), x %= y;
}

void print(const char &c) {
    putchar(c);
}

void print(const string &s) {
    cout << s;
}

template<typename T>
void print(const T& x) {
    write(x), putchar(' ');
}

template<typename T>
void print(const vector<T>& a) {
    for (auto i : a) print(i);
    endline();
}

template<typename T, typename ...T2>
void print(const T& cur, const T2& ...rest) {
    print(cur);
    print(rest...);
}

struct range {
    T l, r;
    T size() const { return r - l + 1; }
    range(const T& x) { l = 0, r = x - 1; }
    range(const T& _l, const T& _r) { l = _l, r = _r;}
    range(const initializer_list<T>& a) {
        l = *(a.begin());
        r = *(next(a.begin()));
    }
};

mt19937 rnd((random_device())());

T randint(const range& a) {
    uniform_int_distribution<T> distrib(a.l, a.r);
    return distrib(rnd);
}
T randint(T l, T r) {
    return randint(range(l, r));
}

// ======================================================== List =========================================================

namespace limits {
    const int unique = 1 << 1;
    const int decrease = 1 << 2;
    const int increase = 1 << 3;
}  // namespace limits

vector<T> get_list(const int&, const range&, int = 0);
vector<T> get_list(const int&, function<T(const vector<T>&)>);
vector<T> get_increase_list(const int&, const range&);
vector<T> get_decrease_list(const int&, const range&);
vector<T> get_unique_list(const int&, const range&);
vector<T> get_non_decrease_list(const int&, const range&);
vector<T> get_non_increase_list(const int&, const range&);

vector<T> get_list(const int& size, const range& a, int attr) {
    assert(size > 0);
    if (attr & limits::decrease) {
        if (attr & limits::unique)
            return get_decrease_list(size, a);
        else
            return get_non_increase_list(size, a);
    }
    if (attr & limits::increase) {
        if (attr & limits::unique)
            return get_increase_list(size, a);
        else
            return get_non_decrease_list(size, a);
    }
    if (attr & limits::unique) {
        return get_unique_list(size, a);
    }
    vector<T> res;
    uniform_int_distribution<T> distrib(a.l, a.r);
    for (int i = 0; i < size; i++) { res.push_back(distrib(rnd)); }
    return res;
}

vector<T> get_list(const int& size, function<T(const vector<T>&)> generate) {
    assert(size >= 0);
    vector<T> res;
    for (int i = 0; i < size; i++) { res.push_back(generate(res)); }
    return res;
}

vector<T> get_increase_list(const int& size, const range& val) {
    assert(val.size() >= size);
    auto generate = [val, size](const vector<T>& cur) -> T {
        T prev;
        if (cur.size())
            prev = cur[cur.size() - 1];
        else
            prev = val.l - 1;
        T l = prev + 1;
        T r = max(prev + 1, val.l + (T)(((cur.size() + 2) * 1.0 / size) * (val.r - val.l)));
        r = min(r, (T)(val.r - (size - cur.size() - 1)));
        return randint(l, r);
    };
    vector<T> res = get_list(size, generate);
    return res;
}

vector<T> get_decrease_list(const int& size, const range& val) {
    vector<T> res = get_increase_list(size, val);
    reverse(res.begin(), res.end());
    return res;
}

vector<T> get_non_decrease_list(const int& size, const range& val) {
    vector<T> res = get_list(size, val);
    sort(res.begin(), res.end());
    return res;
}

vector<T> get_non_increase_list(const int& size, const range& val) {
    vector<T> res = get_non_decrease_list(size, val);
    reverse(res.begin(), res.end());
    return res;
}

vector<T> get_unique_list(const int& size, const range& val) {
    vector<T> res = get_increase_list(size, val);
    shuffle(res.begin(), res.end(), default_random_engine((random_device())()));
    return res;
}

// ======================================================== Graph =======================================================

struct edge {
    int from, to;
    T val;
    edge(int x, int y, T v = 0) : from(x),
                                  to(y),
                                  val(v) {}
};

class graph {
public:
    bool has_edge_val, has_node_val;
    int n, m;
    vector<edge> edges;
    vector<T> node;
    void add_node_weight(const range&, int = 0);
    void add_edge_weight(const range&, int = 0);
};

void graph::add_edge_weight(const range& val_range, int attr) {
    has_edge_val = 1;
    auto edge_val = get_list(m, val_range, attr);
    for (int i = 0; i < m; i++) {
        edges[i].val = edge_val[i];
    }
}

void graph::add_node_weight(const range& val_range, int attr) {
    has_node_val = 1;
    node = get_list(n, val_range, attr);
}

class tree : public graph {
public:
    tree();
    tree(const int& size);
};

class dag : public graph {
public:
    dag();
    dag(const int& size);
    dag(const int& size, const int& edge_cnt);
};

tree get_tree(const int& size) {
    return tree(size);
}

tree::tree() {
    has_node_val = has_edge_val = n = 0;
}

//TODO: optimize copy reference

tree::tree(const int& size) {
    n = size;
    m = size - 1;
    has_edge_val = 0;
    has_node_val = 0;
    vector<T> id(n);
    for (int i = 0; i < n; i++) {
        id[i] = i + 1;
    }
    shuffle(id.begin(), id.end(), default_random_engine((random_device())()));
    for (int i = 1; i < size; i++) {
        int j = randint(0, i - 1);
        edges.push_back(edge(id[j], id[i]));
    }
    shuffle(edges.begin(), edges.end(), default_random_engine((random_device())()));
}

dag get_dag(const int& size) {
    return dag(size);
}

dag get_dag(const int& size, const int& edge_size) {
    return dag(size, edge_size);
}

dag::dag() {
    has_node_val = has_edge_val = n = 0;
}

dag::dag(const int& size) {
    n = size;
    m = 0;
    has_edge_val = has_node_val = 0;
    vector<T> id(n);
    for (int i = 0; i < n; i++) id[i] = i + 1;
    shuffle(id.begin(), id.end(), default_random_engine((random_device())()));
    for (int i = 1; i < size; i++) {
        int cnt = randint(0, i);
        m += cnt;
        auto targets = get_unique_list(cnt, {0, i - 1});
        for (auto t : targets) {
            edges.push_back(edge(id[i], id[t]));
        }
    }
    shuffle(edges.begin(), edges.end(), default_random_engine((random_device())()));
}

dag::dag(const int& size, const int& edge_size) {
    n = size, m = edge_size;
    has_edge_val = has_node_val = 0;
    vector<T> id(n);
    for (int i = 0; i < n; i++) id[i] = i + 1;
    shuffle(id.begin(), id.end(), default_random_engine((random_device())()));
    int res = m;
    for (int i = 1; i < n; i++) {
        //_debugf("i = %d:\n", i);
        //_debug(max(0, res - (i+n) * (n-i-1) / 2)), _debugln(min(i, res));
        int cnt = randint(max(0, res - (i+n) * (n-i-1) / 2), min(i, res));
        //_debugln(cnt);
        res -= cnt;
        auto targets = get_unique_list(cnt, {0, i - 1});
        //_debugln(targets.size());
        for (auto t : targets) {
            edges.push_back(edge(id[i], id[t]));
        }
    }
    shuffle(edges.begin(), edges.end(), default_random_engine((random_device())()));
}

void print(tree _t) {
    print(_t.n, '\n');
    for (auto e : _t.edges) {
        if (randint(0, 1)) swap(e.from, e.to);
        if (_t.has_edge_val)
            print(e.from, e.to, e.val, '\n');
        else
            print(e.from, e.to, '\n');
    }
}

void print(dag _d) {
    print(_d.n, _d.m, '\n');
    for (auto e : _d.edges) {
        if (_d.has_edge_val) {
            print(e.from, e.to, e.val, '\n');
        } else {
            print(e.from, e.to, '\n');
        }
    }
}

} // namespace gen
