#include <iostream>
#include "rangeview.h"
#include "noisy.h"
#include <memory>
using namespace std;
using namespace rangeview;
template<typename U>
auto kek = [](U data){
        return std::optional<U>(std::move(data));
    };

int main()
{
    std::vector<std::unique_ptr<int>> list;

    for (int i = 1; i < 11; ++i)
    {
        list.emplace_back(std::make_unique<int>(i));
    }

//    std::unique_ptr<int> k(new int(5));
//    using REsult =
//    auto temp = foo(*kek<std::unique_ptr<int>>(std::move(k)));
//    std::optional<std::unique_ptr<int>> q(std::move(temp));
//    std::cout << *k;
    //std::optional<std::unique_ptr<int>> opt(k);

    std::vector<Noisy> q{1,2,3,4,5,6,7,8,9,10,11,12};
    Noisy::noisy = 0;
    auto res_vec = (q |
                    remove_if([](auto&& i) {return i.value % 2 == 1;}) |
                    transform([](auto&& i) {return 2*i.value;}) |
                    reverse()
                    | take(20)
                    );
//    std::cout << res_vec.size();
//    for(auto a: res_vec){
//        std::cout << a << std::endl;
//    }

    return 0;
}
