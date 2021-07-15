#ifndef JSON_H
#define JSON_H

#include <QString>
#include "lib/hashmap.h"
#include "lib/xmltree.h"


class JSON
{
public:
    JSON();

    /**
     * @brief xml2json
     * @param tree
     * @param spaces
     * @return
     */
    static QString xml2json(const XMLTree& tree, int spaces = -1);

private:
    /**
     * @brief xml2json_helper
     * @param node
     * @param spaces
     * @param depth
     * @param output
     */
    static void xml2json_helper(const XMLNode* node,
                                int spaces,
                                int depth,
                                bool array_parent,
                                QTextStream &output);
};

#endif // JSON_H
