#include "lib/hashcode.h"
#include "lib/hashmap.h"

void test_hash_codes() {
    hash_code(4);
//    hashCode(9.9);
    qDebug() << hash_code("String");
    qDebug() << hash_code(QString("String"));
    qDebug() << hash_code(std::string("String"));
}

void test_hash_map() {
    HashMap<int, QString> mp;
    for(int i = 0; i < 16; i++)
    {
        mp.insert(i, "Test4");
        qDebug() << mp;
    }

    for(auto it = mp.find(1); it != mp.end(); ++it) {
        qDebug() << *it << " " << mp.get(*it);
    }


    int i = 0;
    for(auto item : mp) {
         qDebug() << i++ << item;
    }
}

void testAll() {
//    test_hash_codes();
    test_hash_map();
}
