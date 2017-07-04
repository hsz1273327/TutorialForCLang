#include <iostream>
#include <stddef.h>
#include"binary_tree.h"
using std::cout;
using std::endl;
        
int main(){
    BinaryTree<int> L,R,a,b,c,d,e;
    a.makeTree(0,&L,&R);
    b.makeTree(1,&L,&R);
    c.makeTree(2,&a,&b);
    d.makeTree(3,&a,&b);
    e.makeTree(4,&c,&d);
    cout<<"前序遍历："<<endl;
    e.preOrder(e.root);
    cout<<"\n中序遍历："<<endl;
    e.inOrder(e.root);
    cout<<"\n后序遍历："<<endl;
    e.postOrder(e.root);
    cout<<"\n"<<endl;
    return 0;
}