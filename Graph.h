//
// Created by Артём Сиротин on 18.12.2023.
//

#ifndef OOPPROJECT_GRAPH_H
#define OOPPROJECT_GRAPH_H
struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair;

class Graph {
public:


    Graph(vector<Edge> const &edges, int n) {
        adjList.resize(n);

        for (auto &edge: edges) {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;

            adjList[src].push_back(make_pair(dest, weight));
            adjList[dest].push_back(make_pair(src, weight));
        }
    }

    vector<int> dijkstra(int start) {
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;
        vector<int> dist(adjList.size(), numeric_limits<int>::max());

        pq.push(make_pair(0, start));
        dist[start] = 0;

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            for (Pair &neighbor : adjList[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }

        return dist;
    }
    friend std::ostream& operator<<(std::ostream& os, const Graph& graph) {
        os << "Graph:\n";
        for (int i = 0; i < graph.adjList.size(); ++i) {
            os << "Vertex " << i << ": ";
            for (const auto& neighbor : graph.adjList[i]) {
                os << "(" << i  << ", " << neighbor.first << ", " << neighbor.second << ") ";
            }
            os << "\n";
        }
        return os;
    }
private:
    vector<vector<Pair>> adjList;
};

#endif //OOPPROJECT_GRAPH_H
