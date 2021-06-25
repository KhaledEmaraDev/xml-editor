#ifndef XMLTREE_H
#define XMLTREE_H

#include "lib/xmlnode.h"

class XMLTree
{
public:
    XMLTree();
    ~XMLTree();

    void dump(int spaces = -1) const;
    void load(QTextStream& input);

    int size() const { return m_size;}
private:
    XMLNode * m_root;
    int m_size;
    void load_helper(const QStringList &list, int pos, XMLNode *node);
    void dump_helper(XMLNode *node, int spaces, int depth, QTextStream &output) const;
};

#endif // XMLTREE_H
