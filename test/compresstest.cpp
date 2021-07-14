#include <string>
#include <fstream>

#include "compress/huffman.h"

void test_huffman()
{
    huffman huff;
    std::string inputfile = "../xml-editor/data/data.adj.xml";
    std::string encoded_outputfile = "../xml-editor/data/data.adj.huff";
    std::string decoded_outputfile = "../xml-editor/data/data.adj.outhuff";

    qDebug() << "Compressing ...";
    std::ifstream input_file(inputfile, std::ios::in | std::ios::binary);
    std::ofstream output_file(encoded_outputfile, std::ios::out | std::ios::binary);

    float ratio = huff.encode(input_file, output_file);
    qDebug() << "Compression ratio: " << ratio * 100 << "%";

    input_file.close();
    output_file.close();

    qDebug() << "Decompressing ...";

    std::ifstream encoded_input_file(encoded_outputfile, std::ios::in | std::ios::binary);
    std::ofstream encoded_output_file(decoded_outputfile, std::ios::out | std::ios::binary);

    huff.decode(encoded_input_file, encoded_output_file);

    encoded_input_file.close();
    encoded_input_file.close();

    qDebug() << "Finished";

}

void compress_test_all()
{
//    test_huffman();
}
