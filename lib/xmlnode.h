#ifndef XMLNODE_H
#define XMLNODE_H

#include <QList>
#include <QString>

#include "hashmap.h"
/**
 * @brief The XMLNode class
 *        Represents a node in XML Tree
 */
class XMLNode
{
public:
    /**
     * @brief XMLNode
     *        Default constructor
     */
    XMLNode();

    /**
      * Destructor
      */
    ~XMLNode();

    /**
     * @brief tag
     * @return tag name
     */
    QString tag() const;

    /**
     * @brief set_tag
     *        update the tag name of the node
     */
    void set_tag(const QString &tag);

    /**
     * @brief value
     * @return value
     */
    QString value() const;

    /**
     * @brief set_value
     *        update the value of the node
     */
    void set_value(const QString &value);

    /**
     * @brief add_child
     */
    void add_child(XMLNode* node);

    /**
     * @brief children_size
     * @return number of children
     */
    int children_size() const;

    /**
     * @brief attributes_size
     * @return number of attributes
     */
    int attributes_size() const;

    /**
     * @brief is_leaf
     * @return true if the node has no children
     */
    bool is_leaf() const;

    /**
     * @brief is_selfclosing
     * @return true if the node is selfclosing
     */
    bool is_selfclosing() const;

    /**
     * @brief add_attribute
     *        add attribute in form of key/value pair
     */
    void add_attribute(const QString &key, const QString &value);

    /**
     * @brief add_attribute
     *        add attribute in form of MPair
     */
    void add_attribute(const MPair<QString, QString> &attribute);

    /**
     * @brief children
     * @return QList of pointers to the children of this node
     */
    QList<XMLNode *> children() const;

    /**
     * @brief attributes
     * @return the attributes of this node
     */
    HashMap<QString, QString> attributes() const;

    /**
     * @brief parent
     * @return pointer to the parent of this node
     */
    XMLNode *parent() const;

    friend class XMLTree;

private:
    QString m_tag;
    QString m_value;
    XMLNode * m_parent;
    bool m_selfclosing;
    QList<XMLNode *> m_children;
    HashMap<QString, QString> m_attributes;
};

#endif // XMLNODE_H
