#include "json.h"

JSON::JSON()
{

}

QString JSON::xml2json(const XMLTree &tree, int spaces)
{
    QString builder;
    QTextStream ts(&builder);
    QString local_indent;
    local_indent.reserve(spaces + 2);
    for(int i = 0; i < spaces; i++)
        local_indent += " ";

    ts << "{" << (spaces >= 0 ? "\n" : "" ) << local_indent
       << tree.root()->tag() << ":" << (spaces >= 0 ? " " : "" );

    xml2json_helper(tree.root(), spaces, 1, 0, ts);

    ts << "}";

    return builder;
}

void JSON::xml2json_helper(const XMLNode *node,
                           int spaces,
                           int depth,
                           bool array_parent,
                           QTextStream &output)
{
    if(node == nullptr)
        return;

    QString space;
    QString indent;
    QString end_line;
    QString indent_less;
    QString local_indent;

    if(spaces >= 0) {
        local_indent.reserve(spaces + 2);
        for(int i = 0; i < spaces; i++)
            local_indent += " ";

        indent_less.reserve(spaces * depth + 2);
        indent.reserve(spaces * (depth + 1) + 2);
        for(int i = 0; i < depth; i++)
            indent_less += local_indent;

        indent = indent_less + local_indent;
        space = " ";
        end_line = "\n";
    }

    QString value = node->value();
    QRegExp comments("<!--[\\w\\W]+-->");
    value.remove(comments);
    if(spaces < 0)
        value = value.simplified();

    if(!node->attributes_size() && node->is_leaf()) {
        if(array_parent) output << local_indent;
        output << "\"" << (value == "" ? "null" : value) <<  "\"," << end_line;
        return;
    }

    output << "{" << end_line;

    for(const auto& item : node->attributes()) {
        output << indent  << "#" << item.key << ":" << space
                 << item.value << "," << end_line;
    }

    HashMap<QString, QList<XMLNode *>> children;

    for(const auto& child: qAsConst((const QList<XMLNode *>&)node->children())) {
        if(children.contains(child->tag()))
            children[child->tag()].append(child);
        else
            children.insert(child->tag(), QList({child}));
    }

    for(const auto& group : children) {
        if(group.value.size() == 1) {
            output << indent << group.key << ":" << space;
            xml2json_helper(group.value[0], spaces, depth + 1, 0, output);
        } else {
            output << indent << group.key << ":" << space << "[" << end_line;
            for(const auto& child : group.value) {
                output << indent ;
                xml2json_helper(child, spaces, depth + 1, 1, output);
            }
            output << indent << "]," << end_line;
        }
    }

    if(value != "")
        output << indent << "@text:" << space << "\"" << value << "\"," << end_line;

    output << indent_less << "}," << end_line;
}
