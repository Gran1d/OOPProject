//
// Created by Артём Сиротин on 17.12.2023.
//

#ifndef OOPPROJECT_HASH_TABLE_H
#define OOPPROJECT_HASH_TABLE_H
#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <string>
using  namespace std;
template <typename K, typename V>
class HashTable {
public:
    HashTable() : table(INITIAL_SIZE), size(0), tableSize(INITIAL_SIZE) {}
    HashTable(const HashTable& other) = default;
    ~HashTable() = default;
    //вставка
    void insert(const K& key, const V& value) {
        size_t index = hashFunction(key);

        // Проверка наличия ключа в списке по индексу
        for (auto& p : table[index]) {
            if (p.first == key) {
//                cout << "Key already exists. Updating value.\n";
                p.second = value;
                return;
            }
        }

        // Если ключ не найден, добавляем новую пару ключ-значение в список
        table[index].emplace_back(key, value);
        size++;

        // Проверка коэффициента загрузки
        if (loadFactor() > LOAD_FACTOR_THRESHOLD_HIGH) {
            rehash();
        }
    }


    //удаление по ключу
    void remove(const K& key) {
        size_t index = hashFunction(key);

        table[index].erase(remove_if(table[index].begin(), table[index].end(),
                                     [key](const auto& entry) { return entry.first == key; }),
                           table[index].end());
        size--;

        if (loadFactor() < LOAD_FACTOR_THRESHOLD_LOW) {
            shrink();
        }
    }
    //удалить все
    void removeAll(){
        vector<vector<pair<K, V>>> newTable;
        newTable.resize(INITIAL_SIZE, vector<pair<K, V>>());
        table = std::move(newTable);
        size = 0;
        tableSize = INITIAL_SIZE;
    }
    //текущий размер
    size_t size_() const {
        return size;
    }
    //Существование ключа
    bool containKey(const K key) const {
        size_t i = hashFunction(key);
        for (auto[k, v] : table[i]) {
            if (k == key) {
                return true;
            }
        }
        return false;
    }
    //возвращает все ключи
    vector<K> keys() const {
        vector<K> keys;
        for (auto& arr : table) {
            for (auto& p : arr) {
                keys.push_back(p.first);
            }
        }
        return keys;
    }

    //Возвращает значение по задаваемому ключу
    V operator[](const K key) const {
        size_t i = hashFunction(key);
        for (auto[k, v] : table[i]) {
            if (k == key) {
                return v;
            }
        }
    }


private:
    static const size_t INITIAL_SIZE = 10;  // Начальный размер таблицы
    static const double LOAD_FACTOR_THRESHOLD_HIGH;  // Порог для увеличения размера таблицы
    static const double LOAD_FACTOR_THRESHOLD_LOW;   // Порог для уменьшения размера таблицы

    vector<vector<pair<K, V>>> table;
    size_t size;  // Количество элементов в таблице
    size_t tableSize;  // Текущий размер таблицы

    //хэш-функция
    size_t hashFunction(const K& key) const {
        return hash<K>{}(key) % tableSize;
    }

    // Пересчитать коэффициент загрузки
    double loadFactor() const {
        return static_cast<double>(size) / tableSize;
    }

    // Перехеширование таблицы при изменении размера
    void rehash() {
        size_t newSize = tableSize * 2;  // Удвоение размера таблицы
        vector<vector<pair<K, V>>> newTable(newSize);

        // Перехеширование элементов
        for (auto& i : table) {
            for (auto& p : i) {
                size_t newIndex = hashFunction(p.first);
                newTable[newIndex].emplace_back(p);
            }
        }

        tableSize = newSize;
        table = std::move(newTable);
    }

    // Уменьшение размера таблицы
    void shrink() {
        size_t newSize = tableSize / 2;  // Уменьшение размера таблицы вдвое
        vector<vector<pair<K, V>>> newTable(newSize);

        // Перехеширование элементов
        for (auto& i : table) {
            for (auto& p : i) {
                size_t newIndex = hashFunction(p.first);
                newTable[newIndex].emplace_back(p);
            }
        }

        tableSize = newSize;
        table = std::move(newTable);
    }
};

template <typename K, typename V>
const double HashTable<K, V>::LOAD_FACTOR_THRESHOLD_HIGH = 0.75;

template <typename K, typename V>
const double HashTable<K, V>::LOAD_FACTOR_THRESHOLD_LOW = 0.25;
#endif //OOPPROJECT_HASH_TABLE_H
