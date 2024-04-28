#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<cassert>
#include <functional>
#include <queue>

using namespace std;

struct edge {
    int v;
    multiset<double> w;
};

enum class Type{
    MAKE_SET, UNION
};

class Persistent_DSU {
private:
    map<double, pair<Type, pair<int, int>>> events;
    map<int, map<int, edge>> adj;
    void make_set(int u) { adj[u]; }
    void union_set(int u, int v, double time)
    {
        adj[u][v].w.insert(time);
        adj[v][u].w.insert(time);
        events[time] = { Type::UNION, { u, v } };
    }

public:
    void INSERT(Type t, int u, int v = -1, double time = 0.0) {
        if (t == Type::MAKE_SET)
            make_set(u);
        else if (t == Type::UNION)
            union_set(u, v, time);
        else
            assert(false);
    }

    void DELETE(double time) {
        auto it = events.find(time);
        if (it == events.end())
            return;
        Type t = it->second.first;
        int u = it->second.second.first;
        int v = it->second.second.second;
        if (t == Type::MAKE_SET)
            adj.erase(u);
        else if (t == Type::UNION) {
            adj[u][v].w.erase(adj[u][v].w.find(time));
            adj[v][u].w.erase(adj[v][u].w.find(time));
            if (adj[u][v].w.empty())
                adj[u].erase(v);
            if (adj[v][u].w.empty())
                adj[v].erase(u);
        }
        events.erase(it);
    }
    int QUERY(double time, int u, int v) {
        map<int, bool> vis;
        function<bool(int, double)> dfs = [&](int u, double tm) {
            if (u == v)
                return (tm <= time);
            vis[u] = true;
            for (auto [v, e] : adj[u]) {
                if (vis[v])
                    continue;
                for (auto t : e.w) {
                    if (dfs(v, max(tm, t)))
                        return true;
                }
            }
            return false;
        };
        return dfs(u, 0.0);
    } // complexity is exponential
    int QUERYFast(double time, int u, int v) {
        vector<int> dist(1e5, 1e9);
        priority_queue<pair<int, int>> pq; pq.push({ 0, u });
        dist[u] = 0;
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (u == v)
                return (d <= time);
            if (d > dist[u])
                continue;
            for (auto [v, e] : adj[u]) {
                for (auto t : e.w) {
                    if (dist[v] > max(d, (int)t)) {
                        dist[v] = max(d, (int)t);
                        pq.emplace( dist[v], v );
                    }
                }
            }
        }
        return false;
    } // better than the previous, we use dijkstra to dind the min on the path between u, and v, still not optimal!
};

string pr(int u, int v, double time, bool ok) {
    if(ok) return "At time: " + to_string(time) + " u: " + to_string(u) + " and v: " + to_string(v) + " are connected";
    return "At time: " + to_string(time) + " u: " + to_string(u) + " and v: " + to_string(v) + " are not connected";
}

void test() {
    Persistent_DSU dsu;
    dsu.INSERT(Type::MAKE_SET, 1, -1, 1);
    dsu.INSERT(Type::MAKE_SET, 2, -1, 2);
    dsu.INSERT(Type::MAKE_SET, 3, -1, 3);
    cout << pr(1, 2, 4, dsu.QUERYFast(4, 1, 2)) << endl;
    dsu.INSERT(Type::UNION, 1, 2, 4);
    cout << pr(1, 2, 5, dsu.QUERYFast(5, 1, 2)) << endl;
    dsu.DELETE(4);
    cout << pr(1, 2, 5, dsu.QUERYFast(5, 1, 2)) << endl;
    dsu.INSERT(Type::UNION, 1, 3, 4);
    cout << pr(1, 2, 5, dsu.QUERY(5, 1, 2)) << endl;
    cout << pr(1, 3, 5, dsu.QUERY(5, 1, 3)) << endl;
}

int main() {
    test();
    return 0;
}
