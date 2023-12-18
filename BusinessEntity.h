//
// Created by Артём Сиротин on 18.12.2023.
//

#ifndef OOPPROJECT_BUSINESSENTITY_H
#define OOPPROJECT_BUSINESSENTITY_H
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
    bool contract = false;
};
template <typename T>
class BusinessEntity {
public:
    // дефолтный
    BusinessEntity(){
        name = "";
        currentcity = 0;
    }
    // Задаем имя и в каком мы городе сейчас
    BusinessEntity(const string& _name, int _currentcity){
        name = _name;
        currentcity = _currentcity;
    }
    BusinessEntity(const BusinessEntity &other) = default;

    string getName() const {
        return name;
    }
    virtual int getCurrentcity() const {
        return currentcity;
    }

    virtual void performAction(Client& client, Graph& graph, Warehouse& warehouse) const = 0;
    virtual string identify() const{
        return "BusinessEntity";
    };
    virtual ~BusinessEntity() = default;

protected:
    string name;
    int currentcity;
};
#endif //OOPPROJECT_BUSINESSENTITY_H
