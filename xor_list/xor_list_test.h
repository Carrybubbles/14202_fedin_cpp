#ifndef XOR_LIST_TEST_H
#define XOR_LIST_TEST_H
#include "xor_list.h"

namespace test {
using namespace xlist;

TEST(AddingTest, AddToTail){
    LinkedList<int,std::allocator<Node<int>>> list;
    list.push_back(1);
    list.push_back(1);
    list.push_back(1);
    list.push_back(23);
    EXPECT_EQ(23, list.back());

}

TEST(AddingTest, AddToHead){
    LinkedList<int,std::allocator<Node<int>>> list;
    list.push_back(1);
    list.push_back(1);
    list.push_back(1);
    list.push_front(23);
    EXPECT_EQ(23, list.front());
}

TEST(AddingTest, AddToItPos){
    LinkedList<int,std::allocator<Node<int>>> list;
    list.push_back(1);
    list.push_back(1);
    list.push_back(1);
    //head
    list.insert(list.cbegin(), 50);
    EXPECT_EQ(50, list.front());
    //tail
    list.insert(list.cbegin(), 51);
    EXPECT_EQ(51, list.front());
    //between pos
    list.insert(++(list.cbegin()), 52);
    auto it = list.cbegin();
    for(int i = 0; i < 1; i++, ++it);
    EXPECT_EQ(52, *it);

}

TEST(AddingTest, AddManuValuesToItPos){
    LinkedList<int,std::allocator<Node<int>>> list;
    list.push_back(1);
    list.push_back(1);
    list.push_back(1);

    std::vector<int> myvector (3,30);
    list.insert(++list.cbegin(), myvector.begin(), myvector.end());
    auto it = ++list.cbegin();
    for(std::size_t i = 3; i > 0 && it != list.cend(); --i, ++it){
        EXPECT_EQ(30, *it);
    }

}

TEST(RemoveTest, PopBack){
    LinkedList<int,std::allocator<Node<int>>> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    list.pop_back();
    EXPECT_EQ(2, list.size());
    EXPECT_EQ(2, list.back());
}

TEST(RemoveTest, PopFront){
    LinkedList<int,std::allocator<Node<int>>> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    list.pop_front();
    EXPECT_EQ(2, list.size());
    EXPECT_EQ(2, list.front());
}


}//test

#endif // XOR_LIST_TEST_H
