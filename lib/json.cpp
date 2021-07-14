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

    ts << "{\n" << local_indent << tree.root()->tag() << ": ";
    xml2json_helper(tree.root(), spaces, 1, ts);
    ts << "}";
//    while(!ts.atEnd())
//        qDebug().noquote().nospace() << ts.readLine();
    return builder;
}

void JSON::xml2json_helper(const XMLNode *node, int spaces, int depth, QTextStream &output)
{
    if(node == nullptr)
        return;

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
        end_line = "\n";
    }

    if(!node->attributes_size() && node->is_leaf()) {
       output << "\"" << (node->value() == "" ? "null" : node->value()) <<  "\"\n";
        return;
    }

    output << "{\n";

    for(const auto& item : node->attributes()) {
        output << indent  << "#" << item.key << ": "
                 << item.value << ",\n";
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
            output << indent << group.key << ": ";
            xml2json_helper(group.value[0], spaces, depth + 1, output);
        } else {
            output << indent << group.key << ": [ \n";
            for(const auto& child : group.value) {
                output << indent ;
                xml2json_helper(child, spaces, depth + 1, output);
            }
            output << indent << "],\n";
        }
    }

    if(node->value() != "")
        output << indent << "@text: \"" << node->value() << "\"\n";

    output << indent_less << "},\n";
}