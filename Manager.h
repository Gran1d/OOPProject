//
// Created by Артём Сиротин on 18.12.2023.
//

#ifndef OOPPROJECT_MANAGER_H
#define OOPPROJECT_MANAGER_H
class Manager : public BusinessEntity<Manager> {
public:
    Manager() : BusinessEntity(){
        currentcitymanager = currentcity;
    }
    Manager(const string& name, int currentcity) : BusinessEntity(name, currentcity) {
        currentcitymanager = currentcity;
    }
    Manager(const Manager &other) = default;
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
    //Метод идентификации
    virtual string identify() const override{
        return "Manager";
    };
private:
    mutable int currentcitymanager;
};
#endif //OOPPROJECT_MANAGER_H
