#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "avl.h"

#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

using namespace std;

/* public methods */

AVLTree::~AVLTree() {
    makeEmpty(root);
}

void AVLTree::Insert(int32_t val) {
    Insert(val, root);
}

void AVLTree::Delete(int32_t val) {
    Delete(val, root);
}

bool AVLTree::Access(int32_t val) {
    Access(val, root);
}


std::string AVLTree::PrintPreOrder () const {
    traversalResult = "";
    PrintPreOrder(root);
    return traversalResult;
}

void AVLTree::PrintPreOrder(AVLNode *node) const {
    if (node != NULL) {
        if (traversalResult == "")
            traversalResult += std::to_string(node->elem);
        else
            traversalResult += " " + std::to_string(node->elem);

        PrintPreOrder(node->left);
        PrintPreOrder(node->right);
    }
}


std::string AVLTree::PrintInOrder () const {
    traversalResult = "";
    PrintInOrder(root);
    return traversalResult;
}

void AVLTree::PrintInOrder(AVLNode *node) const {
    if (node != NULL) {
        PrintInOrder(node->left);

        if (traversalResult == "")
            traversalResult += std::to_string(node->elem);
        else
            traversalResult += " " + std::to_string(node->elem);

        PrintInOrder(node->right);
    }
}

int AVLTree::Height(AVLNode *node) const {
    if (node == NULL) return 0;

    int hLeft = 0;
    int hRight = 0;

    if(node->left != NULL)
        hLeft = Height(node->left);
    if(node->right != NULL)
        hRight = Height(node->right);

    return max(hLeft, hRight) + 1;
}

int AVLTree::BalanceFactor(AVLNode *node) const {
    if (node == NULL)
        return 0;

    return Height(node->left) - Height(node->right);
}

/* ROTATIONS
 * Input: "trouble finder" node/root of subtree that is unbalanced
 * Output: the new root of rotated subtree
 * */

/* case 1: LL rotation - single: right fix
         H                           M
       /   \                       /   \
      M     c        --->        L      H
    /   \                      /      /   \
   L     b                    x      b     c
  /
 x
*/
AVLNode* AVLTree::rightRotate(AVLNode *&node) {
    AVLNode *mid = node->left;
    AVLNode *midRightSubtree = mid->right;

    // rotate
    mid->right = node;
    node->left = midRightSubtree;

    // Update heights
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    mid->height = max(getHeight(mid->left), node->height) + 1;

    node = mid;

    return mid;

}

/* case 2: RR rotation - single: left fix
         L                           M
       /   \                       /   \
      a     M        --->        L      H
          /   \                 / \       \
         b     H               a   b       x
                \
                 x
*/

AVLNode* AVLTree::leftRotate(AVLNode *&node) {
    AVLNode *mid = node->right;
    AVLNode *midLeftSubtree = mid->left;

    // rotate
    mid->left = node;
    node->right = midLeftSubtree;

    // Update heights
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    mid->height = max(getHeight(mid->left), node->height) + 1;

    node = mid;

    return mid;

}

/* case 3: LR rotation - double: left, right fix
         H                           M
       /   \                       /   \
      L     c       --->          L      H
    /   \                       /       /  \
   a     M                     a       x    c
          \
           x

 */

AVLNode* AVLTree::leftRightRotate(AVLNode *&node) {
    AVLNode *low = node->left;
    AVLNode *mid = low->right;
    AVLNode *midRightSubtree = mid->right;
    AVLNode *midLeftSubtree = mid->left;

    // rotate
    node->left = midRightSubtree;
    mid->right = node;
    low->right = midLeftSubtree;
    mid->left = low;

    // Update heights
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    mid->height = max(getHeight(mid->left), node->height) + 1;

    node = mid;

    return mid;

}



/* case 4: RL rotation - double: right, left fix
         L                           M
       /   \                       /   \
      a     H       --->          L     H
           /  \                  /     /  \
          M    b                a     x    b
           \
            x

 */
AVLNode* AVLTree::rightLeftRotate(AVLNode *&node) {

    AVLNode *high = node->right;
    AVLNode *mid = high->left;
    AVLNode *midRightSubtree = mid->right;
    AVLNode *midLeftSubtree = mid->left;

    // rotate
    node->right = midLeftSubtree;
    mid->left = node;
    mid->right = high;
    high->left = midRightSubtree;

    // Update heights
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    mid->height = max(getHeight(mid->left), node->height) + 1;

    node = mid;

    return mid;

}

AVLNode* AVLTree::findMin(AVLNode *node)
{
    if(node != NULL) {
        while (node->left != NULL)
            node = node->left;
    }
    return node;
}

void AVLTree::Delete(int32_t val, AVLNode * & node) {
    if (node == NULL)
        return;
    if (val < node->elem)
        Delete(val, node->left);
    else if (node->elem < val)
        Delete(val, node->right);
    else if (node->left != NULL && node->right != NULL) {
        node->elem = findMin(node->right)->elem;
        Delete(node->elem, node->right);
    }
    else {
        AVLNode *oldNode = node;
        node = (node->left != NULL) ? node->left : node->right;
        delete oldNode;
    }
    balance(node);
}


void AVLTree::Insert(const Element e, AVLNode * & node){
    // Insert at leaf level
    if(node == NULL) {
        node = new AVLNode(e);
    }
    else if(e == node->elem) {
        return;
    }
    else if(e < node->elem) {
        Insert(e, node->left);
    }
    else if(e > node->elem) {
        Insert(e, node->right);
    }

    balance(node);
}

AVLNode* AVLTree::balance(AVLNode * & node) {
    if (node == NULL)
        return node;
    if (getHeight(node->left) - getHeight(node->right) > 1) {
        if (getHeight(node->left->left) >= getHeight(node->left->right))
            return rightRotate(node);
        else
            return leftRightRotate(node);
    }
    else {
        if(getHeight( node->right ) - getHeight( node->left ) > 1) {
            if( getHeight( node->right->right ) >= getHeight( node->right->left ) )
                return leftRotate(node);
            else
                return rightLeftRotate(node);
        }
    }
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
}


bool AVLTree::Access(const Element e, AVLNode * & node){
    if(node == NULL) {
        return false;
    }
    else if(e == node->elem) {
        return true;
    }
    else if(e < node->elem) {
        return Access(e, node->left);
    }
    else {
        return Access(e, node->right);
    }
}

void AVLTree::makeEmpty(AVLNode * & node)
{
    if(node != NULL) {
        makeEmpty(node->left);
        makeEmpty(node->right);
        delete node;
    }
    node = NULL;
}


