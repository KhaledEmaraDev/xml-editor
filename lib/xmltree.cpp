#include "xmltree.h"
#include <QFile>
#include <QStringBuilder>
#include <QTextStream>
#include <iostream>
#include <QStack>
#include <QFileInfo>

XMLTree::XMLTree()
    : m_root(nullptr), m_size(0)
{

}

XMLTree::~XMLTree()
{
    delete m_root;
    m_root = nullptr;
}

QString XMLTree::dump(int spaces) const
{
    QString builder;
    QTextStream ts(&builder);

    dump_helper(m_root, spaces, 0, ts);
//    while(!ts.atEnd())
//        qDebug().noquote().nospace() << ts.readLine();
    return builder;

}

void  XMLTree::dump_helper(XMLNode * node, int spaces, int depth, QTextStream& output) const
{
    if(node == nullptr)
        return;

    QString indent;
    QString end_line;
    QString local_indent;

    if(spaces >= 0) {
        local_indent.reserve(spaces + 2);
        for(int i = 0; i < spaces; i++)
            local_indent += " ";

        indent.reserve(spaces * depth + 2);
        for(int i = 0; i < depth; i++)
            indent += local_indent;

        end_line = "\n";
    }

    output << indent << "<" << node->m_tag;

    for(const auto& attribute : node->m_attributes)
        output << " " << attribute.key << "=" << attribute.value << "";

    if(node->m_selfclosing) {
        output << "/>" << end_line;
        return;
    }

    output << ">" << end_line;
    \
    if(node->m_value != "") {
        if(end_line != "") {
            QStringList lines = node->m_value.split('\n');
            for(int i = 0; i < lines.size(); ++i)
                output << indent  << local_indent <<  lines[i].trimmed() << "\n";
        } else {
            output << node->m_value.simplified();
        }
    }

    for(XMLNode * child : qAsConst(node->m_children))
        dump_helper(child, spaces, depth + 1, output);

    output << indent << "</" << node->m_tag << ">" << end_line ;
}

bool XMLTree::is_token(const QString& token) {
    const QRegExp xml_tokens { "<|>|</|=|/>|-->|<!--|<\\?|\\?>" };
    return xml_tokens.exactMatch(token);
}

QStringList XMLTree::tokenize(QTextStream &input)
{
    // regex to tokize the XML text
    const QString word { "[\\w\\.\\$\\%\\^\\&\\#\\@\\*\\(\\-\\+\\-\\):']+" };
    const QString literal_string { "\"[\\w\\s\\.\\$\\%\\^\\&\\#\\@\\*\\(\\-\\+\\-\\):/'`,;]+\"" };
    const QString white_spaces { "[\\s]+" };
    const QString xml_tokens { "<|>|</|=|/>|-->|<!--|<\\?|\\?>" };

    const QRegExp xml_rx { "(" + xml_tokens + "|"
                               + white_spaces + "|"
                               + literal_string + "|"
                               + word + ")" };
    int pos = 0;
    QStringList list;
    QString str = input.readAll();

    // extract the capctured groups
    while ((pos = xml_rx.indexIn(str, pos)) != -1) {
        list << xml_rx.cap(1);
        pos += xml_rx.matchedLength();
    }

    return list;
}

int XMLTree::syntax_check(QTextStream &input, bool capture_all)
{
    //    QFile file("../xml-editor/data/data-so-sample.xml");
    //    file.open(QFile::ReadOnly);
    //    QTextStream ts(&file);



    // extract the captured groups
    QStringList list = tokenize(input);

    //    qDebug() << list.size();
    //    qDebug() << list;

    // regex to match white spaces
    QRegExp white_spaces("[\\s]+");

    QStack<QString> stack;
    QVector<QPair<int, QString>> errors;

    int index = 0;
    int pos = 0;

    auto throw_error = [&index, &errors, capture_all](const QString& error) {
        if(capture_all)
            errors.push_back({index, error});
        else
            throw qMakePair(index, error);
    };

    auto ignore_white_spaces = [&list, &pos, &white_spaces, &index]() {
        while(++pos < list.size() &&
              white_spaces.exactMatch(list[pos]))
            index += list[pos].length();
    };

    while(pos < list.size()) {
        // search for the start of the node
        while(pos < list.size() &&
              list[pos] != "<" &&
              list[pos] != "</" /*&&
                                      list[pos] != ">" &&
                                      list[pos] != "/>"*/)
        {
            index += list[pos].length();
            pos++;
        }

        if(pos == list.size())
            break;

        if(list[pos] == ">" || list[pos] == "/>") {
            throw_error("Didn'r expected " + list[pos]);
            index += list[pos].length();
            pos++;
            continue;

        }

        // closing tag
        if(list[pos] == "</") {
            index += list[pos].length();
            ignore_white_spaces();

            if(pos == list.size()) {
                throw_error("Expected tag name");
                break;
            }

            if(is_token(list[pos])) {
                throw_error("Expected tag name");
                pos++;
                continue;
            }

            if(!(stack.size() && stack.top() ==  list[pos]))
                throw_error( stack.size() ? "Mismatched tages: Expected " + stack.top()
                                          : "Closing tag without matched opening tag");
            if(stack.size())
                stack.pop();
            index += list[pos].length();
            continue;
        }

        // opening tag
        ignore_white_spaces();

        if(pos == list.size()) {
            throw_error("Expected opening tag");
            break;
        }

        if(is_token(list[pos])) {
            throw_error("Expected tag name");
            pos++;
            continue;
        }

        stack.push(list[pos]);

        ignore_white_spaces();

        if(pos == list.size()) {
            throw_error("Expected > or />");
            break;
        }

        // attributes
        HashMap<QString, int> attributes;
        while(list[pos] != ">" && list[pos] != "/>") {

            if(is_token(list[pos])) {
                throw_error("Didn't expect \"" + list[pos] +"\"");
                pos++;
                continue;
            }

            if(attributes.contains(list[pos]))
                throw_error("Repeated attributes: \"" + list[pos] + "\"");
            else
                attributes[list[pos]];

            index += list[pos].length();
            ignore_white_spaces();

            if(pos == list.size()) {
                throw_error("Expected > or />");
                break;
            }
            if(list[pos] != "=")
                throw_error("Expected =");
            index += list[pos].length();

            if(list[pos] == ">" || list[pos] == "/>")
                break;

            ignore_white_spaces();

            if(pos == list.size() || list[pos] == ">" || list[pos] == "/>") {
                throw_error("Expected attribute value");
                break;
            }

            ignore_white_spaces();

            if(pos == list.size()) {
                throw_error("Expected > or />");
                break;
            }
        }

        // selfclosing tag
        if(list[pos] == "/>")
        {
            if(stack.size())
                stack.pop();
            else
                throw_error("Didn't expect />");
        }
        index += list[pos].length();
        pos++;


    }

    if(stack.size()) {
        QString tags;
        tags.reserve(stack.size() * 10);

        while(stack.size()) {
            tags += " <" + stack.top() + ">";
            stack.pop();
        }
        throw_error("Incomplete tags:" + tags);
    }

    qDebug() << errors;
    if(capture_all && errors.size())
        throw  errors;

    return errors.empty();


}

void XMLTree::load(QTextStream &input)
{
//    QFile file("../xml-editor/data/data-so-sample.xml");
//    file.open(QFile::ReadOnly);
//    QTextStream ts(&file);

    QStringList list = tokenize(input);
    qDebug() << list;

    // start loading the tree
    if(m_root) {
        delete m_root;
        m_size = 0;
    }
    m_root = new XMLNode;
    m_root->m_parent = nullptr;
    load_helper(list, 0, m_root);
    //    dump(2);
    //    dump(0);
    //    dump();
}

void XMLTree::load_helper(const QStringList& list,
                          int pos,
                          XMLNode* node)
{
    // regex to match white spaces
    QRegExp white_spaces("[\\s]+");

    // QStrings to hold the tag and value
    QString tag, value;
    // QStrings to hold attributes
    QString att_name, att_value;

    //    qDebug() << node << pos << m_size << node->m_parent ;
    // node must be initalized before calling the function
    if(node == nullptr)
        return;
    while(pos <= list.size()) {
        //        qDebug() << node << pos << m_size << node->m_parent ;

        // store node_parent
        XMLNode * node_parent = node->m_parent;

        // search for the start of the node
        while(pos < list.size() &&
              list[pos] != "<" &&
              list[pos] != "</")
            pos++;

        // found "</"
        // return to the parent of the current parent
        if(pos < list.size() && list[pos] == "</" &&
                node_parent && node_parent->m_parent)
        {
            XMLNode * sibling = new XMLNode;
            if(!sibling)
                throw "No memory left";
            sibling->m_parent = node_parent->m_parent;
            node_parent->m_parent->add_child(sibling);
            pos++;
            node = sibling;

            // delete the last added node
            delete node_parent->m_children.back();
            node_parent->m_children.pop_back();

            // continue exploraing other nodes
            continue;
        }

        // if we found we are finished
        // delete the last added node if it was a child
        // the last added node has no use now
        if(pos == list.size() ){
            if(node_parent) {
                delete node_parent->m_children.back();
                node_parent->m_children.pop_back();
            }
            break;
        }

        // not a new node
        // continue exploaring other nodes
        if(list[pos] == "</") {
            pos++;
            continue;
        }

        // extract node tag
        // ignore white spaces
        while(white_spaces.exactMatch(list[++pos]));
        tag = list[pos++];

        //    qDebug() << tag;
        node->m_tag = tag;

        // ignore white spaces
        while(white_spaces.exactMatch(list[pos])) ++pos;

        // extract attributes
        while(list[pos] != ">" && list[pos] != "/>") {

            // extract attribute name
            // ignore white spaces
            while(white_spaces.exactMatch(list[pos])) ++pos;
            att_name = list[pos];

            // ignore white spaces
            while(white_spaces.exactMatch(list[++pos]));


            // attribute value should proceed an "="
            // else throw an error
            if(list[pos] == "=") {
                while(white_spaces.exactMatch(list[++pos]));
                att_value = list[pos++];
            } else {
                qDebug() << pos << list[pos];
                throw QString("Expected = near index");
            }

            // add the current attribute and
            // search for anthor
            //        qDebug() << att_name << att_value;
            node->add_attribute(att_name, att_value);

            // ignore white spaces
            while(white_spaces.exactMatch(list[pos])) ++pos;
        }



        if(list[pos] == ">") {
            // ignore the closing tag
            ++pos;
            // normal node
            node->m_selfclosing = false;
            // extract node value
            // including white spaces
            value = "";
            while(list[pos] != "<" && list[pos] != "</")
                value += list[pos++];
            //    qDebug() << value;
            node->m_value = value.trimmed();
        } else {
            // self closing node
            node->m_selfclosing = true;
        }

        // increase number of nodes
        ++m_size;

        // Determine the next node
        if(list[pos] == "<") {
            // the next node is a child node
            XMLNode * child = new XMLNode;
            if(!child)
                throw "No memory left";
            child->m_parent = node;
            node->add_child(child);
            node = child;
        }
        else if (node->m_parent) {
            // the next node is a sibling
            // for the current node
            XMLNode * sibling = new XMLNode;
            if(!sibling)
                throw "No memory left";
            sibling->m_parent = node->m_parent;
            node->m_parent->add_child(sibling);
            pos++;
            node = sibling;
        }
    }
}
