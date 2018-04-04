#ifndef XOR_LIST_TEST_H
#define XOR_LIST_TEST_H
#include "xor_list.h"
#include "deps/include/gtest/gtest.h"
#include "deps/include/gmock/gmock.h"

namespace test {
using namespace xlist;

TEST(AddingTest, AddToTail){
    LinkedList<int,std::allocator<Node<int>>> list;
    for(int i = 0 ; i < 100500; i++){
        list.push_back(i);
        EXPECT_EQ(i, list.back());
        EXPECT_EQ(i+1 , list.size());
    }

}

TEST(AddingTest, AddToHead){
    LinkedList<int,std::allocator<Node<int>>> list;
    for(int i = 0 ; i < 100500; i++){
        list.push_front(i);
        EXPECT_EQ(i, list.front());
        EXPECT_EQ(i+1 , list.size());
    }
}

TEST(AddingTest, AddToItPos){
    LinkedList<int,std::allocator<Node<int>>> list;
    list.insert(list.cbegin(), 0);
    //between pos
    auto it = list.cend();
    for(int i = 1; i < 100500; i++){
        list.insert(it, i);
    }
    int i = 0;
    for(auto it = list.cbegin(); it != list.cend(); it++){
        EXPECT_EQ(i++, *it);
    }

}

TEST(AddingTest, AddManuValuesToItPos){
    LinkedList<int,std::allocator<Node<int>>> list;
    std::vector<int> v(5);
    std::generate(v.begin(), v.end(), std::rand);
    list.insert(list.cbegin(), v.begin(), v.end());
    EXPECT_THAT(list,::testing::ElementsAreArray(v));

}

TEST(RemoveTest, PopBackAndPopFront){
    LinkedList<int,std::allocator<Node<int>>> list(100500);
    for(int i = 100500; i > 0; i-=2){
        list.pop_back();
        list.pop_front();
        EXPECT_EQ(i-2,list.size());
    }
}

TEST(SortTest, SortTest){
    LinkedList<int,std::allocator<Node<int>>> list(500);
    std::vector<int> v(500);
    int i = 0;
    int j = 499;
    std::generate(list.begin(), list.end(), [&](){return j--;});
    list.sort();
    std::generate(v.begin(), v.end(), [&](){ return i++;});
    EXPECT_THAT(list,::testing::ElementsAreArray(v));

}

TEST(ReverseTest, Reverse){
    LinkedList<int,std::allocator<Node<int>>> list(500);
    std::vector<int> v(500);
    int i = 0;
    int j = 499;
    std::generate(list.begin(), list.end(), [&](){return j--;});
    list.reverse();
    std::generate(v.begin(), v.end(), [&](){ return i++;});
    EXPECT_THAT(list,::testing::ElementsAreArray(v));
}


}//test

#endif // XOR_LIST_TEST_H
