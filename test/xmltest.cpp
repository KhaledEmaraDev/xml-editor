#include "lib/xmltree.h"

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
    tree.load(ts);
}

void xml_test_all()
{
    test_xmltree();
}
