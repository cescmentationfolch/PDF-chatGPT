#include <iostream>
#include <vector>
#include <queue>
#include <limits>
using namespace std;

// Implementació de l'algorisme maxflow de Dinic.

template<typename T>
class Dinic {
    struct Edge {
        int to, rev;
        T cap;
    };

    int n;
    vector<vector<Edge>> adj;
    vector<int> level, ptr;
    static constexpr T INF = numeric_limits<T>::max() / 2;

public:
    Dinic(int n) : n(n), adj(n), level(n), ptr(n) {}

    void add_edge(int u, int v, T cap) {
        adj[u].push_back({v, (int)adj[v].size(), cap});
        adj[v].push_back({u, (int)adj[u].size() - 1, 0}); // aresta inversa amb cap 0
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        q.push(s);
        level[s] = 0;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (const Edge &e : adj[u]) {
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    }

    T dfs(int u, int t, T pushed) {
        if (u == t || pushed == 0) return pushed;
        for (int &i = ptr[u]; i < adj[u].size(); ++i) {
            Edge &e = adj[u][i];
            if (level[e.to] == level[u] + 1 && e.cap > 0) {
                T tr = dfs(e.to, t, min(pushed, e.cap));
                if (tr > 0) {
                    e.cap -= tr;
                    adj[e.to][e.rev].cap += tr;
                    return tr;
                }
            }
        }
        return 0;
    }

    T max_flow(int s, int t) {
        T flow = 0;
        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (T pushed = dfs(s, t, INF)) {
                flow += pushed;
            }
        }
        return flow;
    }
};

// Exemple d'us

int main() {
    int n = 4;
    Dinic<long long> dinic(n);

    dinic.add_edge(0, 1, 3);
    dinic.add_edge(0, 2, 2);
    dinic.add_edge(1, 2, 1);
    dinic.add_edge(1, 3, 2);
    dinic.add_edge(2, 3, 4);

    long long flow = dinic.max_flow(0, 3);
    cout << "Flux màxim: " << flow << endl;

    return 0;
}
