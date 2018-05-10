#ifndef VIEW_H
#define VIEW_H

#include <functional>
#include <vector>
#include <algorithm>
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

template <typename Func>
auto transform(Func&& func) {
    auto lambda = ([=](auto&& range_view) {
        using Param = typename decltype(range_view.data)::value_type;
        using Result = std::result_of_t<Func(Param&&)>;
        auto compose_func = range_view.compose_func;
        auto functor = [=](auto data){
            std::vector<Result> new_vector;
            for(int i = 0; i < data.size(); i++){
                new_vector.push_back(func(data[i]));
            }
            return new_vector;
        };
        auto new_compose_func = [compose_func, functor](auto data) {
            return functor(compose_func(data));
        };
        return RangeView<Result, Param>(std::move(range_view.data_),
                                     std::move(new_compose_func),
                                     range_view.gen_,
                                     range_view.has_gen_);
    });
    return Function<decltype(lambda)>(lambda);
}


auto reverse() {
    auto lambda = [=] (auto&& range_view) {
        using Param = typename decltype(range_view.data)::value_type;
        auto compose_func = range_view.compose_func;
        using Result = typename decltype(compose_func)::result_type::value_type;
        auto func = [=](auto data){
            std::reverse(data.begin(),data.end());
            return data;
        };
        auto new_compose_func = [compose_func, func](auto data) {
            return func(compose_func(data));
        };
        return RangeView<Result, Param>(std::move(range_view.data),
                                     std::move(new_compose_func),
                                     std::move(range_view.gen),
                                     std::move(range_view.has_gen));
    };
    return Function<decltype(lambda)>(lambda);

}

template <typename U, typename Func>
auto operator | (std::vector<U> vector, Function<Func> func) {
    RangeView<U,U> temp(vector);
    return func.apply(std::move(temp));
}

template <typename Func1, typename Func2>
auto operator | (Function<Func1> init_func, Function<Func2> func) {
    return func.apply(init_func.apply(std::move(RangeView<int,int>())));
}

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
    template <typename Func>
    friend auto operator | (std::vector<U> vector, Function<Func> func);
    template <typename Func1, typename Func2>
    friend auto operator | (Function<Func1> init_func, Function<Func2> func);

public:
    RangeView(){}

    RangeView(std::vector<U>&& data, std::function<std::vector<T>(std::vector<U>)>&& compose_func, int&& gen, bool&& has_gen)
        : data_(std::move(data)), compose_func_(std::move(compose_func)), has_gen_(std::move(has_gen)),gen_(std::move(gen))
    {}

    RangeView(std::vector<U> v) {
        std::for_each(std::begin(v), std::end(v),[this](auto val){
            data_.push_back(val);
        });
    }

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
