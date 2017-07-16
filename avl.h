#ifndef AVL_H_
#define AVL_H_
#define Element int
#include <string>
#include <sstream>

struct AVLNode {
    Element elem;
    //NSMutableString * elemString = elem.c_str();
    AVLNode *left;
    AVLNode *right;
    //AVLNode *parent;

    int height;
    int balanceFactor;

    AVLNode(): elem(0), left(NULL), right(NULL),
               height(1), balanceFactor(0) {}

    AVLNode(const Element &e): elem(e), left(NULL), right(NULL),
                               height(1), balanceFactor(0) {}
};

class AVLTree
{
public:


    AVLTree (): root() {}
    virtual ~AVLTree();
    void Insert (int32_t val);
    // choose smallest node on the right subtree when deleting a node with two children
    void Delete (int32_t val);
    bool Access (int32_t val);
    std::string PrintPreOrder () const;
    std::string PrintInOrder () const;

    // get height of tree where node is the root
    int Height (AVLNode *node) const;

    //get balance factor of tree where node is the root
    int BalanceFactor(AVLNode *node) const;

    /* 4 rotation functions.
    * Input: "trouble finder" node/root of subtree that is unbalanced
    * Output: the new root of rotated subtree
    * */

    // case 1: LL rotation - single: right fix
    AVLNode* rightRotate(AVLNode *&node);

    // case 2: RR rotation - single: left fix
    AVLNode* leftRotate(AVLNode *&node);

    // case 3: LR rotation - double: left, right fix
    AVLNode* leftRightRotate(AVLNode *&node);

    // case 4: RL rotation - double: right, left fix
    AVLNode* rightLeftRotate(AVLNode *&node);

    AVLNode* balance(AVLNode * & node);

private:

    AVLNode *root = NULL;


    mutable std::string traversalResult;


//    std::string PrintPreOrder(AVLNode *node) const;
//    std::string PrintInOrder(AVLNode *node) const;

    void PrintPreOrder(AVLNode *node) const;
    void PrintInOrder(AVLNode *node) const;


    void Insert(const Element e, AVLNode * & node);

    AVLNode* findMin(AVLNode *node);

    void Delete(const Element e, AVLNode * & node);

    bool Access(const Element e, AVLNode * & node);

    void makeEmpty(AVLNode * & node);

    int getHeight(AVLNode *node) const {
        return node == NULL ? -1 : node->height;
    }


};

#endif /* end of include guard: AVL_H_ */

// NTS:
// try replacing this->root with just root