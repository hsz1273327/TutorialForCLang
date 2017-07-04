#include <stddef.h>
#include <iostream>
template <class T>

struct Node{
    T value;
    Node<T> *left;
    Node<T> *right;
    Node(const T &e) {
        value = e;
        left = right = NULL;
    };
};

template <class T>
class BinaryTree{
    public:
        BinaryTree();
        ~BinaryTree();
        Node<T> *root;
        //清空当前树
        void clear();
        //获取到当前元素
        T getElement()    const;
        //判断当前二叉树是不是空二叉树
        bool isEmpty()    const;
        //构建一棵数
        void makeTree(const T &x,BinaryTree<T> *left,BinaryTree<T> *right);
        //拆除一棵数
        void breakTree(T &x,BinaryTree*left,BinaryTree*right);
        //前序遍历 统一采用递归遍历
        void preOrder(Node<T> *t);
        //中序遍历
        void inOrder(Node<T> *t);
        //后序遍历
        void postOrder(Node<T> *t);
};
using std::cout;
using std::endl;
        
template <class T>
BinaryTree<T>::BinaryTree(){
    root =NULL;
}

template<class T>
BinaryTree<T>::~BinaryTree(){
    clear();
}

template<class T>
void BinaryTree<T>::clear() {
    if(root==NULL){
        return;
    }    
    delete root;
    root = NULL;
}

template<class T>
T BinaryTree<T>::getElement() const {
    if (isEmpty()) {
        return NULL;
    }
    return root->value;
}
    
template<class T>
bool BinaryTree<T>::isEmpty() const {
    return root == NULL;
}
    
template<class T>
void BinaryTree<T>::makeTree(const T &x, BinaryTree<T> *left, BinaryTree<T> *right) {
    if (root)return;//如果根节点不为空 就直接return 这实际上让用户操作起来十分不方便 不能自己构建自己 必须重一个节点
    root = new Node<T>(x);//实例化二叉树根节点
    root->left = left->root;//让左指针指向左子树的根节点
    root->right = right->root;//让右指针指向右子树的根节点
    left->root = right->root = NULL;
}
    
template<class T>
void BinaryTree<T>::breakTree(T &x, BinaryTree*left, BinaryTree*right) {
    if (!root || left == right || left->_root || right->_root) {
        //若此树本身就是空的，或则用于承接的左右树本身一样，或则用于承接的左子树不为空都直接退出
        return;
    }
    x = root->value;//将这个树的根节点的元素域转移走
    left->root = root->left;//将左子树单独拆成一棵树
    right->root = root->right;//将右子树单独拆成一棵树
    clear();
}
    
template<class T>
void BinaryTree<T>::preOrder(Node<T> *t) {
    if (t) {
        cout << ("%d",t->value) << endl;
        preOrder(t->left);
        preOrder(t->right);
    }
}
        
template<class T>
void BinaryTree<T>::inOrder(Node<T> *t) {
    if (t) {
        inOrder(t->left);
        cout << ("%d",t->value) << endl;
        inOrder(t->right);
    }
}
template<class T>
void BinaryTree<T>::postOrder(Node<T> *t) {
    if (t) {
        postOrder(t->left);
        postOrder(t->right);
        cout << ("%d",t->value) << endl;
    }
}