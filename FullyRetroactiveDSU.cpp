#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;

#define fastIO ios::sync_with_stdio(0), cin.tie(0), cout.tie(0)
#define dbg(x) cerr << #x << " = " << x << endl;

const ld DEFAULT = -1;
const int mxN = 2e5 + 2;

struct Node
{
    int child[2] = {0, 0}, parent = 0; // left, right, parent
    ld val = DEFAULT, path = DEFAULT;  // Node value, path
    bool flip = 0;                     // Lazy propagation for reversing subtree
};

class SplayTree
{

protected:
    vector<Node> T; // Tree
    SplayTree(int n) : T(n + 1) {}

    // Push lazy propagation, to be called before exposeing children
    void push_children(int x)
    {
        if (!x || !T[x].flip)
            return;
        int l = T[x].child[0], r = T[x].child[1];
        T[l].flip ^= 1, T[r].flip ^= 1;
        swap(T[x].child[0], T[x].child[1]);
        T[x].flip = 0;
    }

    // Update path value of node x
    void update_node(int x)
    {
        int l = T[x].child[0], r = T[x].child[1];
        push_children(l), push_children(r);
        T[x].path = max({T[l].path, T[r].path, T[x].val});
    }

    // Set child of x to y at direction d
    void set_child(int x, int d, int y)
    {
        T[x].child[d] = y;
        T[y].parent = x;
        update_node(x);
    }

    // find side of x relative to its parent
    int side(int x)
    {
        int p = T[x].parent;
        if (!p)
            return -1;
        if (T[p].child[0] == x)
            return 0;
        if (T[p].child[1] == x)
            return 1;
        return -1;
    }

    // Rotate x with its parent
    void rotate(int x)
    {
        int y = T[x].parent, g = T[y].parent, dx = side(x), dy = side(y);
        set_child(y, dx, T[x].child[!dx]);
        set_child(x, !dx, y);
        if (dy != -1)
            set_child(g, dy, x);
        else
            T[x].parent = g;
    }

    // splay x to root
    void splay(int x)
    {
        push_children(x);
        while (side(x) != -1)
        {
            int p = T[x].parent, g = T[p].parent;
            push_children(g), push_children(p), push_children(x);
            int dx = side(x), dy = side(p);
            if (dy != -1)
                rotate((dx == dy) ? p : x);
            rotate(x);
        }
    }
};

class Link_Cut_Tree : SplayTree
{
protected:
    int nodeCount = (mxN >> 1);
    int edges[mxN] = {0};

    // Make the path from the root node to x a preferred path
    int expose(int x)
    {
        int last = 0, y = x;
        while (y)
        {
            splay(y);
            T[y].child[1] = last;
            push_children(y);
            last = y;
            y = T[y].parent;
        }
        splay(x);
        return last;
    }

    // makes x the root of the represented tree
    void evert(int x)
    {
        expose(x);
        T[x].flip ^= 1;
        push_children(x);
    }

    // add the Edge between x and y
    void link(int u, int v)
    {
        evert(u);
        expose(v);
        T[u].parent = v;
        push_children(v);
    }

    // remove the Edge between x and y
    void cut(int u, int v)
    {
        evert(u);
        expose(v);
        T[v].child[0] = T[u].parent = 0;
        push_children(v);
    }

public:
    Link_Cut_Tree(int n) : SplayTree(n) {}

    // remove the Edge between x and y
    void Cut(int u, int v)
    {
        if (u > v)
            swap(u, v);
        cut(u, edges[u]);
        edges[u] = edges[v] = 0;
    }

    // returns the maximum value on the path from u to v
    ld PathQuery(int u, int v)
    {
        evert(u);
        expose(v);
        return T[v].path;
    }

    // update the value of node u to v
    void Update(int u, ld v)
    {
        expose(u);
        T[u].val = v;
        update_node(u);
    }

    // returns the root of the represented tree that contains u
    int find(int u)
    {
        expose(u);
        while (T[u].child[0])
            u = T[u].child[0];
        return u;
    }

    void Link(int u, int v, ld w = 1)
    {
        if (u > v)
            swap(u, v);
        link(u, nodeCount);
        link(v, nodeCount);
        Update(nodeCount, w);
        edges[u] = edges[v] = nodeCount++;
    }
};

struct Fully_Retroactive_DSU : Link_Cut_Tree
{
    multiset<pair<ld, pair<int, int>>> event;
    Fully_Retroactive_DSU() : Link_Cut_Tree(mxN) {}

    void UNION(int u, int v, ld time)
    {
        assert(find(u) != find(v));
        event.insert({time, {u, v}});
        Link(u, v, time);
    }

    void ERASE(ld time)
    {
        int u = event.lower_bound({time, {0, 0}})->second.first;
        int v = event.lower_bound({time, {0, 0}})->second.second;
        assert(find(u) == find(v));
        Cut(u, v);
        event.erase(event.find({time, {u, v}}));
    }

    bool QUERY(int u, int v, ld time)
    {
        return find(u) == find(v) && PathQuery(u, v) <= time;
    }
};

void solve()
{
    int n, choice;
    cout << "Welcome to the fully retroactive DSU.\n";

    Fully_Retroactive_DSU dsu;

    while (true)
    {
        cout << "\n1. Union\n2. Erase\n3. Query\n4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        int u, v;
        long double time;

        switch (choice)
        {
        case 1:
            cout << "Enter the nodes to union (u v) and time: ";
            cin >> u >> v >> time;
            dsu.UNION(u, v, time);
            cout << "Union performed.\n";
            break;

        case 2:
            cout << "Enter the time of the event to erase: ";
            cin >> time;
            dsu.ERASE(time);
            cout << "Event erased.\n";
            break;

        case 3:
            cout << "Enter the nodes to query (u v) and time: ";
            cin >> u >> v >> time;
            if (dsu.QUERY(u, v, time))
            {
                cout << "Nodes are connected as of time " << time << ".\n";
            }
            else
            {
                cout << "Nodes are not connected as of time " << time << ".\n";
            }
            break;

        case 4:
            return;

        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

int main()
{
    // fastIO;
    solve();
    return 0;
}