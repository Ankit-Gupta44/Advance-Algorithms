///////////////////////////////////////////////////////////////////////////////////// CSES Path Queries /////////////////////////////////////////////////////////////////////////


#include <bits/stdc++.h>
using namespace std;

#define int long long

const int MAXN = 200005;
vector<int> tree[MAXN];
int values[MAXN]; // Values of the nodes
int parent[MAXN], depth[MAXN], heavy[MAXN], head[MAXN], pos[MAXN];
int cur_pos;
int segtree[4 * MAXN];
int n; // Number of nodes

// Segment Tree functions
void build_segment_tree(int node, int start, int end, const vector<int>& base_array) {
    if (start == end) {
        segtree[node] = base_array[start];
    } else {
        int mid = (start + end) / 2;
        build_segment_tree(2 * node, start, mid, base_array);
        build_segment_tree(2 * node + 1, mid + 1, end, base_array);
        segtree[node] = segtree[2 * node] + segtree[2 * node + 1];
    }
}

void update_segment_tree(int node, int start, int end, int idx, int value) {
    if (start == end) {
        segtree[node] = value;
    } else {
        int mid = (start + end) / 2;
        if (idx <= mid) {
            update_segment_tree(2 * node, start, mid, idx, value);
        } else {
            update_segment_tree(2 * node + 1, mid + 1, end, idx, value);
        }
        segtree[node] = segtree[2 * node] + segtree[2 * node + 1];
    }
}

int query_segment_tree(int node, int start, int end, int l, int r) {
    if (l > end || r < start) return 0; // Out of range
    if (l <= start && end <= r) return segtree[node];
    int mid = (start + end) / 2;
    return query_segment_tree(2 * node, start, mid, l, r) + query_segment_tree(2 * node + 1, mid + 1, end, l, r);
}

// DFS to calculate size and find heavy child
int dfs(int u) {
    int size = 1;
    int max_subtree = 0;
    for (int v : tree[u]) {
        if (v != parent[u]) {
            parent[v] = u;
            depth[v] = depth[u] + 1;
            int subtree_size = dfs(v);
            if (subtree_size > max_subtree) {
                max_subtree = subtree_size;
                heavy[u] = v;
            }
            size += subtree_size;
        }
    }
    return size;
}

// Decompose the tree and assign positions
void decompose(int u, int h, vector<int>& base_array) {
    head[u] = h;
    pos[u] = cur_pos++;
    base_array[pos[u]] = values[u];
    if (heavy[u] != -1) {
        decompose(heavy[u], h, base_array);
    }
    for (int v : tree[u]) {
        if (v != parent[u] && v != heavy[u]) {
            decompose(v, v, base_array);
        }
    }
}

// Query the path sum between two nodes
int query_path(int u, int v) {
    int result = 0;
    while (head[u] != head[v]) {
        if (depth[head[u]] < depth[head[v]]) swap(u, v);
        result += query_segment_tree(1, 0, n - 1, pos[head[u]], pos[u]);
        u = parent[head[u]];
    }
    if (depth[u] > depth[v]) swap(u, v);
    result += query_segment_tree(1, 0, n - 1, pos[u], pos[v]);
    return result;
}

// Update the value of a node
void update_value(int u, int value) {
    update_segment_tree(1, 0, n - 1, pos[u], value);
}

signed main() {
    cin >> n;
    int q;
    cin >> q;
    for (int i = 0; i < n; ++i) {
        cin >> values[i];
    }

    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        --u; --v;
        tree[u].push_back(v);
        tree[v].push_back(u);
    }

    memset(heavy, -1, sizeof(heavy));
    parent[0] = -1;
    depth[0] = 0;

    dfs(0);

    vector<int> base_array(n);
    cur_pos = 0;
    decompose(0, 0, base_array);

    build_segment_tree(1, 0, n - 1, base_array);

    while (q--) {
        int type;
        cin >> type;
        if (type == 2) {
            int u;
            cin >> u;
            --u;
            cout << query_path(0, u) << "\n";
        } else if (type == 1) {
            int u, value;
            cin >> u >> value;
            --u;
            update_value(u, value);
        }
    }

    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////// SPOJ QTREE - Query on a tree /////////////////////////////////////////////////////////////////////////



#include <bits/stdc++.h>
using namespace std;

const int MAXN = 10010;
const int INF = INT_MIN;

vector<pair<int, int>> adj[MAXN]; // adjacency list with (neighbor, edge index)
int parent[MAXN], depth[MAXN], heavy[MAXN], head[MAXN], pos[MAXN];
int edgeCost[MAXN], edgeIndex[MAXN], segTree[4 * MAXN];
int curPos;

int dfs(int u) {
    int size = 1, maxSubtree = 0;
    for (auto &[v, idx] : adj[u]) {
        if (v == parent[u]) continue;
        parent[v] = u;
        depth[v] = depth[u] + 1;
        edgeIndex[v] = idx; // Edge index corresponding to this child
        int subtree = dfs(v);
        if (subtree > maxSubtree) {
            maxSubtree = subtree;
            heavy[u] = v;
        }
        size += subtree;
    }
    return size;
}

void decompose(int u, int h) {
    head[u] = h;
    pos[u] = curPos++;
    if (heavy[u] != -1) {
        decompose(heavy[u], h);
    }
    for (auto &[v, idx] : adj[u]) {
        if (v != parent[u] && v != heavy[u]) {
            decompose(v, v);
        }
    }
}

void buildSegTree(int node, int start, int end) {
    if (start == end) {
        segTree[node] = edgeCost[start];
    } else {
        int mid = (start + end) / 2;
        buildSegTree(2 * node, start, mid);
        buildSegTree(2 * node + 1, mid + 1, end);
        segTree[node] = max(segTree[2 * node], segTree[2 * node + 1]);
    }
}

void updateSegTree(int node, int start, int end, int idx, int value) {
    if (start == end) {
        segTree[node] = value;
    } else {
        int mid = (start + end) / 2;
        if (idx <= mid) {
            updateSegTree(2 * node, start, mid, idx, value);
        } else {
            updateSegTree(2 * node + 1, mid + 1, end, idx, value);
        }
        segTree[node] = max(segTree[2 * node], segTree[2 * node + 1]);
    }
}

int querySegTree(int node, int start, int end, int l, int r) {
    if (r < start || l > end) return INF;
    if (l <= start && end <= r) return segTree[node];
    int mid = (start + end) / 2;
    return max(querySegTree(2 * node, start, mid, l, r),
               querySegTree(2 * node + 1, mid + 1, end, l, r));
}

int queryHLD(int a, int b) {
    int res = INF;
    while (head[a] != head[b]) {
        if (depth[head[a]] < depth[head[b]]) swap(a, b);
        res = max(res, querySegTree(1, 0, curPos - 1, pos[head[a]], pos[a]));
        a = parent[head[a]];
    }
    if (depth[a] > depth[b]) swap(a, b);
    if (a != b) {
        res = max(res, querySegTree(1, 0, curPos - 1, pos[a] + 1, pos[b]));
    }
    return res;
}

void solve() {
    int n;
    cin >> n;

    for (int i = 0; i < n; i++) {
        adj[i].clear();
        heavy[i] = -1;
    }

    vector<tuple<int, int, int>> edges;

    for (int i = 0; i < n - 1; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        --a, --b;
        adj[a].emplace_back(b, i);
        adj[b].emplace_back(a, i);
        edges.emplace_back(a, b, c);
    }

    curPos = 0;
    parent[0] = -1;
    depth[0] = 0;
    dfs(0);
    decompose(0, 0);

    for (int i = 0; i < n - 1; ++i) {
        auto [a, b, c] = edges[i];
        int node = (parent[a] == b) ? a : b;
        edgeCost[pos[node]] = c;
    }

    buildSegTree(1, 0, curPos - 1);

    string cmd;
    while (cin >> cmd && cmd != "DONE") {
        if (cmd == "QUERY") {
            int a, b;
            cin >> a >> b;
            --a, --b;
            cout << queryHLD(a, b) << "\n";
        } else if (cmd == "CHANGE") {
            int i, ti;
            cin >> i >> ti;
            --i;
            auto [a, b, c] = edges[i];
            int node = (parent[a] == b) ? a : b;
            updateSegTree(1, 0, curPos - 1, pos[node], ti);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) {
        solve();
        if (t) cout << "\n";
    }

    return 0;
}


