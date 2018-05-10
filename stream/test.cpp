#include <iostream>
#include "rangeview.h"
#include "deps/include/gtest/gtest.h"
#include "deps/include/gmock/gmock.h"
using namespace rangeview;

TEST(RangeView, rangeview){
    std::vector<int> v{2,4,6,8,10,12};
    std::vector<int> q{1,2,3,4,5,6,7,8,9,10,11,12};
    auto res_vec = (q | remove_if([](auto x){ return x % 2 == 1;})).make_compose_vector();
    ASSERT_THAT(res_vec,::testing::ElementsAreArray(v));
}

TEST(RangeView, rangeview1){
    std::vector<int> vi{1,2,3,4,5,6,7,8,9,10};
    std::vector<std::string> q{"2","4","6","8","10"};
    auto res_vec = (vi
    | remove_if([](int i) {return i % 2 == 1;})
    | transform([](int i) {return std::to_string(i);})).make_compose_vector();
}

TEST(RangeView, rangeview2){
    std::vector<int> vi{1,2,3,4,5,6,7,8,9,10};
    std::vector<int> v{4,8,12,16,20};
    auto res_vec = (vi
    | remove_if([](int i) {return i % 2 == 1;})
    | transform([](auto x){ return x * 2;})).make_compose_vector();
    ASSERT_THAT(res_vec,::testing::ElementsAreArray(v));

}

TEST(RangeView, rangeview3){
    std::vector<int> vi{1,2,3,4,5,6,7,8,9,10};
    int res_vec = accumulate(ints(1) | take(20)
    | transform([](int i){return i*i;}));
    ASSERT_THAT(res_vec, 2870);

}

TEST(RangeView, rangeview4){
    std::vector<int> vi{1,2,3,4,5,6,7,8,9,10};
    std::vector<double> q{1.5,3,4.5,6,7.5,9,10.5,12,13.5,15};
    auto res_vec = (vi | transform([](auto x){return static_cast<float>(x) * 1.5;})).make_compose_vector();
    ASSERT_THAT(res_vec, ::testing::ElementsAreArray(q));
}

TEST(RangeView, rangeview5){
    std::vector<int> q{20, 22, 24, 26, 28, 10, 11, 12, 13, 14, 10, 11, 12, 13, 14 };
    auto res_vec = (ints(10)| take(5)| transform([](auto x){ return x * 2;}) | take(10) | take(15)).make_compose_vector();
    ASSERT_THAT(res_vec, ::testing::ElementsAreArray(q));
}

TEST(RangeView, rangeview6){
    std::vector<int> q{11, 13, 10, 11, 12};
    auto res_vec = (ints(10)| take(5)| transform([](auto x){ return x * 2;}) | take(10) | remove_if([](auto x){return x % 2 == 0;}) | take(5)).make_compose_vector();
    ASSERT_THAT(res_vec, ::testing::ElementsAreArray(q));
}

TEST(RangeView, rangeview7){
    std::vector<int> vi{1,2,3,4,5,6,7,8,9,10};
    std::vector<double> q{1.5,3,4.5,6,7.5,9,10.5,12,13.5,15};
    std::reverse(std::begin(q), std::end(q));
    auto res_vec = (vi | transform([](auto x){return static_cast<float>(x) * 1.5;}) | reverse()).make_compose_vector();
    ASSERT_THAT(res_vec, ::testing::ElementsAreArray(q));
}

TEST(RangeView, rangeview8){
    std::vector<int> v{2};
    std::vector<int> q{1,2,3,4,5,6,7,8,9,10,11,12};
    auto res_vec = (q | take(2) | remove_if([](int i) {return i % 2 == 1;})).make_compose_vector();
    ASSERT_THAT(res_vec,::testing::ElementsAreArray(v));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}
