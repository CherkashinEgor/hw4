#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void adjust_after_insert(AVLNode<Key, Value>* n, AVLNode<Key, Value>* p);
    void rotateRight(AVLNode<Key, Value>* node);
    void rotateLeft(AVLNode<Key, Value>* node);
    void update_balance(AVLNode<Key, Value>* node, int8_t balance);
    void adjust_after_remove(AVLNode<Key,Value>* node, char diff);

};

template<class Key, class Value>
void AVLTree<Key, Value>::update_balance(AVLNode<Key, Value>* node, int8_t balance){
    if(node->getParent() == NULL){
        return;
    }
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this->empty() == true){
        AVLNode<Key, Value>* new_root = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        new_root->setBalance(0);
        this->root_ = new_root;
        return;
    }

    Node<Key, Value>* found = this->internalFind(new_item.first);
    if(found != NULL){
        found->setValue(new_item.second);
    } else {
        AVLNode<Key, Value>* temp = (AVLNode<Key, Value>*)AVLTree<Key, Value>::root_;
        // AVLNode<Key, Value>* parent = temp;
        int8_t difference;
        AVLNode<Key, Value>* item;
        while(temp != NULL){
            if(new_item.first > temp->getKey()){
                if(temp->getRight() == NULL){
                    item = new AVLNode<Key, Value>(new_item.first, new_item.second, temp);
                    temp->setRight(item);
                    item->setBalance(0);
                    difference = 1;
                    update_balance(item, difference);
                    break;
                } else {
                    temp = temp->getRight();
                }
            } else {
                if(temp->getLeft() == NULL){
                    item = new AVLNode<Key, Value>(new_item.first, new_item.second, temp);
                    temp->setLeft(item);
                    item->setBalance(0);
                    difference = -1;
                    update_balance(item, difference);
                    break;
                } else {
                    temp = temp->getLeft();
                }
            }
        }
        
        if(temp->getParent() != NULL){
            if(abs(temp->getParent()->getBalance()) == 1){
                temp->getParent()->setBalance(0);
            } else {
                if(temp->getParent()->getLeft() == temp)
                {
                    temp->getParent()->setBalance(-1);
                }
                else
                {
                    temp->getParent()->setBalance(1);
                }
                adjust_after_insert(item,item->getParent());
            }
        }

    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::adjust_after_insert(AVLNode<Key, Value>* n, AVLNode<Key, Value>* p){
    if(p == NULL || p->getParent() == NULL){
        return;
    }
    AVLNode<Key, Value>* g = p->getParent();
    // bool left_child = false;
    bool right_child = false;

    if(p == g->getLeft()){
        // left_child = true;
        g->updateBalance(-1);
    }
    if(p == g->getRight()){
        right_child = true;
        g->updateBalance(1);
    }
    if(g->getBalance() == 0){
        return;
    } else if(abs(g->getBalance()) == 1){
        adjust_after_insert(p, g);
    } else if(abs(g->getBalance()) == 2){
        //Right right or left left
        if(p == g->getRight() && n == p->getRight()){
            //rotate left
            rotateLeft(g);
            p->setBalance(0);
            g->setBalance(0);
        } else if (p == g->getLeft() && n == p->getLeft()){
            //rotate right
            rotateRight(g);
            p->setBalance(0);
            g->setBalance(0);
        }
        else if(p == g->getLeft() && n == p->getRight()){
            //Left right
            rotateLeft(p);
            rotateRight(g);
        } else if(p == g->getRight() && n == p->getLeft()){
            // Right left
            rotateRight(p);
            rotateLeft(g);
        }
        if(right_child){
            if(n->getBalance() == 1){
                g->setBalance(-1);
                p->setBalance(0);
                n->setBalance(0);
            } else if (n->getBalance() == 0){
                g->setBalance(0);
                p->setBalance(0);
                n->setBalance(0);
            } else if (n->getBalance() == -1){
                g->setBalance(0);
                p->setBalance(1);
                n->setBalance(0);
            }
        } else {
            if(n->getBalance() == 1){
                g->setBalance(0);
                p->setBalance(-1);
                n->setBalance(0);
            } else if (n->getBalance() == 0){
                g->setBalance(0);
                p->setBalance(0);
                n->setBalance(0);
            } else if (n->getBalance() == -1){
                g->setBalance(1);
                p->setBalance(0);
                n->setBalance(0);
            }
        }
    }


}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node){
    AVLNode<Key, Value>* edge = NULL;
    if(node->getLeft()->getRight() != NULL){
        edge = node->getLeft()->getRight();
        node->getLeft()->setRight(NULL);
    }
    if(node->getParent() == NULL){
        node->getLeft()->setParent(NULL);
        this->root_ = node->getLeft();
        node->setParent(node->getLeft());
        node->getLeft()->setRight(node);
    } else {
        if(node->getParent()->getRight() == node){
            node->getParent()->setRight(node->getLeft());
        } else {
            node->getParent()->setLeft(node->getLeft());
        }
        node->getLeft()->setParent(node->getParent());
        node->setParent(node->getLeft());
        node->getLeft()->setRight(node);
    }

    if(edge != NULL){
        node->setLeft(edge);
        edge->setParent(node);
    } else {
        node->setLeft(NULL);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node){
    AVLNode<Key, Value>* edge = NULL;
    if(node->getRight()->getLeft() != NULL){
        edge = node->getRight()->getLeft();
        node->getRight()->setLeft(NULL);
    }
    if(node->getParent() == NULL){
        node->getRight()->setParent(NULL);
        this->root_ = node->getRight();
        node->setParent(node->getRight());
        node->getRight()->setLeft(node);
    } else {
        if(node->getParent()->getRight() == node){
            node->getParent()->setRight(node->getRight());
        } else {
            node->getParent()->setLeft(node->getRight());
        }
        node->getRight()->setParent(node->getParent());
        node->setParent(node->getRight());
        node->getRight()->setLeft(node);
    }

    if(edge != NULL){
        node->setRight(edge);
        edge->setParent(node);
    } else {
        node->setRight(NULL);
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key,Value>*>(this->internalFind(key));
    if(node == NULL){
        return;
    }
    if(node->getLeft() != NULL && node->getRight() != NULL){
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(node));
        nodeSwap(node, pred);
    }
    AVLNode<Key, Value>* parent = node->getParent();
    char diff;
    if(parent != NULL){
        if(parent->getLeft() == node){
            diff = 1;
        } else {
            diff = -1;
        }
    }
    if(node->getRight() == NULL && node->getLeft() == NULL){
        if(node->getParent() == NULL){
            this->root_ = NULL;
            delete node;
            return;
        }
        if(parent->getLeft() == node){
            parent->setLeft(NULL);
        } else if (parent->getRight() == node){
            parent->setRight(NULL);
        }
        delete node;
        return; // Perhaps
    } else {
        if(node->getParent() == NULL){
            if(node->getLeft() != NULL){
                this->root_ = node->getLeft();
                node->getLeft()->setParent(NULL);
                delete node;
            } else {
                this->root_ = node->getRight();
                node->getRight()->setParent(NULL);
                delete node;
            }
        } else if(node->getLeft() != NULL){
            if(node->getParent()->getLeft() == node){
                node->getParent()->setLeft(node->getLeft());
            } else {
                node->getParent()->setRight(node->getLeft());
            }
            node->getLeft()->setParent(node->getParent());
            delete node;
        } else if(node->getRight() != NULL){
            if(node->getParent()->getLeft() == node){
                node->getParent()->setLeft(node->getRight());
            } else {
                node->getParent()->setRight(node->getRight());
            }
            node->getRight()->setParent(node->getParent());
            delete node;
        }
    }
    adjust_after_remove(parent, diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>::adjust_after_remove(AVLNode<Key,Value>* node, char diff) {
    if(node == NULL){
        return;
    }
    char ndiff;
    AVLNode<Key, Value>* p = node->getParent();
    if(node->getParent() != NULL){
        if(p->getLeft() == node){
            ndiff = 1;
        } else {
            ndiff = -1;
        }
    }
    if(diff == 1){
        if(node->getBalance() + diff == 2){
            AVLNode<Key, Value>* c = node->getRight();
            if(c->getBalance() == 1){
                rotateLeft(node);
                c->setBalance(0);
                node->setBalance(0);
                adjust_after_remove(p, ndiff);
            } else if (c->getBalance() == 0){
                rotateLeft(node);
                node->setBalance(1);
                c->setBalance(-1);
            } else if (c->getBalance() == -1) {
                AVLNode<Key, Value>* g = p->getParent();
                rotateRight(c);
                rotateLeft(node);
                if(g->getBalance() == -1){
                    node->setBalance(0);
                    c->setBalance(1);
                    g->setBalance(0);
                } else if (g->getBalance() == 0){
                    node->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                } else {
                    node->setBalance(-1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                adjust_after_remove(p, ndiff);
            }
        } else if (node->getBalance() + diff == 1){
            node->setBalance(1);
        } else {
            node->setBalance(0);
            adjust_after_remove(p, ndiff);
        }
    }
    return;
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
