#ifndef HASHMAP_H
#define HASHMAP_H

#include <QDebug>
#include <QVector>

#include "lib/hashcode.h"

//template <typename key_t, typename value_t>
//using enable_condition = std::enable_if_t<std::is_integral<key_t>::value ||
//                                          std::is_same<key_t, QString>::value ||
//                                          std::is_same<key_t, std::string>::value ||
//                                          std::is_same<key_t, const char *>::value, int> = 0;
template <typename key_t, typename value_t>
struct Cell {
    key_t key;
    value_t value;
    Cell* next;
    Cell* previous;

    Cell() = default;

    Cell(key_t key, value_t value)
        : key(key), value(value), next(nullptr), previous(nullptr)
    {
        // do nothing
    }
};

template <typename key_t, typename value_t>
class HashMap {
public:
    HashMap()
        : buckets(16), num_cells(0)
    {
        // do nothing
    }

    HashMap& insert(const key_t& key, const value_t& vaule) {
        int index = hash_code(key) % buckets.size();
        Cell<key_t, value_t>* new_cell =
                new Cell<key_t, value_t>(key, vaule);

        if(buckets[index]) {
            Cell<key_t, value_t>* cp = buckets[index];
            while(cp) {
                if(cp->key == key) {
                    delete new_cell;
                    return *this;
                }
                cp = cp->next;
            }
            new_cell->next = buckets[index];
            buckets[index]->previous = new_cell;
            buckets[index] = new_cell;
        }
        else {
            buckets[index] = new_cell;
        }

        num_cells++;
        return *this;
    }

    value_t get(const key_t& key) {
        int index = hash_code(key) % buckets.size();
        Cell<key_t, value_t>* cp = buckets[index];
        while (cp) {
            if(cp->key == key) {
                return cp->value;
            }
            cp = cp->next;
        }
        return NULL;
    }

    HashMap& insert(const std::pair<key_t, value_t>& item) {
        return insert(item.first, item.second);
    }

    template <typename T, typename U>
    friend QDebug operator<<(QDebug dbg, const HashMap<T, U> &seg);
public:
    class iterator:
            public std::iterator<std::input_iterator_tag, key_t> {
    private:
        const HashMap* mp;
        int bucket;
        Cell<key_t, value_t>* cp;
    public:
        iterator() = default;
        iterator(const HashMap* mp, bool end)
        {
            this->mp = mp;
            if(end) {
                bucket = mp->buckets.size();
                cp = nullptr;
            } else {
                bucket = 0;
                cp = mp->buckets[0];
                while (!cp && ++bucket < mp->buckets.size()) {
                    cp = mp->buckets[bucket];
                }
            }
        }
        iterator& operator++() {
            cp = cp->next;
            while (!cp && ++bucket < mp->buckets.size()) {
                cp = mp->buckets[bucket];
            }
            return *this;
        }

        iterator operator++(int) {
            iterator copy(*this);
            operator++();
            return copy;
        }

        key_t& operator*() {
            return cp->key;
        }

        key_t* operator->() {
            return &(cp->key);
        }
        bool operator ==(const iterator& it) {
            return mp == it.mp && bucket == it.bucket && cp == it.cp;
        }

        bool operator !=(const iterator& it) {
            return mp != it.mp || bucket != it.bucket || cp != it.cp;
        }
    };

    iterator begin() const {
        return iterator(this, false);
    }

    iterator end() const {
        return iterator(this, true);
    }

private:

    QVector<Cell<key_t, value_t> *> buckets;
    int num_cells;
};

template <typename key_t, typename value_t>
QDebug operator<<(QDebug dbg, const HashMap<key_t, value_t> &mp) {
    for(int i = 0; i < mp.buckets.size(); i++) {
        dbg.nospace() << i << ":";
        auto cp = mp.buckets[i];
        while(cp != nullptr) {
            dbg.nospace() << " { "
                          << cp->key
                          << " : "
                          << cp->value
                          << " }";
            cp = cp->next;
        }
        dbg.nospace() << "\n";
    }
    dbg.nospace() << "\n";
    return dbg.nospace();
}


#endif // HASHMAP_H
