#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <tuple>
using namespace std;


// Codi del Maxflow mincost

template<typename T>
class MinCostMaxFlow {
    struct Edge {
        int to, rev;
        T cap;
        T cost;
    };

    int n;
    vector<vector<Edge>> adj;
    vector<T> dist, potential;
    vector<int> parent_node, parent_edge;

    static constexpr T INF = numeric_limits<T>::max() / 2;

public:
    MinCostMaxFlow(int n) : n(n), adj(n), dist(n), potential(n), parent_node(n), parent_edge(n) {}

    void add_edge(int u, int v, T cap, T cost) {
        adj[u].push_back(Edge{v, (int)adj[v].size(), cap, cost});
        adj[v].push_back(Edge{u, (int)adj[u].size() - 1, 0, -cost});
    }

    pair<T, T> min_cost_max_flow(int s, int t) {
        T flow = 0, cost = 0;
        potential.assign(n, 0);

        while (true) {
            dist.assign(n, INF);
            dist[s] = 0;
            priority_queue<pair<T, int>, vector<pair<T, int>>, greater<>> pq;
            pq.push({0, s});

            while (!pq.empty()) {
                auto [d, u] = pq.top(); pq.pop();
                if (d != dist[u]) continue;

                for (int i = 0; i < adj[u].size(); ++i) {
                    Edge &e = adj[u][i];
                    if (e.cap > 0) {
                        T new_dist = dist[u] + e.cost + potential[u] - potential[e.to];
                        if (new_dist < dist[e.to]) {
                            dist[e.to] = new_dist;
                            parent_node[e.to] = u;
                            parent_edge[e.to] = i;
                            pq.push({new_dist, e.to});
                        }
                    }
                }
            }

            if (dist[t] == INF) break;

            for (int i = 0; i < n; ++i)
                if (dist[i] < INF)
                    potential[i] += dist[i];

            T f = INF;
            for (int u = t; u != s; u = parent_node[u]) {
                Edge &e = adj[parent_node[u]][parent_edge[u]];
                f = min(f, e.cap);
            }

            for (int u = t; u != s; u = parent_node[u]) {
                Edge &e = adj[parent_node[u]][parent_edge[u]];
                e.cap -= f;
                adj[u][e.rev].cap += f;
                cost += f * e.cost;
            }

            flow += f;
        }

        return {flow, cost};
    }
};

// Exemple d'us

int main() {
    int n = 4;
    MinCostMaxFlow<long long> mcmf(n);

    mcmf.add_edge(0, 1, 2, 1);
    mcmf.add_edge(0, 2, 1, 5);
    mcmf.add_edge(1, 2, 1, 0);
    mcmf.add_edge(1, 3, 1, 3);
    mcmf.add_edge(2, 3, 2, 1);

    auto [max_flow, min_cost] = mcmf.min_cost_max_flow(0, 3);
    cout << "Flux màxim: " << max_flow << endl;
    cout << "Cost mínim total: " << min_cost << endl;

    return 0;
}
