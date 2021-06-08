#include "lib/hashcode.h"

void test_hash_codes() {
    hashCode(4);
//    hashCode(9.9);
    qDebug() << hashCode("String");
    qDebug() << hashCode(QString("String"));
    qDebug() << hashCode(std::string("String"));
}

void testAll() {
    test_hash_codes();
}
