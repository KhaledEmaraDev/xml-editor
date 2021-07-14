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
     */
    huffman();

    /**
     * @brief encode
     *        compress the input file to output_file
     * @param input_file  an opend file with binary read permissions
     * @param output_file an opend file with binary write permissions
     *
     * @return compression ratio
     *
     * @complexity O(size)
     */
    float encode(ifstream &input_file, ofstream &output_file);

    /**
     * @brief decode
     *        decompress the input file to output_file
     * @param input_file  an opend file with binary read permissions
     * @param output_file an opend file with binary write permissions
     */
    void decode(ifstream &input_file, ofstream &output_file);

private:
    /**
     * read bit from input stream
     * store internal buffer to hold the information unit(Byte)
     * @complexity O(1)
     */
    bool read_bit(ifstream &input_file, bool final = 0);

    /**
     * read byte from input stream
     * synchronous with read_bit()
     * @complexity O(1)
     */
    char read_byte(ifstream &input_file);

    /**
     * write bit to output stream
     * store internal buffer to hold the information unit(Byte)
     * @param final write to the stored buffer
     * @complexity O(1)
     */
    void write_bit(ofstream &output_file, bool bit, bool final = 0);

    /**
     * write byte to output stream
     * synchronous with write_bit()
     * @complexity O(1)
     */
    void write_byte(ofstream &output_file, char byte);

    /**
     * read entire file to text
     * @complexity O(sizeof(input_file))
     */
    void read_file(ifstream &input_file);

    /**
     * verify signature of the file
     * signature is used to make sure the file has been
     * compresed with the same program
     * @complexity O(1)
     */
    bool verify_sign(ifstream &input_file);

    /**
     * write signature to an output file
     * @complexity O(1)
     */
    void store_sign(ofstream &output_file);

    /**
     * store huffman tree to output filesteam
     * @complexity O(sizeof(huffman tree))
     *  O(1) as the size of huffman tree can't exceed 512 node
     */
    void store_tree(ofstream &output_file, HNode *root);

    /**
     * calculate the comprssed file size based on codes and their freqs
     * @complexity O(sizeof(freqs))
     *   O(1) as the max size of freqs is 265
     */
    uint64_t file_size(vector<int>& freqs, HashMap<char, string> &codes);

    /**
     * encode text based on the given charcter map
     * @complexity O(sizeof(input_file))
     */
    void encode_file(ofstream &output_file, HashMap<char, string> &codes);

    /**
     * decode text based on the given huffman tree
     * @complexity O(sizeof(input_file))
     */
    void decode_file(ifstream &input_file, ofstream &output_file, HNode *root);

    /**
     * read huffman tree from encoded data
     * it must be prececed by verify sign
     */
    HNode *read_tree(ifstream &input_file);

    /**
     * compute the frequencies of the charcters on the text
     * @complexity O(sizeof(input_file))
     */
    vector<int> compute_freqs();

    /**
     * generate huffman tree based on the frequency of the charcters
     * @complexity O(sizeof(freqs) * log(sizeof(freqs)))
     *  O(1) as the max size of freqs is 265
     */
    HNode *generate_tree(vector<int>& freqs);

    /**
     * generate huffman codes based on huffman tree
     * @complexity O(sizeof(huffman tree))
     *  O(1) as the size of huffman tree can't exceed 512 node
     */
    HashMap<char, string> generate_codes(HNode *root);

    /**
     * to temporary save the content of the file
     *
     */
    string text;
};

#endif // End of the file
