/******************************************************************************
 * Copyright (C) 2020 by Hassan El-shazly
 *
 * Redistribution, modification or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright.
 *
 *****************************************************************************/
/**
  * @file hnode.h
  *
  * This file defines HNode class
  * A simple tree node used to in huffman tree implementation
  *
  * @author Hassan El-shazly
  * @date Last Edit Jun-2021
  *
  */

#ifndef _HNODE_H_
#define _HNODE_H_

class HNode
{
public:
    /**
     * Default constructor
     */
    HNode();

    /**
     * construct a leaf node with just data
     */
    HNode(char data);

    /**
     * construct node with left, right and data
     */
    HNode(HNode *left, HNode *right, char data);

    /**
     * @return data
     */
    char get_data();

    /**
     * @return left child
     */
    HNode *get_left();

    /**
     * @return right child
     */
    HNode *get_right();

    /**
     * overwite the data
     */
    void set_data(char new_data);

    /**
     * overwite left child
     */
    void set_left(HNode * new_left);

    /**
     * overwite right child
     */
    void set_right(HNode * new_right);

    /**
     * @return ture if leaf node
     */
    bool is_leaf();

    /**
     * friend method to delete a given tree
     * @param node root of the tree
     */
    friend void del_tree(HNode *node);

    friend class Huffman;

private:
    HNode *left;
    HNode *right;
    char data;
};

#endif // End of the file
