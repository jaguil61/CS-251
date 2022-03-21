/*bstt.h*/

// Jose M. Aguilar (jaguil61)
// Threaded binary search tree
//

#pragma once

#include <iostream>

using namespace std;

template<typename KeyT, typename ValueT>
class bstt
{
private:
    struct NODE
    {
        KeyT   Key;
        ValueT Value;
        NODE* Left;
        NODE* Right;
        bool   isThreaded;
    };

    NODE* Root;  // pointer to root node of tree (nullptr if empty)
    int   Size;  // # of nodes in the tree (0 if empty)
    NODE* curOrder; // pointer to the first in-order node

    // Called by the destructor to clear give memory back
    void _postOrderDelete(NODE* cur)
    {
        if (cur == nullptr) //Already empty
            return;

        else // Post order traversal to delete whole tree 
        {
            _postOrderDelete(cur->Left);

            if (cur->isThreaded)
            {
                delete cur;
                return;
            }

            else
            {
                _postOrderDelete(cur->Right);
                delete cur;
            }
        }
    }

    //Prints the tree in-order traversal
    void _inOrderPrint(NODE* cur) const
    {
        if (cur == nullptr)
            return;

        else
        {
            _inOrderPrint(cur->Left);

            if (cur->isThreaded)
            {
                if (cur->Right == nullptr)
                    cout << "(" << cur->Key << "," << cur->Value << ")" << endl;

                else
                {
                    cout << "(" << cur->Key << "," << cur->Value << "," << cur->Right->Key << ")" << endl;
                    return;
                }

            }

            else
            {
                cout << "(" << cur->Key << "," << cur->Value << ")" << endl;
                _inOrderPrint(cur->Right);
            }
        }
    }

    // Makes a copy in pre-order traversal
    void _preOrderCopy(NODE* cur)
    {
        if (cur == nullptr)
            return;

        else
        {
            insert(cur->Key, cur->Value);

            _preOrderCopy(cur->Left);

            if (cur->isThreaded)
            {
                insert(cur->Key, cur->Value);
                return;
            }

            else
                _preOrderCopy(cur->Right);
        }
    }

    //used by to find the most left Node
    NODE* _inOrderFirst(NODE* cur)
    {
        if (cur == nullptr)
            return nullptr;

        else
        {
            while (cur->Left != nullptr)
                cur = cur->Left;

            return cur;
        }
    }


public:
    /*
    // default constructor:
    //
    // Creates an empty tree.
    */
    bstt()
    {
        Root = nullptr;
        Size = 0;
        curOrder = nullptr;
    }

    //
    // copy constructor
    //
    bstt(const bstt& other)
    {
        this->Root = nullptr;
        this->Size = 0;
        this->curOrder = other.curOrder;

        // call _preOrder copy Constructor
        _preOrderCopy(other.Root);
    }

    /*
    // destructor:
    //
    // Called automatically by system when tree is about to be destroyed;
    // this is our last chance to free any resources / memory used by
    // this tree.
    */
    virtual ~bstt()
    {
        // Call a helper function that will actually delete everything
        _postOrderDelete(Root);
    }

    //
    // operator=
    //
    // Clears "this" tree and then makes a copy of the "other" tree.
    //
    bstt& operator=(const bstt& other)
    {
        // clear "this" tree
        this->clear();

        // call _preOrderCopy() function to make copy
        _preOrderCopy(other.Root);

        this->curOrder = other.curOrder;

        return *this;
    }

    //
    // clear:
    //
    // Clears the contents of the tree, resetting the tree to empty.
    //
    void clear()
    {
        // Call _postOrder() and set the Root = nullptr | size = 0;
        _postOrderDelete(Root);

        Root = nullptr;
        Size = 0;
        curOrder = nullptr;
    }

    // 
    // size:
    //
    // Returns the # of nodes in the tree, 0 if empty.
    //
    // Time complexity:  O(1) 
    //
    int size() const
    {
        return Size;
    }

    // 
    // search:
    //
    // Searches the tree for the given key, returning true if found
    // and false if not.  If the key is found, the corresponding value
    // is returned via the reference parameter.
    //
    // Time complexity:  O(lgN) on average
    //
    bool search(KeyT key, ValueT& value) const
    {
        // Declare cur ptr
        NODE* cur = Root;

        // Loop and search
        while (cur != nullptr)
        {
            if (key == cur->Key)
            {
                value = cur->Value;

                return true; //Found
            }

            else if (key < cur->Key)
                cur = cur->Left;

            else
            {
                if (cur->isThreaded)
                    cur = nullptr; // Exits out of loop 

                else
                    cur = cur->Right;
            }
        }

        return false; //Not found
    }

    //
    // insert
    //
    // Inserts the given key into the tree; if the key has already been insert then
    // the function returns without changing the tree.
    //
    // Time complexity:  O(lgN) on average
    //
    void insert(KeyT key, ValueT value)
    {
        NODE* prev = nullptr;
        NODE* cur = Root;

        // Search to see if tree already contains key:
        while (cur != nullptr)
        {
            if (key == cur->Key) // already in tree
                return;

            if (key < cur->Key) // search left
            {
                prev = cur;
                cur = cur->Left;
            }

            else //searching right
            {
                if (cur->isThreaded)
                {
                    prev = cur;
                    cur = nullptr;
                }

                else
                {
                    prev = cur;
                    cur = cur->Right;
                }

            }
        }

        // if we get here, key is not in tree, so allocate a new node to insert:
        NODE* n = new NODE();
        n->Key = key;
        n->Value = value;
        n->Left = nullptr;
        n->Right = nullptr;
        n->isThreaded = true;

        // Link in the new node where we fell off:
        if (prev == nullptr) // empty tree   
        {
            Root = n;
            //cout << "Root " << n->Key << " " << n->Value << endl; // for debugging
        }

        else if (key < prev->Key) // insert to the left
        {
            prev->Left = n;
            n->Right = prev; // Points to the parent node
            //cout << "Left " << n->Key << " " << n->Value << endl; // for debugging
            //cout << "Parent Node: " << n->Right->Key << endl; // for debugging
        }

        else // insert to the right
        {
            if (prev->isThreaded) // Threaded
            {
                n->Right = prev->Right; // Points to prev thread
                prev->Right = n; // prev right now points to n node
                prev->isThreaded = false;
                //cout << "Right " << n->Key << " " << n->Value << "(prev was threaded)" << endl; // for debugging   

            }

            else // Not threaded
            {
                prev->Right = n;
                //cout << "Right " << n->Key << " " << n->Value << "(Prev was not threaded)" << endl; // for debugging   
            }
        }

        // Update size
        Size++;
    }

    //
    // []
    //
    // Returns the value for the given key; if the key is not found,
    // the default value ValueT{} is returned.
    //
    // Time complexity:  O(lgN) on average
    //
    ValueT operator[](KeyT key) const
    {
        ValueT val = ValueT{ };

        /* call search function
        If found return the value */
        if (search(key, val)) // found
            return val;

        // Else return default value
        else
            return ValueT{ }; // not found
    }

    //
    // ()
    //
    // Finds the key in the tree, and returns the key to the "right".
    // If the right is threaded, this will be the next inorder key.
    // if the right is not threaded, it will be the key of whatever
    // node is immediately to the right.
    //
    // If no such key exists, or there is no key to the "right", the
    // default key value KeyT{} is returned.
    //
    // Time complexity:  O(lgN) on average
    //
    KeyT operator()(KeyT key) const
    {
        NODE* cur = Root;

        /* Perform search
        if found return the next inorder key*/
        while (cur != nullptr)
        {
            if (key == cur->Key)
            {
                if (cur->Right != nullptr)
                    return cur->Right->Key; // Found

                else
                    cur = nullptr; // Found but the right node was null
            }

            else if (key < cur->Key)
                cur = cur->Left;

            else
            {
                if (cur->isThreaded)
                    cur = nullptr; // Exits out of loop 

                else
                    cur = cur->Right;
            }
        }

        // else return default value
        return KeyT{ }; // not found
    }

    //
    // begin
    //
    // Resets internal state for an inorder traversal.  After the 
    // call to begin(), the internal state denotes the first inorder
    // key; this ensure that first call to next() function returns
    // the first inorder key.
    //
    // Space complexity: O(1)
    // Time complexity:  O(lgN) on average
    //
    // Example usage:
    //    tree.begin();
    //    while (tree.next(key))
    //      cout << key << endl;
    //
    void begin()
    {
        NODE* cur = Root;

        if (cur == nullptr)
            return;

        curOrder = _inOrderFirst(cur); //first in-order node

        //cout << "First key in in-order traversal: " << curOrder->Key << endl; // for debugging   
    }

    //
    // next
    //
    // Uses the internal state to return the next inorder key, and 
    // then advances the internal state in anticipation of future
    // calls.  If a key is in fact returned (via the reference 
    // parameter), true is also returned.
    //
    // False is returned when the internal state has reached null,
    // meaning no more keys are available.  This is the end of the
    // inorder traversal.
    //
    // Space complexity: O(1)
    // Time complexity:  O(lgN) on average
    //
    // Example usage:
    //    tree.begin();
    //    while (tree.next(key))
    //      cout << key << endl;
    //
    bool next(KeyT& key)
    {

        if (curOrder == nullptr)
            return false;

        else
        {
            key = curOrder->Key;

            //go right
            if (curOrder->isThreaded)
                curOrder = curOrder->Right;

            //sometimes go left (all the way)
            else
            {
                curOrder = _inOrderFirst(curOrder->Right);
            }

            return true;
        }
    }

    //
    // dump
    // 
    // Dumps the contents of the tree to the output stream, using a
    // recursive inorder traversal.
    //
    void dump(ostream& output) const
    {
        NODE* cur = this->Root;
        output << "**************************************************" << endl;
        output << "********************* BSTT ***********************" << endl;

        output << "** size: " << this->size() << endl;

        //
        // inorder traversal, with one output per line: either 
        // (key,value) or (key,value,THREAD)
        //
        // (key,value) if the node is not threaded OR thread==nullptr
        // (key,value,THREAD) if the node is threaded and THREAD denotes the next inorder key
        //

        //
        // Call inorder function to print tree
        _inOrderPrint(cur);

        output << "**************************************************" << endl;
    }

};

