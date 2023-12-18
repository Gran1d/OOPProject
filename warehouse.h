//
// Created by Артём Сиротин on 17.12.2023.
//

#ifndef OOPPROJECT_WAREHOUSE_H
#define OOPPROJECT_WAREHOUSE_H
#include "Hash_Table.h"

class Warehouse {
public:
    Warehouse() = default;
    Warehouse(const Warehouse& other) = default;
    //Вставить
    void add(const string& name, size_t count){
        warehouse_.insert(name, count);
    }
    //удалить по ключу
    void remove(const string& name){
        return warehouse_.remove(name);
    }
    //удалить все
    void removeAll(){
        return warehouse_.removeAll();
    }
    //Геттер
    size_t get(string name) const{
        return warehouse_[name];
    }
    //Задаем число и выводим количество повторений
    int numberofrepetitions(int number){
        vector<string> keys = warehouse_.keys();
        int counter = 0;
        for(string& key : keys){
            if (warehouse_[key] >= number){
                counter+=1;
            }
        }
        return counter;
    }

private:
    HashTable<string, size_t> warehouse_;
};

#endif //OOPPROJECT_WAREHOUSE_H
