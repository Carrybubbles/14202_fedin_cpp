#include <iostream>
#include <string>

#include "rangeview.h"
#include "noisy.h"
#include "deps/include/gtest/gtest.h"
#include "deps/include/gmock/gmock.h"
using namespace rangeview;

TEST(RangeView, rangeview0){
    std::vector<int> v{2,4,6,8,10,12};
    std::vector<int> q{1,2,3,4,5,6,7,8,9,10,11,12};
    auto res_vec = (q | remove_if([](auto x){ return x % 2 == 1;})).to_vector();
    ASSERT_THAT(res_vec,v);
}

TEST(RangeView, rangeview1){
    std::vector<int> vi{1,2,3,4,5,6,7,8,9,10};
    std::vector<std::string> q{"2","4","6","8","10"};
    auto res_vec = (vi
    | remove_if([](int i) {return i % 2 == 1;})
    | transform([](int i) {return std::to_string(i);})).to_vector();
    std::string result = "";
    std::string expected = "";

    std::for_each(std::begin(res_vec), std::end(res_vec),[&](auto val){
        result +=val;
    });
    std::for_each(std::begin(q), std::end(q),[&](auto val){
        expected +=val;
    });
    ASSERT_EQ(result,expected);
}

TEST(RangeView, rangeview2){
    std::vector<int> vi{1,2,3,4,5,6,7,8,9,10};
    std::vector<int> v{4,8,12,16,20};
    auto res_vec = (vi
    | remove_if([](int i) {return i % 2 == 1;})
    | transform([](auto x){ return x * 2;})).to_vector();
    ASSERT_THAT(res_vec,v);

}

TEST(RangeView, rangeview3){
    int res_vec = accumulate(ints(1)
    | transform([](int i){return i*i;}) | take(10));
    ASSERT_THAT(res_vec, 385);
}

TEST(RangeView, rangeview4){
    std::vector<int> vi{1,2,3,4,5,6,7,8,9,10};
    std::vector<double> q{1.5,3,4.5,6,7.5,9,10.5,12,13.5,15};
    auto res_vec = (vi | transform([](auto x){return static_cast<float>(x) * 1.5;})).to_vector();
    ASSERT_THAT(res_vec, q);
}


TEST(RangeView, rangeview7){
    std::vector<int> vi{1,2,3,4,5,6,7,8,9,10};
    std::vector<double> q{1.5,3,4.5,6,7.5,9,10.5,12,13.5,15};
    std::reverse(std::begin(q), std::end(q));
    auto res_vec = (vi | transform([](auto x){return static_cast<float>(x) * 1.5;}) | reverse()).to_vector();
    ASSERT_THAT(res_vec, q);
}

TEST(RangeView, rangeview8){
    std::vector<int> v{2};
    std::vector<int> q{1,2,3,4,5,6,7,8,9,10,11,12};
    auto res_vec = (q | take(2) | remove_if([](auto i) {return i % 2 == 1;})).to_vector();
    ASSERT_THAT(res_vec,v);
}

TEST(RangeView, rangeview9){
    std::vector<int> v;
    std::vector<int> q{1,2,3,4,5,6,7,8,9,10,11,12};
    auto res_vec = (q | remove_if([](auto i) {return i % 2 == 1;}) | remove_if([](auto i) {return i % 2 == 0;}) | take(10)).to_vector();
    ASSERT_THAT(res_vec,v);
}

TEST(RangeView, rangeview10){
    std::vector<int> v;
    std::vector<int> q{1,2,3,4,5,6,7,8,9,10,11,12};
    auto res_vec = (q | remove_if([](auto i) {return i % 2 == 1;}) | remove_if([](int i) {return i % 2 == 0;}) | reverse()  | take(10)).to_vector();
    ASSERT_THAT(res_vec,v);
}

TEST(RangeView, rangeview11){
    std::vector<int> v;
    std::vector<int> q{1,2,3,4,5,6,7,8,9,10,11,12};
    auto res_vec = (q | transform([](auto i) {return i * 2;}) | remove_if([](auto i) {return i % 2 == 0;}) | reverse()  | take(10)).to_vector();
    ASSERT_THAT(res_vec,v);
}

TEST(RangeView, rangeview12){
    std::vector<std::unique_ptr<int>> list;
    for (int i = 1; i < 11; ++i)
    {
        list.emplace_back(std::make_unique<int>(i));
    }

    auto res_vec = (list | transform([](auto  v){ *v = *v * *v; return std::move(v); }) | remove_if([](auto&& v){ return *v % 2 == 0; }) |take(3)).to_vector();
    ASSERT_THAT(res_vec,testing::ElementsAre(testing::Pointee(1),
                                             testing::Pointee(9),
                                             testing::Pointee(25)));
}

TEST(RangeView, rangeview13){
    std::vector<std::unique_ptr<int>> v;
    for (int i = 1; i < 11; ++i)
    {
        v.emplace_back(std::make_unique<int>(i));
    }

    auto res_vec = (v | transform([](auto&&  v){ *v = *v * *v; return std::move(v); }) | remove_if([](auto&& v){ return *v % 2 == 0; }) | transform([](auto&&  v){ *v = *v * 2; return std::move(v); }) | reverse() | take(3)).to_vector();
    ASSERT_THAT(res_vec,testing::ElementsAre(testing::Pointee(162),
                                             testing::Pointee(98),
                                             testing::Pointee(50)));
}

TEST(RangeView, rangeview14){
    std::vector<std::unique_ptr<int>> v;
    for (int i = 1; i < 11; ++i)
    {
        v.emplace_back(std::make_unique<int>(i));
    }

    auto res_vec = (v | transform([](auto&&  v){ *v = *v * *v; return std::move(v); })
                    | remove_if([](auto&& v){ return *v % 2 == 0; })
                    | transform([](auto&&  v){ *v = *v * 2; return std::move(v); })
                    | reverse()
                    | take(3) |  transform([](auto&&  v){ *v = *v * 3; return std::move(v); })
                    | take(6)).to_vector();
    ASSERT_THAT(res_vec,testing::ElementsAre(testing::Pointee(486),
                                             testing::Pointee(294),
                                             testing::Pointee(150)));
}

TEST(RangeView, rangeview15){
    auto res_vec = accumulate((ints(1) | transform([](int i) {return i*i;})| take(10)));
    ASSERT_EQ(res_vec,385);
}

TEST(RangeView, rangeview16){
    std::vector<int> q{4,16,36,64,100};
    auto res_vec = (ints(1) | transform([](int i) {return i*i;}) | remove_if([](auto i){return i % 2 == 1;})| take(10)).to_vector();
    ASSERT_THAT(res_vec,q);
}

TEST(RangeView, rangeview17){
    std::vector<int> q{2,8,18};
    Noisy::noisy = 0;
    auto res_vec = (ints(1) | transform([](int i) {return i*i;}) | remove_if([](auto i){return i % 2 == 1;})| take(10)
                    | transform([](auto i) {return i/2;}) | take(3) ).to_vector();
    ASSERT_THAT(Noisy::noisy,0);
}

TEST(RangeView, rangeview18){
    std::vector<int> q{2,8,18};
    Noisy::noisy = 0;
    auto res_vec = (ints(1) | transform([](int i) {return i*i;}) | remove_if([](auto i){return i % 2 == 1;})| take(10)
                    | transform([](auto i) {return i/2;}) | take(3) ).to_vector();
    ASSERT_THAT(Noisy::noisy,0);
}

TEST(RangeView, rangeview19){
    std::vector<Noisy> q{1,2,3,4,5,6,7,8,9,10,11,12};
    Noisy::noisy = 0;
    auto res_vec = (q |
                    remove_if([](auto&& i) {return i.value % 2 == 1;}) |
                    transform([](auto&& i) {return 2*i.value;}) |
                    reverse()
                    | take(20)
                    );
    //std::reverse() (((
    ASSERT_EQ(Noisy::noisy, 12);
}

TEST(RangeView, rangeview20){
    std::vector<int> q{1,2,3,4,5,6,7,8,9,10,11,12};
    std::vector<int> v{12,11,10,9,8,7,6,5,4,3,2,1};

    auto res_vec = (q |
                    remove_if([](auto i) {return false;}) |
                    transform([](auto i) {return i;}) |
                    reverse()
                    | take(20)
                    ).to_vector();
     ASSERT_THAT(res_vec,v);
}

TEST(RangeView, rangeview21){
    std::vector<int> q{1,2,3,4,5,6,7,8,9,10,11,12};
    std::vector<int> v;

    auto res_vec = (q |
                    remove_if([](auto i) {return true;}) |
                    transform([](auto i) {return i;}) |
                    reverse()
                    | take(20)
                    ).to_vector();
     ASSERT_THAT(res_vec,v);
}

TEST(RangeView, rangeview22){
    std::vector<double> q{1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.};
    std::vector<int> v{144};

    auto res_vec = (q |
                    transform([](auto i) {return i*i;}) |
                    transform([](auto i) {return static_cast<int>(i);}) |
                    reverse()
                    | take(1)
                    ).to_vector();
     ASSERT_THAT(res_vec,v);
}


TEST(RangeView, rangeview23){
    std::vector<int> v{1,4,9,16,25,36,49,64,81,100};
    auto res_vec = (ints(-10) |
                    transform([](auto i) {return i*i;}) |
                    reverse()
                    | take(10)
                    ).to_vector();
     ASSERT_THAT(res_vec,v);
}

TEST(RangeView, rangeview24){
    std::vector<std::unique_ptr<int>> v;
    for (int i = 1; i < 11; ++i)
    {
        v.emplace_back(std::make_unique<int>(i));
    }

    auto res_vec = (v | transform([](auto&&  v){ *v = *v * *v; return std::move(v); })
                    | remove_if([](auto&& v){ return false; })
                    | reverse()
                    | transform([](auto&&  v){ *v = *v * 3; return std::move(v); })
                    | take(4)).to_vector();
    ASSERT_THAT(res_vec,testing::ElementsAre(testing::Pointee(300),
                                             testing::Pointee(243),
                                             testing::Pointee(192),
                                             testing::Pointee(147)));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}
