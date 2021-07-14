#include "lib/xmltree.h"
#include "lib/json.h"

#include <QFile>

void test_xmltree()
{
    XMLTree tree;
//    tree.dump(4);
    QString builder;
    QTextStream ts(&builder);
    ts << "<Tag1 att1=val1 att2=val2>\n"
    << "<Tag2 att11=val11 att21=val21>\n"
    << "VALUE value also \n still value \n"
    << "</Tag2>\n"
    << "<Tag2 att11=val11 att21=val21>\n"
    << "VALUE value also \n still value \n"
    << "</Tag2>\n"
    << "</Tag1>\n";
        QFile file("../xml-editor/data/data-so-sample.xml");
        file.open(QFile::ReadOnly);
        QTextStream fs(&file);
    tree.load(fs);
    JSON::xml2json(tree, 2);
}

void test_xml_syntax_check()
{
    XMLTree tree;
//    tree.dump(4);
    QString builder;
    QTextStream ts(&builder);
    ts << "<Tag1 att1=val1 att2=val2>\n"
    << "<Tag2 att11=val11 att21=val21>\n"
    << "VALUE value also \n still value \n"
    << "</Tag2>\n"
    << "<Tag2 att11=val11 att21=val21>\n"
    << "VALUE value also \n still value \n"
    << "</Tag2>\n"
    << "</Tag1>\n";
    XMLTree::syntax_check(ts);
}

void xml_test_all()
{
    test_xmltree();
//    test_xml_syntax_check();
}
