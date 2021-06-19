#ifndef XMLNODE_H
#define XMLNODE_H

#include <QList>
#include <QString>

#include "hashmap.h"

class XMLNode
{
public:
    XMLNode();


    QString tag() const;
    void set_tag(const QString &tag);

    QString value() const;
    void set_value(const QString &value);

    void add_child(XMLNode* node);

    int children_size() const;
    int attributes_size() const;

    bool is_leaf() const;

    void add_attribute(const QString &key, const QString &value);
    void add_attribute(const MPair<QString, QString> &attribute);

    QList<XMLNode *> children() const;

    HashMap<QString, QString> attributes() const;

private:
    QString m_tag;
    QString m_value;
    QList<XMLNode *> m_children;
    HashMap<QString, QString> m_attributes;
};

#endif // XMLNODE_H
