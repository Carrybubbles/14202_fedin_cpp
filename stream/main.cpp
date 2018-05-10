#include <iostream>
#include "rangeview.h"
using namespace std;
using namespace rangeview;
int main()
{
    std::vector<int> q{15,17,19,13,14};
    auto res_vec = (ints(1)
                    | take(12)
                    | transform([](auto i) {return i * 2;}) | remove_if([](auto i) {return i % 2 == 0;}) | reverse()  | take(10)).make_compose_vector();
    return 0;
}
