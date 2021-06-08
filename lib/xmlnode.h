#ifndef XMLNODE_H
#define XMLNODE_H

#include <QList>
#include <QString>

#include "hashmap.h"

class XMLNode
{
public:
    XMLNode();


private:
    QString tag;
    QString value;
    QList<XMLNode *> children;
    HashMap<QString, QString> attributes;
};

#endif // XMLNODE_H
