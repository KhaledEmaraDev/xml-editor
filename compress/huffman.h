/******************************************************************************
 * Copyright (C) 2020 by Hassan El-shazly
 *
 * Redistribution, modification or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright.
 *
 *****************************************************************************/
 /**
  *
  * @author Hassan El-shazly
  * @date Last Edit Jun-2021
  *
  */
#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include <string>
#include <vector>
#include <fstream>

#include "hnode.h"
#include "lib/hashmap.h"

using std::ifstream;
using std::vector;
using std::ios;
using std::ofstream;
using std::string;

class huffman
{
public:
    /**
     * Default constructor
     * calling reader() is required before
     * encoding or decoding
     */
    huffman();

    /**
     * compress the input file to output_file
     * @param input_file  an opend file with binary read permissions
     * @param output_file an opend file with binary write permissions
     *
     * @return compression ratio
     */
    float encode(ifstream &input_file, ofstream &output_file);

    /**
     * decompress the input file to output_file
     * @param input_file  an opend file with binary read permissions
     * @param output_file an opend file with binary write permissions
     */
    void decode(ifstream &input_file, ofstream &output_file);

private:
    /**
     * read bit from input stream
     * store internal buffer to hold the information unit
     */
    bool read_bit(ifstream &input_file, bool final = 0);

    /**
     * read byte from input stream
     * synchronous with read_bit()
     */
    char read_byte(ifstream &input_file);

    /**
     * write bit to output stream
     * store internal buffer to hold the information unit
     * @param final write the stored buffer
     */
    void write_bit(ofstream &output_file, bool bit, bool final = 0);

    /**
     * write byte to output stream
     * synchronous with write_bit()
     */
    void write_byte(ofstream &output_file, char byte);

    /**
     * read entire file to text
     */
    void read_file(ifstream &input_file);

    /**
     * verify signature of the file
     */
    bool verify_sign(ifstream &input_file);

    /**
     * write signature to an output file
     */
    void store_sign(ofstream &output_file);

    /**
     * store huffman tree to output filesteam
     */
    void store_tree(ofstream &output_file, HNode *root);

    /**
     * calculate file size
     */
    uint64_t file_size(vector<int>& freqs, HashMap<char, string> &codes);

    /**
     * encode text base of the given charcter map
     */
    void encode_file(ofstream &output_file, HashMap<char, string> &codes);

    /**
     * decode text base of the given huffman tree
     */
    void decode_file(ifstream &input_file, ofstream &output_file, HNode *root);

    /**
     * read huffman tree from encoded data
     */
    HNode *read_tree(ifstream &input_file);

    /**
     * compute the frequencies of the charcters
     */
    vector<int> compute_freqs();

    /**
     * generate huffman tree
     */
    HNode *generate_tree(vector<int>& freqs);

    /**
     * generate huffman codes
     */
    HashMap<char, string> generate_codes(HNode *root);

    /**
     * to save the content of the file
     */
    string text;
};

#endif // End of the file
