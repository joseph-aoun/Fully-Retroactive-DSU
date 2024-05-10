#include <bits/stdc++.h>
using namespace std;
typedef long long ld;

#define fastIO ios::sync_with_stdio(0), cin.tie(0), cout.tie(0)
#define dbg(x) cerr << #x << " = " << x << endl;

const ld DEFAULT = -1;
const int mxN = 4e5 + 2;

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
    void push_to_children(int u)
    {
        if (!u || !T[u].flip)
            return;
        int l = T[u].child[0], r = T[u].child[1];
        T[l].flip ^= 1, T[r].flip ^= 1;
        swap(T[u].child[0], T[u].child[1]);
        T[u].flip = 0;
    }

    // Update path value of node x
    void update_node_from_children(int u)
    {
        int l = T[u].child[0], r = T[u].child[1];
        push_to_children(l), push_to_children(r);
        T[u].path = max({T[l].path, T[r].path, T[u].val});
    }

    // Set child of x to y at direction d
    void set_child(int u, int side, int v)
    {
        T[u].child[side] = v;
        T[v].parent = u;
        update_node_from_children(u);
    }

    // find side of x relative to its parent
    int side(int u)
    {
        int parent = T[u].parent;
        if (!parent)
            return -1;
        if (T[parent].child[0] == u)
            return 0;
        if (T[parent].child[1] == u)
            return 1;
        return -1;
    }

    // Rotate x with its parent
    void rotate(int u)
    {
        int parent = T[u].parent, grandParent = T[parent].parent, x_side = side(u), y_side = side(parent);
        set_child(parent, x_side, T[u].child[!x_side]);
        set_child(u, !x_side, parent);
        if (y_side != -1)
            set_child(grandParent, y_side, u);
        else
            T[u].parent = grandParent;
    }

    // splay x to root
    void splay(int u)
    {
        push_to_children(u);
        while (side(u) != -1)
        {
            int parent = T[u].parent, g = T[parent].parent;
            push_to_children(g), push_to_children(parent), push_to_children(u);
            int x_side = side(u), parent_side = side(parent);
            if (parent_side != -1)
                rotate((x_side == parent_side) ? parent : u);
            rotate(u);
        }
    }
};

class Link_Cut_Tree : SplayTree
{
protected:
    int nodeCount = (mxN >> 1);
    int edges[mxN] = {0};

    // Make the path from the root node to x a preferred path
    int expose(int u)
    {
        int last = 0, y = u;
        while (y)
        {
            splay(y);
            T[y].child[1] = last;
            update_node_from_children(y);
            last = y;
            y = T[y].parent;
        }
        splay(u);
        return last;
    }

    // makes x the root of the represented tree
    void evert(int u)
    {
        expose(u);
        T[u].flip ^= 1;
        push_to_children(u);
    }

    // add the Edge between x and y
    void link(int u, int v)
    {
        evert(u);
        expose(v);
        T[u].parent = v;
        update_node_from_children(v);
    }

    // remove the Edge between x and y
    void cut(int u, int v)
    {
        evert(u);
        expose(v);
        T[v].child[0] = T[u].parent = 0;
        update_node_from_children(v);
    }

protected:
    Link_Cut_Tree(int n) : SplayTree(n) {}

    // remove the Edge between x and y
    void Cut(int u, int v)
    {
        if (u > v)
            swap(u, v);
        if (find(u) != find(v))
            return;
        cut(u, edges[u]);
        edges[u] = edges[v] = 0;
    }

    // returns the maximum value on the path from u to v
    ld PathQuery(int u, int v)
    {
        if (find(u) != find(v))
            return -1;
        evert(u);
        expose(v);
        if (u == v)
            return 0;
        return T[v].path;
    }

    // update the value of node u to v
    void Update(int u, ld v)
    {
        expose(u);
        T[u].val = v;
        update_node_from_children(u);
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
        if (find(u) == find(v))
            return;
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
        if (find(u) == find(v)) return;
        event.insert({time, {u, v}});
        Link(u, v, time);
    }

    void ERASE(ld time)
    {
        int u = event.lower_bound({time, {0, 0}})->second.first;
        int v = event.lower_bound({time, {0, 0}})->second.second;
        if(event.lower_bound({time, {0, 0}}) == event.end()) return;
        if(find(u) != find(v)) return;
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

void gen(int num_ops) {
    const int numNodes = 1e5;
    Fully_Retroactive_DSU DSU;
    stack<ld> tm;
    ld cnt = 0;
    for (int i = 0; i < num_ops; i++) {
        int op = rand() % 3;
        if (op == 0) {
            int u = rand() % numNodes + 1, v = rand() % numNodes + 1;
            ld time = ++cnt;
            tm.push(time);
            DSU.UNION(u, v, time);
        } else if (op == 1) {
            if(tm.empty()) continue;
            long double time = tm.top(); tm.pop();
            DSU.ERASE(time);
        } else {
            int u = rand() % numNodes + 1, v = rand() % numNodes + 1;
            long double time = rand() % (cnt + 1);
            DSU.QUERY(u, v, time);
        }
    }
}

void test_n_logn(int sz) {
    vector<int> nums(sz); 
    for(int i = 0; i < sz; i++) nums[i] = rand() % mxN;
    shuffle(nums.begin(), nums.end(), mt19937{random_device{}()});
    sort(nums.begin(), nums.end());
    shuffle(nums.begin(), nums.end(), mt19937{random_device{}()});
    sort(nums.begin(), nums.end());
}


#include <chrono>
vector<long double> time_taken;
vector<int> num_ops = {120000};
void stress_test() {
    for (int i = 0; i < num_ops.size(); i++) {
        auto start = chrono::high_resolution_clock::now();
        gen(num_ops[i]);
        auto end = chrono::high_resolution_clock::now();
        time_taken.push_back(chrono::duration_cast<chrono::milliseconds>(end - start).count());
        cout << "Time taken for " << num_ops[i] << " operations: " << time_taken[i] << "ms\n";
    }
}

void test() {
    for(int i = 0; i < num_ops.size(); i++) {
        auto start = chrono::high_resolution_clock::now();
        test_n_logn(num_ops[i]);
        auto end = chrono::high_resolution_clock::now();
        time_taken.push_back(chrono::duration_cast<chrono::milliseconds>(end - start).count());
    }
    for (int i = 0; i < num_ops.size(); i++) {
        cout << "Time taken for " << num_ops[i] << " n*log(n) operations: " << time_taken[i] << "ms\n";
    }
}

int main()
{
    freopen("output.txt", "w", stdout);
    srand(10);
    // test();
    // stress_test();
    return 0;
}