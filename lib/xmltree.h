#ifndef XMLTREE_H
#define XMLTREE_H

#include "lib/xmlnode.h"

class XMLTree
{
public:
    /**
     * @brief XMLTree
     *        Default constructor
     */
    XMLTree();

    /**
      * Destructor
      */
    ~XMLTree();

    /**
     * @brief dump
     * @return the XML Tree with the proper indentation
     *         as specfied with the number of spaces
     *         negative value means minifinying the XML
     *         Minifying the XML removes all the unneeded
     *         spaces and newlines in the text including the
     *         values of the XML nodes
     * @complexity O(sizeof(tree))
     */
    QString dump(int spaces = -1) const;

    /**
     * @brief load
     *        load the XML Tree from input stream
     *        The XML must be syntactically correct
     * @complexity O(length of(input))
     */
    void load(QTextStream& input);

    /**
     * @brief syntax_check
     *        checks if the XML is syntactically correct
     *        it loops through the list of tokens checks
     *        if its order subject to the rules of the XML
     *        it throws MPair{error_index, error_description}
     * @param capture_all
     *        if specfied it will throw QVector<MPair>
     *        of all encountered errors in the document
     *        the following errors may be more or different
     *        than expected
     * @return true if the XML is syntactically correct
     * @complexity O(length of(input))
     */
    static int syntax_check(QTextStream& input, bool capture_all = true);

    /**
     * @brief is_token
     *        static function checks if the given token
     *        is a special XML token
     * @complexity O(size of(token))
     */
    static bool is_token(const QString& token);

    /**
     * @brief size
     * @return number of XML Nodes
     * @complexity O(1)
     */
    int size() const { return m_size;}

    XMLNode *root() const;

private:
    /**
     * @brief load_helper
     *        load the xml tree to the given XML parent
     *        it operates on single parent node
     *        it can detect self-closing nodes
     *        it ignores the meta data of xml as they are
     *        not part of the xml document
     *        it operates on syntactically correct XML
     *        with light error checking for the attribute list
     *
     *        it simply loop through the list of tokens
     *        if it encountered an opening tag
     *        it will add this node as a child of the current node
     *        else it will add it as a sibling for this node
     *        if it encountered a second closing tag it will return to
     *        the parent of the current node
     *
     * @param list QStringList of tokens generated from tokenize function
     * @param pos  starting postion in the list
     * @param node the parent node
     */
    void load_helper(const QStringList &list, int pos, XMLNode *node);

    /**
     * @brief dump_helper
     *        recursive funtion to dump XML tree into output
     * @param node   the current node
     * @param spaces indentation size
     * @param depth  current node depth
     * @param output outputstream
     */
    void dump_helper(XMLNode *node, int spaces, int depth, QTextStream &output) const;

    /**
     * @brief tokenize
     * @return QStringList of XML tokens as well as white spaces
     */
    static QStringList tokenize(QTextStream& input);

    XMLNode * m_root;
    int m_size;
};

#endif // XMLTREE_H
