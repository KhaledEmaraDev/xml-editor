#include "xmlnode.h"

XMLNode::XMLNode()
    : m_tag(""),
      m_value(""),
      m_parent(nullptr),
      m_selfclosing(false),
      m_children(),
      m_attributes()
{

}

XMLNode::~XMLNode()
{
    for(auto& child : m_children) {
        delete child;
        child = nullptr;
    }
}

QString XMLNode::tag() const
{
    return m_tag;
}

void XMLNode::set_tag(const QString &tag)
{
    m_tag = tag;
}

QString XMLNode::value() const
{
    return m_value;
}

void XMLNode::set_value(const QString &value)
{
    if(m_selfclosing)
        throw QString("Can't set value in self closing node");
    m_value = value;
}

void XMLNode::add_child(XMLNode *node)
{
    if(m_selfclosing)
        throw QString("Can't add child in self closing node");
    m_children.append(node);
}

int XMLNode::children_size() const
{
    return m_children.size();
}

int XMLNode::attributes_size() const
{
    return m_attributes.size();
}

bool XMLNode::is_leaf() const
{
    return children_size() == 0;
}

bool XMLNode::is_selfclosing() const
{
    return m_selfclosing;
}

void XMLNode::add_attribute(const QString &key, const QString &value)
{
    m_attributes.insert(key, value);
}

void XMLNode::add_attribute(const MPair<QString, QString> &attribute)
{
    m_attributes.insert(attribute);
}

QList<XMLNode *> XMLNode::children() const
{
    return m_children;
}

HashMap<QString, QString> XMLNode::attributes() const
{
    return m_attributes;
}

XMLNode *XMLNode::parent() const
{
    return m_parent;
}

