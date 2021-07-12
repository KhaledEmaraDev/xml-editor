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

    static int syntax_check(QTextStream& input, int capture_all = 1);

    int size() const { return m_size;}
private:
    XMLNode * m_root;
    int m_size;
    void load_helper(const QStringList &list, int pos, XMLNode *node);
    void dump_helper(XMLNode *node, int spaces, int depth, QTextStream &output) const;

    // regex to tokize the XML text
    inline static const QString word { "[\\w\\.\\$\\%\\^\\&\\#\\@\\*\\(\\-\\+\\-\\):']+" };
    inline static const QString literal_string { "\"[\\w\\s\\.\\$\\%\\^\\&\\#\\@\\*\\(\\-\\+\\-\\):/'`,;]+\"" };
    inline static const QString white_spaces { "[\\s]+" };
    inline static const QString xml_tokens { "<|>|</|=|/>|-->|<!--|<\\?|\\?>" };

    inline static const QRegExp xml_rx { "(" + xml_tokens + "|"
                                             + white_spaces + "|"
                                             + literal_string + "|"
                                             + word + ")" };
};

#endif // XMLTREE_H
