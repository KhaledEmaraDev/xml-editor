#ifndef XMLTREE_H
#define XMLTREE_H

#include "lib/xmlnode.h"

class XMLTree
{
public:
    XMLTree();
    ~XMLTree();

    QString dump(int spaces = -1) const;
    void load(QTextStream& input);

    static int syntax_check(QTextStream& input, bool capture_all = true);
    static bool is_token(const QString& token);

    int size() const { return m_size;}
private:
    XMLNode * m_root;
    int m_size;
    void load_helper(const QStringList &list, int pos, XMLNode *node);
    void dump_helper(XMLNode *node, int spaces, int depth, QTextStream &output) const;
    static QStringList tokenize(QTextStream& input);
};

#endif // XMLTREE_H
