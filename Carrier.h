//
// Created by Артём Сиротин on 18.12.2023.
//

#ifndef OOPPROJECT_CARRIER_H
#define OOPPROJECT_CARRIER_H
class Carrier : public BusinessEntity<Carrier> {
public:
    Carrier() : BusinessEntity(){
        currentcitycarrier = currentcity;
        warehousecity = currentcity;
    }
    Carrier(const string& name, int currentcity) : BusinessEntity(name, currentcity){
        currentcitycarrier = currentcity;
        warehousecity = currentcity;
    }
    Carrier(const Carrier &other) = default;
    void performAction(Client& client, Graph& graph, Warehouse& warehouse) const override {
        if(client.contract){
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
        }else{
            cout << "Договор не был заключен, перевоздич не поехал" << endl;
        }

    }
    int getCurrentcity() const override {
        return currentcitycarrier;
    }

    // Перегруженный оператор << чтобы выводить имя перевозчика
    friend ostream& operator<<(ostream& os, const Carrier& carrier) {
        os << "Перевозчик " << carrier.getName();
        return os;
    }
    //Метод идентификации
    virtual string identify() const override{
        return "Carrier";
    };
private:
    mutable int warehousecity;
    mutable int currentcitycarrier;

    void replenishmentoftheWarehouse(Client& client,Warehouse& warehouse)const{
        warehouse.add(client.order.product.name, client.order.initialVolume + warehouse.get(client.order.product.name));
    }
    void rep(Client& client,Warehouse& warehouse)const{
        warehouse.add(client.order.product.name, warehouse.get(client.order.product.name) - client.order.initialVolume);
    }
};
#endif //OOPPROJECT_CARRIER_H
