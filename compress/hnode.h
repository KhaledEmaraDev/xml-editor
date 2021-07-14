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
    HNode()
        : m_left(nullptr), m_right(nullptr), m_data(0)
    {
        // do nothing
    }

    /**
     * Destructor
     */
    ~HNode()
    {
        delete m_left;
        m_left = nullptr;

        delete m_right;
        m_right = nullptr;
    }

    /**
     * construct a leaf node with just data
     */
    HNode(char data)
        : m_left(nullptr), m_right(nullptr), m_data(data)
    {
        // do nothing
    }

    /**
     * construct node with left, right and data
     */
    HNode(HNode *left, HNode *right, char data)
        : m_left(left), m_right(right), m_data(data)
    {
        // do nothing
    }

    /**
     * @return data
     */
    char data() const
    {
        return m_data;
    }

    /**
     * @return left child
     */
    HNode *left() const
    {
        return m_left;
    }

    /**
     * @return right child
     */
    HNode *right() const
    {
        return m_right;
    }

    /**
     * overwite the data
     */
    void set_data(char data)
    {
        m_data = data;
    }

    /**
     * overwite left child
     */
    void set_left(HNode * left)
    {
        m_left = left;
    }

    /**
     * overwite right child
     */
    void set_right(HNode * right)
    {
        m_right = right;
    }

    /**
     * @return ture if leaf node
     */
    bool is_leaf() const
    {
        return m_left == nullptr && m_right == nullptr;
    }

    friend class Huffman;

private:
    HNode *m_left;
    HNode *m_right;
    char m_data;
};

#endif // End of the file


