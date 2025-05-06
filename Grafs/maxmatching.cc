#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>
using namespace std;

// Implementació de l'algorisme de Gabow (Edmond's Blossom algorithm)

class GeneralGraphMatching {
private:
    int n;
    vector<vector<int>> adj;
    vector<int> match, parent, base;
    vector<bool> used, blossom;
    queue<int> q;

    int lca(int a, int b) {
        vector<bool> visited(n, false);
        while (true) {
            a = base[a];
            visited[a] = true;
            if (match[a] == -1) break;
            a = parent[match[a]];
        }
        while (true) {
            b = base[b];
            if (visited[b]) return b;
            if (match[b] == -1) break;
            b = parent[match[b]];
        }
        return -1;
    }

    void mark_blossom(int v, int w, int b) {
        while (base[v] != b) {
            blossom[base[v]] = blossom[base[match[v]]] = true;
            parent[v] = w;
            w = match[v];
            v = parent[w];
        }
    }

    void contract(int v, int u) {
        int b = lca(v, u);
        fill(blossom.begin(), blossom.end(), false);
        mark_blossom(v, u, b);
        mark_blossom(u, v, b);
        if (base[v] != b) parent[v] = u;
        if (base[u] != b) parent[u] = v;
        for (int i = 0; i < n; ++i) {
            if (blossom[base[i]]) {
                base[i] = b;
                if (!used[i]) {
                    used[i] = true;
                    q.push(i);
                }
            }
        }
    }

    bool bfs(int root) {
        fill(used.begin(), used.end(), false);
        fill(parent.begin(), parent.end(), -1);
        iota(base.begin(), base.end(), 0);
        while (!q.empty()) q.pop();

        used[root] = true;
        q.push(root);

        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (int u : adj[v]) {
                if (base[v] == base[u] || match[v] == u) continue;
                if (u == root || (match[u] != -1 && parent[match[u]] != -1)) {
                    contract(v, u);
                } else if (parent[u] == -1) {
                    parent[u] = v;
                    if (match[u] == -1) {
                        augment(u);
                        return true;
                    } else {
                        used[match[u]] = true;
                        q.push(match[u]);
                    }
                }
            }
        }
        return false;
    }

    void augment(int u) {
        while (u != -1) {
            int pv = parent[u], ppv = match[pv];
            match[u] = pv;
            match[pv] = u;
            u = ppv;
        }
    }

public:
    GeneralGraphMatching(int n) : n(n), adj(n), match(n, -1),
        parent(n), base(n), used(n), blossom(n) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int max_matching() {
        int matching = 0;
        for (int i = 0; i < n; ++i) {
            if (match[i] == -1 && bfs(i)) {
                ++matching;
            }
        }
        return matching;
    }

    const vector<int>& get_matching() const {
        return match;
    }
};

// Exemple d'us

int main() {
    int n = 6;
    GeneralGraphMatching matcher(n);

    matcher.add_edge(0, 1);
    matcher.add_edge(0, 2);
    matcher.add_edge(1, 2);
    matcher.add_edge(1, 3);
    matcher.add_edge(2, 4);
    matcher.add_edge(3, 5);
    matcher.add_edge(4, 5);

    int result = matcher.max_matching();
    cout << "Matching màxim: " << result << endl;

    auto match = matcher.get_matching();
    for (int i = 0; i < n; ++i) {
        if (match[i] != -1 && i < match[i])
            cout << "Emparellats: " << i << " - " << match[i] << endl;
    }

    return 0;
}

