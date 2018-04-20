#ifndef XOR_LIST_TEST_H
#define XOR_LIST_TEST_H
#include "xor_list.h"
#include "deps/include/gtest/gtest.h"
#include "deps/include/gmock/gmock.h"

#include "memory"

namespace test {
using namespace xlist;

TEST(AddingTest, AddToTail){
    LinkedList<int> list;
    for(int i = 0 ; i < 100500; i++){
        list.push_back(i);
        ASSERT_EQ(i, list.back());
        ASSERT_EQ(i+1 , list.size());
    }


}

TEST(AddingTest, AddToHead){
    LinkedList<int> list;
    for(int i = 0 ; i < 100500; i++){
        list.push_front(i);
        ASSERT_EQ(i, list.front());
        ASSERT_EQ(i+1 , list.size());
    }
    LinkedList<int> list2;
    for(int i = 0 ; i < 100500; i++){
        int k = i;
        list2.push_front(std::move(k));
        ASSERT_EQ(i, list2.front());
        ASSERT_EQ(i+1 , list2.size());
    }
}

TEST(AddingTest, AddToItPos){
    LinkedList<int> list;
    list.insert(list.cbegin(), 0);
    //between pos
    auto it = list.cend();
    for(int i = 1; i < 100500; i++){
        list.insert(it, i);
    }
    int i = 0;
    for(auto it = list.cbegin(); it != list.cend(); it++){
        ASSERT_EQ(i++, *it);
    }

}

TEST(AddingTest, AddManuValuesToItPos){
    LinkedList<int> list;
    std::vector<int> v(5);
    std::generate(v.begin(), v.end(), std::rand);
    list.insert(list.cbegin(), v.begin(), v.end());
    ASSERT_THAT(list,::testing::ElementsAreArray(v));

}

TEST(RemoveTest, PopBackAndPopFront){
    LinkedList<int> list(500);
    for(int i = 500; i > 0; i-=2){
        list.pop_back();
        list.pop_front();
        ASSERT_EQ(i-2,list.size());
    }
    list.push_back(2);
    list.pop_back();
    ASSERT_EQ(0,list.size());
    for(int i = 0; i < 500; i++){
        list.pop_back();
        list.pop_front();
        ASSERT_EQ(0, list.size());
    }

}

TEST(SortTest, SortTest){
    LinkedList<int> list(500);
    std::vector<int> v(500);
    int i = 0;
    int j = 499;
    std::generate(list.begin(), list.end(), [&](){return j--;});
    list.sort();
    std::generate(v.begin(), v.end(), [&](){ return i++;});
    ASSERT_THAT(list,::testing::ElementsAreArray(v));

}

TEST(SortTest, UniqPtrSort){
    LinkedList<std::unique_ptr<int>> list;
    std::vector<std::unique_ptr<int>> v;
    for(int i = 0; i < 500; i++){
        list.push_back(std::unique_ptr<int> (new int(i)));
    }
    for(int i = 499; i >= 0; i--){
        v.push_back(std::unique_ptr<int> (new int(i)));
    }
    list.sort([](const std::unique_ptr<int>& a, const std::unique_ptr<int>& b){return *a < *b;});
    auto it2 = v.begin();
    for(auto it = list.begin() ; it != list.end(); ++it){
        ASSERT_EQ(*it->get(), *it2->get());
        ++it2;
    }
}

TEST(ReverseTest, Reverse){
    LinkedList<int> list(500);
    std::vector<int> v(500);
    int i = 0;
    int j = 499;
    std::generate(list.begin(), list.end(), [&](){return j--;});
    list.reverse();
    std::generate(v.begin(), v.end(), [&](){ return i++;});
    ASSERT_THAT(list,::testing::ElementsAreArray(v));
}

TEST(ConstructorsTest, Copy){
    LinkedList<int> list(500);
    int i = 0;
    std::generate(list.begin(), list.end(), [&](){ return i++;});
    LinkedList<int> list2(list);
    ASSERT_THAT(list,::testing::ElementsAreArray(list2));
}

TEST(OperatorsTest, OperatorRavno){
    LinkedList<int> list(500);
    int i = 0;
    std::generate(list.begin(), list.end(), [&](){ return i++;});
    LinkedList<int> list2;
    list2 = list;
    std::vector<int> v(500);
    i = 0;
    std::generate(v.begin(), v.end(), [&](){ return i++;});
    ASSERT_THAT(list2,::testing::ElementsAreArray(v));

}

TEST(ConstructorsTest, MoveCopyConstructors){
    LinkedList<int> list(500);
    int i = 0;
    std::generate(list.begin(), list.end(), [&](){ return i++;});
    LinkedList<int> list2(std::move(list));
    std::vector<int> v(500);
    i = 0;
    std::generate(v.begin(), v.end(), [&](){ return i++;});
    ASSERT_THAT(list2,::testing::ElementsAreArray(v));
}

TEST(OperatorsTEST, MoveOperatorRavno){
    LinkedList<int> list(500);
    int i = 0;
    std::generate(list.begin(), list.end(), [&](){ return i++;});
    LinkedList<int> list2;
    list2 = std::move(list);
    std::vector<int> v(500);
    i = 0;
    std::generate(v.begin(), v.end(), [&](){ return i++;});
    ASSERT_THAT(list2,::testing::ElementsAreArray(v));
}

TEST(SwapTest, Swap){
    LinkedList<int> list(500);
    int i = 0;
    std::generate(list.begin(), list.end(), [&](){ return i++;});
    LinkedList<int> list2(500);
    i = 0;
    std::generate(list2.begin(), list2.end(), [&](){ return i--;});

    list.swap(list2);

    std::vector<int> v(500);
    i = 0;
    std::generate(v.begin(), v.end(), [&](){ return i++;});
    std::vector<int> v2(500);
    i = 0;
    std::generate(v2.begin(), v2.end(), [&](){ return i--;});

    ASSERT_THAT(list2,::testing::ElementsAreArray(v));
    ASSERT_THAT(list,::testing::ElementsAreArray(v2));

}

TEST(ResizeTest, resizeGreate){
    LinkedList<int> list(500);
    int i = 0;
    std::generate(list.begin(), list.end(), [&](){ return i++;});
    list.resize(1000);
    ASSERT_EQ(1000,list.size());
    list.resize(850);
    ASSERT_EQ(850,list.size());
}

TEST(AssignTest, assingnList){
    LinkedList<int> list(500);
    int i = 0;
    std::generate(list.begin(), list.end(), [&](){ return i++;});
    list.assign({1,2,3,4,5,6});
    ASSERT_THAT(list,::testing::ElementsAre(1,2,3,4,5,6));

}

TEST(AssignTest, assignValues){
    LinkedList<int> list(500);
    int i = 0;
    std::generate(list.begin(), list.end(), [&](){ return i++;});
    std::vector<int> v(250,6);
    list.assign(250,6);
    ASSERT_THAT(list,::testing::ElementsAreArray(v));
}

TEST(UniqueTest, unique){
    LinkedList<int> list({1,1,2,2,3,3,3,4,5,5,5,5,6,6,6,7,7,8,9});
    std::vector<int> v({1,2,3,4,5,6,7,8,9});
    list.unique();
    ASSERT_THAT(list,::testing::ElementsAreArray(v));
    LinkedList<int> list2({1});
    list2.unique();
    ASSERT_THAT(list2,::testing::ElementsAreArray({1}));
    LinkedList<int> list3;
    list3.unique();
    ASSERT_THAT(0,list3.size());

}

TEST(SpliceTest, splice){
    LinkedList<int> list(500);
    LinkedList<int> list2(500);
    LinkedList<int> list3(500);
    LinkedList<int> list4(500);
    int i = 0;
    std::generate(list.begin(), list.end(), [&](){ return i++;});
    i = 0;
    std::generate(list3.begin(), list3.end(), [&](){ return i++;});
    std::fill(list.begin(), list.end(), -1);
    list.splice(list.cbegin(),list2);
    list.splice(list.cend(),list3,list3.cbegin(), list3.cend());
    list.splice(list.cbegin(),list4,list4.cbegin());
    ASSERT_EQ(1501,list.size());
}

TEST(MergeTest, merger){
    LinkedList<int> list(500);
    int i = 0;
    LinkedList<int> list2(500);
    std::vector<int> v(1000);
    int j = 2;
    std::generate(v.begin(),v.end(), [&](){return j++;});
    std::generate(list.begin(), list.end(), [&](){ return i += 2; });
    i = 1;
    std::generate(list2.begin(), list2.end(), [&](){ return i += 2;});

    list.merge(list);
    ASSERT_EQ(500,list.size());
    list.merge(list2);
    ASSERT_EQ(1000,list.size());
    ASSERT_THAT(list,::testing::ElementsAreArray(v));
}

TEST(MergeTest, UniqPtrmerge){
    LinkedList<std::unique_ptr<int>> list;
    LinkedList<std::unique_ptr<int>> list2;
    std::vector<std::unique_ptr<int>> v(1000);
    int j = 2;
    std::generate(v.begin(),v.end(), [&](){return std::unique_ptr<int>(new int(j++));});
    int i = 0;
    std::generate(list.begin(), list.end(), [&](){ return std::unique_ptr<int>(new int(i += 2)); });
    i = 1;
    std::generate(list2.begin(), list2.end(), [&](){ return std::unique_ptr<int>(new int(i += 2));});

    list.merge(list2);
}

TEST(PushTest, EmplaceBack){
    LinkedList<int> list;
    for(int i = 0; i < 500; i++){
        list.emplace_back(i);
    }
    for(int i = -1; i >= -500; i--){
        list.emplace_front(i);
    }
    int i = -500;
    std::vector<int> v(1000);
    std::generate(v.begin(), v.end(), [&](){ return i++;});
    ASSERT_THAT(list,::testing::ElementsAreArray(v));
    ASSERT_EQ(1000,list.size());
    ASSERT_EQ(-500, list.front());
    ASSERT_EQ(499, list.back());
}

TEST(EraseTest, erase){
    LinkedList<int> list(500);
    int i = 0;
    std::generate(list.begin(), list.end(), [&](){ return i++;});
    list.erase(list.cend());
}

TEST(IteratorTest, Movement){
    LinkedList<int> list(500);
    int i = 0;
    std::generate(list.begin(), list.end(), [&](){ return i++;});
    i = 499;
    for(auto it = --list.end(); it != list.begin(); --it){
        ASSERT_THAT(i--, *it);
    }
}

}//test

#endif // XOR_LIST_TEST_H
