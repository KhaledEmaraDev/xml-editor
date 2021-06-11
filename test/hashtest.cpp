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
        mp.insert(i, QString("Test") + QString(i + '0'));
    }

    mp.insert({40, QString("Yup")});

//    qDebug() << mp;

    for(int i = 8; i < 16; i++)
    {
        mp.remove(i);
    }
//    qDebug() << mp;
    mp[0] = "Test10";
    mp[9] = "Test9";

    assert(mp[1] == "Test1");
    assert(mp[0] == "Test10");
    assert(mp[12] == "");
    assert(mp.get(2) == "Test2");
    assert(mp.find(20) == mp.end());
    assert(mp.find(2) != mp.end());

//    qDebug() << mp;

    HashMap<int, QString> copy(mp);
//    qDebug() << mp;
//    qDebug() << copy;

    copy = mp;
    copy = std::move(mp);
//    qDebug() << mp;
//    qDebug() << copy;
//    assert(mp == copy);

//    copy.clear();

//    qDebug() << copy;

    int i = 0;
    for(auto& item : copy) {
         qDebug() << i++ << item.key << "/" << item.value;
//         item.key = 9;
//         item.value = "NWay";
    }

    i = 0;
    for(auto it = copy.begin(); it != copy.end(); ++it) {
         qDebug() << i++ << it->key << "/" << it->value;
//         it->key = 9;
         it->value = "NWay";
    }
    qDebug() << copy;
}

void testAll() {
//    test_hash_codes();
    test_hash_map();
}
