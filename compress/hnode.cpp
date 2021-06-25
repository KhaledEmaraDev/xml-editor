/******************************************************************************
 * Copyright (C) 2020 by Hassan El-shazly
 *
 * Redistribution, modification or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright.
 *
 *****************************************************************************/
 /**
  * @file hnode.cpp
  *
  * This file implements HNode class
  * A simple tree node used to in huffman tree implementation
  *
  * @author Hassan El-shazly
  * @date Last Edit Aug-2020
  *
  */

#include "hnode.h"

HNode::HNode()
    : left(nullptr), right(nullptr), data(0)
{
    // do nothing
}
HNode::HNode(char data)
    : left(nullptr), right(nullptr), data(data)
{
    // do nothing
}

HNode::HNode(HNode *left, HNode *right, char data)
    : left(left), right(right), data(data)
{
    // do nothing
}

inline char HNode::get_data()
{
    return data;
}

inline HNode *HNode::get_left()
{
    return left;
}

inline HNode *HNode::get_right()
{
    return right;
}

inline void HNode::set_data(char new_data)
{
    data = new_data;
}
    
inline void HNode::set_left(HNode * new_left)
{
    left = new_left;
}

inline void HNode::set_right(HNode * new_right)
{
    right = new_right;
}

inline bool HNode::is_leaf()
{
    return left == nullptr && right == nullptr;
}

// friend function
void del_tree(HNode *node)
{
    if (node == nullptr)
        return;
    del_tree(node->get_left());
    del_tree(node->get_right());
    delete node;
}
