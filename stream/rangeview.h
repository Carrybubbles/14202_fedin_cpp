#ifndef VIEW_H
#define VIEW_H

#include <functional>
#include <vector>

namespace rangeview {

template <typename T, typename U>
class RangeView {
public:

    std::vector<U> data_ = std::vector<U>();
    std::function<std::vector<T>(std::vector<U>)> compose_func_ = [](std::vector<U> data){
        return data;
    };
    bool has_gen_;
    int gen_;
private:

};

}

#endif // VIEW_H
