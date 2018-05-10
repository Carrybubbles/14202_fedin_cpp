#ifndef VIEW_H
#define VIEW_H

#include <functional>
#include <vector>
#include <iostream>

namespace rangeview {

template <typename T, typename U>
class RangeView;

template <typename Func>
class Function {
public:
    Function(Func f)
        : fn(f)
    {}

    Function(const Function& other)
        : fn(other.fn)
    {}

    template<typename T, typename U>
    std::result_of_t<Func(RangeView<T, U>&&)> apply(RangeView<T, U>&& range_view) {
        return fn(std::forward<RangeView<T, U>>(range_view));
    }

private:
    Func fn;
};

template <typename T, typename U>
class RangeView {
public:
    template <typename Func>
    auto operator| (Function<Func> func) {
        return func.apply(std::move(*this));
    }
public:
    template<typename Func>
    friend class Function;
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
