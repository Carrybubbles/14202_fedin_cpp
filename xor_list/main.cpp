#include <iostream>
#include "xorlist.h"
using namespace std;

int main()
{
    LinkedList<int>* list = new LinkedList<int>();
        Node<int>* node = new Node<int>(3);
        Node<int>* node1 = new Node<int>(3);
        Node<int>* node2 = new Node<int>(3);
    list->insert_after(list->tail_,node);
    list->insert_after(list->tail_,node1);
    list->insert_after(list->tail_,node2);
    list->print();




}
