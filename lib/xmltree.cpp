#include "xmltree.h"
#include <QFile>
#include <QStringBuilder>
#include <QTextStream>
#include <iostream>
#include <QFileInfo>

XMLTree::XMLTree()
    : m_root(nullptr), m_size(0)
{
    //    root = new XMLNode;
    //    root->set_tag("Tag1");
    //    root->add_attribute("att1", "val1");
    //    root->add_attribute("att2", "val2");

    //    XMLNode * node = new XMLNode;
    //    node->set_tag("Tag2");
    //    node->set_value("VALUE");
    //    node->add_attribute("att1.1", "val1.1");
    //    node->add_attribute("att2.1", "val2.1");

    //    root->add_child(node);

}

XMLTree::~XMLTree()
{
    delete m_root;
    m_root = nullptr;
}

void XMLTree::dump(int spaces) const
{
    QString builder;
    QTextStream ts(&builder);


    QString indent = "";
    for(int i = 0; i < spaces; i++)
        indent += " ";
    dump_helper(m_root, spaces, 0, ts);
    while(!ts.atEnd())
        qDebug().noquote().nospace() << ts.readLine();

}

void  XMLTree::dump_helper(XMLNode * node, int spaces, int depth, QTextStream& output) const
{
    if(node == nullptr)
        return;

    QString indent;
    QString end_line;
    QString local_indent = "";

    if(spaces >= 0) {
        indent.reserve(spaces + 2);
        for(int i = 0; i < spaces; i++)
            local_indent += " ";

        indent.reserve(spaces * depth + 2);
        for(int i = 0; i < depth; i++)
            indent += local_indent;

        end_line = "\n";
    } else {
        indent = "";
        end_line = "";
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

void XMLTree::load(QTextStream &input)
{
    QFile file("../xml-editor/data/data-so-sample.xml");
    file.open(QFile::ReadOnly);
    QTextStream ts(&file);
    QString str = ts.readAll();

    // regex to tokize the XML text
    QString word = "[\\w\\.\\$\\%\\^\\&\\#\\@\\*\\(\\-\\+\\-\\):']+";
    QString literal_string = "\"[\\w\\s\\.\\$\\%\\^\\&\\#\\@\\*\\(\\-\\+\\-\\):/'`,;]+\"";
    QString white_spaces = "[\\s]+";
    QString xml_tokens = "<|>|</|=|/>|-->|<!--";
    QRegExp rx("(" + xml_tokens + "|"
                   + white_spaces + "|"
                   + literal_string + "|"
                   + word + ")");
    //    rx.setMinimal(true);
    QStringList list;
    int pos = 0;

    // extract the capctured groups
    while ((pos = rx.indexIn(str, pos)) != -1) {
        list << rx.cap(1);
        pos += rx.matchedLength();
    }
    //    for(int i = 71780; i  < 71780 + 40 ; i++)
    qDebug() << list.size();

    // start loading the tree
    if(m_root)
        delete m_root;
    m_root = new XMLNode;
    m_root->m_parent = nullptr;
    load_helper(list, 0, m_root);
    dump(2);
    dump(0);
    dump();
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
        // ignore the closing tag
        ++pos;


        if(list[pos - 1] == ">") {
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
