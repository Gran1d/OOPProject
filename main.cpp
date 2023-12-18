#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include "Hash_Table.h"
#include "warehouse.h"
//#include "Graph.h"
//#include "BusinessEntity.h"
//#include "Manager.h"

using namespace std;

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair;

class Graph {
public:
    vector<vector<Pair>> adjList;

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
};

struct Product {
    string name;
    int volume;
};

struct Order {
    Product product;
    int remainingVolume;
    int initialVolume;
};
struct Client {
    string name;
    int city;
    Order order;
    //Можно поменять на рандом
    int soldproduct;
    string buyorsell;
};
template <typename T>
class BusinessEntity {
public:
    // дефолтный пока просто первый город и пустое имя
    BusinessEntity(){
        name = "";
        currentcity = 0;
    }
    // Задаем имя и в каком мы городе сейчас
    BusinessEntity(const string& _name, int _currentcity){
        name = _name;
        currentcity = _currentcity;
    }

    string getName() const {
        return name;
    }
    virtual int getCurrentcity() const {
        return currentcity;
    }

    virtual void performAction(Client& client, Graph& graph, Warehouse& warehouse) const = 0;

    virtual ~BusinessEntity() = default;

protected:
    string name;
    int currentcity;
};

class Manager : public BusinessEntity<Manager> {
public:
    Manager(const string& name, int currentcity) : BusinessEntity(name, currentcity) {
        currentcitymanager = currentcity;
    }

    void performAction(Client& client, Graph& graph, Warehouse& warehouse) const override {
        cout << "Менеджер " << getName() << " посетил клиента " << client.name << endl;
        // Выберем стартовую и конечную точку
        int start = currentcitymanager;
        int end = client.city;
        //Алг декстра, находит кротчайший путь из нужного в конечный
        vector<int> shortestDistances = graph.dijkstra(start);
        // Выведем результат для конечной точки
        cout << "Кратчайший путь от вершины " << start << " до вершины " << end << ": " << shortestDistances[end] << endl;
        client.order.initialVolume = client.order.remainingVolume - client.soldproduct * shortestDistances[end];

        double contractVolume = client.order.initialVolume;
        if (client.order.initialVolume < 0){
            cout << "Договор не заключен, товар закончился" << endl;
        }else{
            cout << "Заключен договор на объем: " << contractVolume << endl;
        }
        currentcitymanager = end;
    }

    //перегруженный геттер
    int getCurrentcity() const override {
        return currentcitymanager;
    }
    // Перегруженный оператор << чтобы выводить имя менеджера
    friend ostream& operator<<(ostream& os, const Manager& manager) {
        os << "Менеджер " << manager.getName();
        return os;
    }
private:
    mutable int currentcitymanager;
};

class Carrier : public BusinessEntity<Carrier> {
public:
    Carrier(const string& name, int currentcity) : BusinessEntity(name, currentcity){
        currentcitycarrier = currentcity;
        warehousecity = currentcity;
    }
    void replenishmentoftheWarehouse(Client& client,Warehouse& warehouse)const{
        warehouse.add(client.order.product.name, client.order.initialVolume + warehouse.get(client.order.product.name));
    }
    void rep(Client& client,Warehouse& warehouse)const{
        warehouse.add(client.order.product.name, warehouse.get(client.order.product.name) - client.order.initialVolume);
    }
    void performAction(Client& client, Graph& graph, Warehouse& warehouse) const override {
        cout << "Перевозчик " << getName() << " доставил товар для клиента " << client.name << endl;
        if(currentcitycarrier != warehousecity){
            warehousecity = currentcitycarrier;
        }
        if(client.buyorsell == "sell"){
            // Выберем стартовую и конечную точку
            int start = currentcitycarrier;
            int end = client.city;
            //Алг декстра, находит кротчайший путь из нужного в конечный
            vector<int> shortestDistances = graph.dijkstra(start);
            // Выведем результат для конечной точки
            cout << "Кратчайший путь от вершины " << start << " до вершины " << end << ": " << shortestDistances[end] << endl;
            currentcitycarrier = end;
            replenishmentoftheWarehouse(client, warehouse);
        } else{
            int start = currentcitycarrier;
            int end = client.city;
            //Алг декстра, находит кротчайший путь из нужного в конечный
            vector<int> shortestDistances = graph.dijkstra(start);
            // Выведем результат для конечной точки
            cout << "Кратчайший путь от вершины " << start << " до вершины " << end << ": " << shortestDistances[end] << endl;
            if(warehouse.get(client.order.product.name) < client.order.initialVolume){
                cout << "Товар отсутствует на складе" << endl;
                warehouse.add(client.order.product.name,warehouse.get(client.order.product.name));
            } else{
                rep(client,warehouse);
            }
            currentcitycarrier = end;
        }



    }

    // Перегруженный оператор << чтобы выводить имя перевозчика
    friend ostream& operator<<(ostream& os, const Carrier& carrier) {
        os << "Перевозчик " << carrier.getName();
        return os;
    }
private:
    mutable int warehousecity;
    mutable int currentcitycarrier;
};

int main() {
    vector<Edge> edges = {
            {0, 1, 1}, {1, 2, 8}, {2, 3, 4},
            {3, 0, 5}, {0, 4, 7}, {1, 4, 4},
            {2, 4, 2}, {3, 4, 3}
    };

    int n = 5;
    Graph graph(edges, n);

    vector<Client> clients = {{"Клиент1", 1, {{"Товар1", 150},
                                              150, 150}, 20, "buy"},
                                   {"Клиент2", 2, {{"Товар2", 120},
                                                   120, 120}, 15, "sell"},
                                   {"Клиент3", 3, {{"Товар3", 100},
                                                   100, 100}, 50, "sell"}};

    //склад
    Warehouse warehouse = Warehouse();
    warehouse.add("Товар1", 100);
    warehouse.add("Товар2", 100);
    warehouse.add("Товар3", 50);
    warehouse.add("Товар4", 250);

    Manager manager("Менеджер1", 0);
//    Carrier carrier = {"Перевозчик1"};

    manager.performAction(clients[0],graph, warehouse);
    cout << manager.getCurrentcity() << endl;
    manager.performAction(clients[1],graph, warehouse);
    cout << manager.getCurrentcity() << endl;
    manager.performAction(clients[2], graph, warehouse);
    cout << manager.getCurrentcity() << endl;


    Carrier carrier("Перевозчик1", 0);
    carrier.performAction(clients[0], graph, warehouse);
    cout << warehouse.get("Товар1") << endl;
    carrier.performAction(clients[1], graph, warehouse);
    cout << warehouse.get("Товар2") << endl;

    //Не берем 3-го потому что заказ не был заключен с менеджером

    cout << manager;

//    for (int day = 1; day <= 3; ++day) {
//        std::cout << "День " << day << ":" << std::endl;
//
//        for (auto& client : clients) {
//            if (client.order.remainingVolume > 0) {
//                manager.performAction(client);
//                carrier.performAction(client);
//                std::cout << "Оставшийся объем у клиента " << client.name << ": " << client.order.remainingVolume << std::endl;
//            }
//        }

//        for (auto& client : clients) {
//            double reduction = std::rand() % 21;
//            client.order.remainingVolume -= reduction;
//            std::cout << "Уменьшение объема заказа у клиента " << client.name << " на " << reduction << std::endl;
//        }

//        std::cout << std::endl;
//    }

    return 0;
}