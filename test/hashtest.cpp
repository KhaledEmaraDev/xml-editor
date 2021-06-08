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
    mp.insert(4, "Test4");
    mp.insert(4, "Test4");
    mp.insert(8, "Test8");

    qDebug() << mp;

    int i = 0;
    for(auto item : mp) {
         qDebug() << i++ << item;
    }
}

void testAll() {
//    test_hash_codes();
    test_hash_map();
}
