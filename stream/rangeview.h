#ifndef VIEW_H
#define VIEW_H

#include <functional>
#include <vector>
#include <algorithm>
#include <iostream>
#include <assert.h>

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
        using Param = typename std::remove_reference<decltype(range_view.get_data())>::type::value_type;
        using Result = std::result_of_t<Func(Param&&)>;
        auto compose_func = range_view.get_compose_func();
        auto functor = [=](auto data){
            std::vector<Result> new_vector;
            std::transform(std::begin(data), std::end(data), std::back_inserter(new_vector), func);
            return new_vector;
        };
        auto new_compose_func = [compose_func, functor](auto data) {
            return functor(compose_func(data));
        };
        return RangeView<Result, Param>(std::move(range_view.get_data()),
                                     std::move(new_compose_func),
                                     std::move(range_view.get_gen()),
                                     std::move(range_view.is_has_gen()));
    });
    return Function<decltype(lambda)>(lambda);
}

auto reverse() {
    auto lambda = [=] (auto&& range_view) {
        using Param = typename std::remove_reference<decltype(range_view.get_data())>::type::value_type;
        auto compose_func = range_view.get_compose_func();
        using Result = typename decltype(compose_func)::result_type::value_type;
        auto func = [=](auto data){
            std::reverse(data.begin(),data.end());
            return data;
        };
        auto new_compose_func = [compose_func, func](auto data) {
            return func(compose_func(data));
        };
        return RangeView<Result, Param>(std::move(range_view.get_data()),
                                     std::move(new_compose_func),
                                     std::move(range_view.get_gen()),
                                     std::move(range_view.is_has_gen()));

    };
    return Function<decltype(lambda)>(lambda);

}

template <typename Func>
inline auto remove_if(Func&& func) {
    auto lambda  = [=](auto&& range_view) {
        using Param = typename std::remove_reference<decltype(range_view.get_data())>::type::value_type;
        auto compose_func = range_view.get_compose_func();
        using Result = typename decltype(compose_func)::result_type::value_type;
        auto functor = [=](auto data){
            data.erase(std::remove_if(std::begin(data),std::end(data), func), std::end(data));
            return data;
        };
        auto new_compose_func = [compose_func, functor](auto data) {
            return functor(compose_func(data));
        };
        return RangeView<Result, Param>(std::move(range_view.get_data()),
                                     std::move(new_compose_func),
                                     std::move(range_view.get_gen()),
                                     std::move(range_view.is_has_gen()));

    };
    return Function<decltype(lambda)>(lambda);
}

inline auto take(unsigned int n) {
    auto lambda = [=] (auto&& range_view) {
        using Param = typename std::remove_reference<decltype(range_view.get_data())>::type::value_type;
        auto compose_func = range_view.get_compose_func();
        auto has_gen = range_view.is_has_gen();
        auto start_gen = range_view.get_gen();
        using Result = typename decltype(compose_func)::result_type::value_type;
        auto functor = [has_gen,n,start_gen](auto data){
            if(has_gen == false){
                if(data.size() > n){
                    data.resize(n);
                }
            }
            else if(has_gen == true){
//                if(data.size() > n){
//                    data.resize(n);
//                }else{
                    int j = start_gen;
                    for(std::size_t i = data.size(); i < n; i++){
                        data.push_back(j);
                        j++;
                    }
//                }
             }
            return data;
        };
        auto new_compose_func = [compose_func, functor](auto data) {
            return functor(compose_func(data));
        };

        return RangeView<Result, Param>(std::move(range_view.get_data()),
                                     std::move(new_compose_func),
                                     std::move(range_view.get_gen()),
                                     std::move(range_view.is_has_gen()));
    };
    return Function<decltype(lambda)>(lambda);
}

inline auto ints(int n) {
    auto lambda = [=](auto&& range_view) {
        assert(range_view.get_data().empty());
        using Param = typename std::remove_reference<decltype(range_view.get_data())>::type::value_type;
        auto compose_func = range_view.get_compose_func();
        using Result = typename decltype(compose_func)::result_type::value_type;
        return RangeView<Result, Param>(std::move(range_view.get_data()),
                                     std::move(range_view.get_compose_func()),
                                     n,
                                     true);
    };
    return Function<decltype(lambda)>(lambda);
}

template <class TView>
int accumulate(TView view){
    auto vec = view.make_compose_vector();
    return std::accumulate(std::begin(vec), std::end(vec),0);
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

    RangeView(std::vector<U>&& data, std::function<std::vector<T>(std::vector<U>)>&& compose_func, int gen, bool has_gen)
        : data_(std::move(data)), compose_func_(std::move(compose_func)), has_gen_(has_gen),gen_(gen)
    {}

    RangeView(const std::vector<U>& v) : data_(v), has_gen_(false) {    }

    std::vector<T> make_compose_vector() {
        return compose_func_(data_);
    }

public:

    std::vector<U>& get_data(){
        return data_;
    }
    bool& is_has_gen(){
        return has_gen_;
    }

    int& get_gen(){
        return gen_;
    }

    std::function<std::vector<T>(std::vector<U>)>& get_compose_func(){
        return compose_func_;
    }

    void set_start_gen(int gen){
        gen_ = gen;
    }

private:
    std::vector<U> data_ = std::vector<U>();
    std::function<std::vector<T>(std::vector<U>)> compose_func_ = [](std::vector<U> data){
        return data;
    };
    bool has_gen_ = false;
    int gen_;
};

}

#endif // VIEW_H
