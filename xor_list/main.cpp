#include <iostream>

#include <cstddef>
#include <memory>
#include <initializer_list>
#include <cstdint>
#include <list>
#include <vector>
#include "xor_list.h"

using namespace std;
using namespace xlist;
int main()
{
    LinkedList<int>* list = new LinkedList<int>();
    LinkedList<int>* list2 = new LinkedList<int>();

    std::vector<int> vec = {1,2,3,4};

    list->push_back(1);
    list->push_back(8);
    list->push_back(4);
    list->push_back(10);
    list->push_back(5);
    list->push_back(9);

    list2->push_back(0);
    list->sort();
    //list2->sort();
    for(auto it = list->cbegin(); it != list->cend(); ++it){
        std::cout << *it << " ";
    }





}
