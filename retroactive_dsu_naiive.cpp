#include <bits/stdc++.h>
using namespace std;

struct edge
{
  int v;
  multiset<double> w;
};

enum class Type
{
  MAKE_SET,
  UNION
};

class Persistent_DSU
{
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
  void INSERT(Type t, int u, int v = -1, double time = 0.0)
  {
    if (t == Type::MAKE_SET)
      make_set(u);
    else if (t == Type::UNION)
      union_set(u, v, time);
    else
      assert(false);
  }
  void DELETE(double time)
  {
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
  int QUERY(double time, int u, int v)
  {
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
  }
};

int
main()
{
  return 0;
}
