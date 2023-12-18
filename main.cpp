#include <iostream>
#include <vector>
#include <queue>
#include "Hash_Table.h"
#include "warehouse.h"
#include "Graph.h"
#include "BusinessEntity.h"
#include "Manager.h"
#include "Carrier.h"

int main() {

    //Тесты класса Hash_Table
    HashTable<int, int> table1;

    assert(table1.size_() == 0);
    table1.insert(1, 11);
    assert(table1.size_() == 1);
    table1.insert(1, 4);
    assert(table1.size_() == 1);
    table1.remove(1);
    assert(table1.size_() == 0);
    table1.insert(1, 1);
    table1.removeAll();

    table1.insert(1, 1);
    assert(table1[1] == 1);
    table1.insert(1,5);
    assert(table1[1] == 5);
    cout << "Test HashTable" << endl;
    //Тесты warehouse
    Warehouse warehouseTest = Warehouse();
    warehouseTest.add("Товар1", 100);
    warehouseTest.add("Товар2", 100);
    warehouseTest.add("Товар3", 50);
    warehouseTest.add("Товар4", 250);
    assert(warehouseTest.get("Товар1") == 100);
    assert(warehouseTest.get("Товар2") == 100);
    warehouseTest.add("Товар1",200);
    assert(warehouseTest.get("Товар1") == 200);
    cout << "Test warehouse" << endl;
    //Тесты Graph
    vector<Edge> edges = {
            {0, 1, 1}, {1, 2, 8}, {2, 3, 4},
            {3, 0, 5}, {0, 4, 7}, {1, 4, 4},
            {2, 4, 2}, {3, 4, 3}
    };
    //Количество вершин
    int n = 5;
    Graph graph(edges, n);
    vector<int> shortestDistances = graph.dijkstra(0);
    // Выводим граф
    cout << graph;
    cout << shortestDistances[4] << endl;
    cout << shortestDistances[2] << endl;
    assert(shortestDistances[4] == 5);
    assert(shortestDistances[2] == 7);
    cout << "Test Graph" << endl;
    // Тесты Manager
    //создадим клиентов
    vector<Client> clients = {{"Клиент1", 1, {{"Товар1", 150},
                                                     150, 150}, 20, "buy"},
                              {"Клиент2", 2, {{"Товар2", 120},
                                                     120, 120}, 15, "sell"},
                              {"Клиент3", 3, {{"Товар3", 100},
                                                     100, 100}, 50, "sell"}};
    Manager manager("Менеджер1", 0);

    manager.performAction(clients[0],graph, warehouseTest);
    cout << manager.getCurrentcity() << endl;
    assert(manager.getCurrentcity() == 1);
    manager.performAction(clients[1],graph, warehouseTest);
    cout << manager.getCurrentcity() << endl;
    assert(manager.getCurrentcity() == 2);
    manager.performAction(clients[2], graph, warehouseTest);
    cout << manager.getCurrentcity() << endl;
    assert(manager.getCurrentcity() == 3);
    assert(manager.getName() == "Менеджер1");
    cout << "Test Manager" << endl;
    cout << manager << endl;
    //Дефолтный конструктор
    Manager man1;
    assert(man1.getName() == "");
    assert(man1.getCurrentcity() == 0);
    //Конструктор копирования
    Manager man2 = Manager(manager);
    assert(man2.getCurrentcity() == 3);
    assert(man2.getName() == "Менеджер1");
    //Идентификация
    assert(manager.identify() == "Manager");
    // Тесты Carrier
    Carrier carrier("Перевозчик1", 0);
    carrier.performAction(clients[0], graph, warehouseTest);
    cout << warehouseTest.get("Товар1") << endl;
    assert(warehouseTest.get("Товар1") == 70);
    assert(carrier.getCurrentcity() == 1);
    carrier.performAction(clients[1], graph, warehouseTest);
    cout << warehouseTest.get("Товар2") << endl;
    assert(warehouseTest.get("Товар2") == 130);
    assert(carrier.getName() == "Перевозчик1");
    assert(carrier.getCurrentcity() == 2);
    //Дефолтный конструктор
    Carrier car1;
    assert(car1.getName() == "");
    assert(car1.getCurrentcity() == 0);
    //Конструктор копирования
    Carrier car2 = Carrier(carrier);
    assert(car2.getCurrentcity() == 2);
    assert(car2.getName() == "Перевозчик1");
    //Идентификация
    assert(carrier.identify() == "Carrier");
    carrier.performAction(clients[2], graph, warehouseTest);
    cout << warehouseTest.get("Товар2") << endl;
    cout << "Pass all test" << endl;

    return 0;
}
