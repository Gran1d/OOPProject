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
    // Конструктор Graphа
    Graph(vector<Edge> const &edges, int n) {
        // изменить размер вектора, чтобы он содержал `n` элементов типа vector<Edge>
        adjList.resize(n);
        // добавляем ребра
        for (auto &edge: edges) {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;

            adjList[src].push_back(make_pair(dest, weight));
            adjList[dest].push_back(make_pair(src, weight));
        }
    }

    vector<int> dijkstra(int start) {
        // Приоритетная очередь для хранения пар (расстояние, вершина)
        // с минимальным расстоянием на вершине
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;

        // Вектор для хранения кратчайших расстояний от начальной вершины до каждой вершины графа
        vector<int> dist(adjList.size(), numeric_limits<int>::max());

        // Инициализация начальной вершины
        pq.push(make_pair(0, start));
        dist[start] = 0;

        while (!pq.empty()) {
            // Извлекаем вершину с минимальным расстоянием из очереди
            int u = pq.top().second;
            pq.pop();
            // Обходим соседей текущей вершины

            for (Pair &neighbor : adjList[u]) {
                int v = neighbor.first; // Соседняя вершина
                int weight = neighbor.second;   // Вес ребра между u и v

                // Проверяем, можно ли улучшить расстояние до вершины v через текущую вершину u
                if (dist[u] + weight < dist[v]) {
                    // Улучшаем расстояние до вершины v
                    dist[v] = dist[u] + weight;
                    // Добавляем вершину v с новым расстоянием в приоритетную очередь
                    pq.push(make_pair(dist[v], v));
                }
            }
        }

        return dist;
    }
    friend ostream& operator<<(ostream& os, const Graph& graph) {
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
    // вектор векторов Pair для представления списка смежности
    vector<vector<Pair>> adjList;
};

#endif //OOPPROJECT_GRAPH_H
