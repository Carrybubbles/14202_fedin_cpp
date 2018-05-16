#ifndef VIEW_H
#define VIEW_H

#include <functional>
#include <vector>
#include <algorithm>
#include <iostream>
#include <assert.h>
#include <numeric>
#include <optional>

namespace rangeview {

template <typename T, typename U>
class RangeView;

template <typename Func>
class Function {
public:
    Function(Func f)
        : fn(f)
    {}
    Function(Func&& f)
        : fn(std::move(f))
    {}

    template<typename T, typename U>
    std::result_of_t<Func(RangeView<T, U>&&)> apply(RangeView<T, U>&& range_view) {
        return fn(std::forward<RangeView<T, U>>(range_view));
    }

private:
    Func fn;
};

auto reverse() {
    auto lambda = [=] (auto&& range_view) {
        using Argument = typename decltype(range_view.data_)::value_type;
        auto compose_func = range_view.compose_func_;
        using Result = typename decltype(compose_func)::result_type::value_type;
        auto&& temp =  RangeView<Result, Argument>(std::move(range_view.data_),
                                     range_view.compose_func_,
                                     range_view.gen_,
                                     range_view.has_gen_,
                                     !range_view.is_reverse_);
        return std::move(temp);

    };
    return Function<decltype(lambda)>(lambda);
}

template <typename Func>
inline auto transform(Func&& func) {
    auto lambda = ([=](auto&& range_view) {
        using Argument = typename decltype(range_view.data_)::value_type;
        using Result = typename std::result_of_t<Func(Argument&&)>;
        auto compose_func = range_view.compose_func_;

        auto new_compose_func = [=](std::optional<Argument> data) -> std::optional<Result>{
            auto val = compose_func(std::move(data));
            if(val.has_value()){
               return std::optional<Result>(func(*std::move(val)));
            }else{
                return std::optional<Result>();
            }
        };
        auto&& temp =  RangeView<Result, Argument>(std::move(range_view.data_),
                                        new_compose_func,
                                        range_view.gen_,
                                        range_view.has_gen_,
                                        range_view.is_reverse_);
        return std::move(temp);

    });
    return Function<decltype(lambda)>(lambda);
}


template <typename Func>
inline auto remove_if(Func&& func) {
    auto lambda  = [=](auto&& range_view) {
        using Argument = typename decltype(range_view.data_)::value_type;
        auto compose_func = range_view.compose_func_;
        using Result = typename decltype(compose_func)::result_type::value_type;
        auto functor = [=](std::optional<Argument> data) -> std::optional<Result>{
            if(data.has_value()){
               if(!func(*std::move(data))){
                   return std::optional<Result>(*std::move(data));
                }else{
                    return std::optional<Result>();
                }
            }else{
                return std::optional<Result>();
            }
        };
        auto new_compose_func = [compose_func, functor](std::optional<Argument> data)->std::optional<Result>{
            auto val = compose_func(std::move(data));
            if(val.has_value()){
               return std::optional<Result>(functor(std::move(val)));
            }else{
                return std::optional<Result>();
            }
        };

        auto&& temp =  RangeView<Result, Argument>(std::move(range_view.data_),
                                     new_compose_func,
                                     range_view.gen_,
                                     range_view.has_gen_,
                                     range_view.is_reverse_);
        return std::move(temp);

    };
    return Function<decltype(lambda)>(lambda);
}
inline auto take(unsigned int n) {
   auto lambda = [=] (auto&& range_view) {
        auto compose_func = range_view.compose_func_;
        using Result = typename decltype(compose_func)::result_type::value_type;
        using Argument = typename decltype(range_view.data_)::value_type;
        std::vector<Result> result_vector;
        std::function<std::optional<Result>(std::optional<Result>)> f = [](std::optional<Result> data){
            return std::move(data);
        };
            
        if(range_view.has_gen_){
            assert(range_view.data_.empty());
            if constexpr (std::is_same_v<Argument, int>){
                for(std::size_t i = 0; i < n; i++){
                    auto val = compose_func(std::optional<Argument>(i+range_view.gen_));
                    if(val.has_value()){
                        result_vector.emplace_back(std::move(val.value()));
                    }
                }
                if(range_view.is_reverse_){
                    std::reverse(std::begin(result_vector), std::end(result_vector));
                }
            }
        }else{
            result_vector = range_view.to_vector();
            if(n < result_vector.size()){
                result_vector.resize(n);
            }
        }
        auto&& temp =  RangeView<Result, Result>(std::move(result_vector),
                                    f,
                                    range_view.gen_,
                                    false,
                                    false);
        return std::move(temp);
    
    };
    return Function<decltype(lambda)>(lambda);
}

inline auto ints(int n) {
   auto lambda = [=](auto&& range_view) {
        //assert(range_view.get_data().empty());
        using Argument = typename decltype(range_view.data_)::value_type;
        auto compose_func = range_view.compose_func_;
        using Result = typename decltype(compose_func)::result_type::value_type;
        auto&& temp = RangeView<Result, Argument>(std::move(range_view.data_),
                                    range_view.compose_func_,
                                    n,
                                    true,
                                    range_view.is_reverse_);
        return std::move(temp);
   };
   return Function<decltype(lambda)>(lambda);
}

template <class TView>
int accumulate(TView view){
    auto vec = view.to_vector();
    return std::accumulate(std::begin(vec), std::end(vec),0);
}

template <typename U, typename Func>
auto operator | (std::vector<U>& vector, Function<Func> func) {
    RangeView<U,U> temp(std::move(vector));
    return func.apply(std::move(temp));
}

template <typename Func1, typename Func2>
auto operator | (Function<Func1> init_func, Function<Func2> func) {
    return func.apply(init_func.apply(std::move(RangeView<int,int>())));
}

template <typename T, typename U>
class RangeView {
public:
    using OptU = std::optional<U>;
    using OptT = std::optional<T>;
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
    friend auto ints(int n);
    friend auto take(unsigned int n);
    template<typename Func>
    friend auto remove_if(Func&& func);
    friend auto reverse();
    template<typename Func>
    friend auto transform(Func&& func);

public:

    auto to_vector(){
        std::vector<T> res;
        if(is_reverse_){
            std::reverse(std::begin(data_),std::end(data_));
        }
        for(std::size_t i = 0; i < data_.size(); i++){
            auto value = compose_func_(std::optional<U>(std::move(data_[i])));
            if(value.has_value())
                res.emplace_back(std::move(value.value()));
        }
        return std::move(res);
    }
    RangeView(){}

    RangeView(std::vector<U>&& data, std::function<OptT(OptU)> compose_func, int gen, bool has_gen, bool is_reverse)
        : data_(std::move(data)), compose_func_(compose_func), gen_(gen), has_gen_(has_gen), is_reverse_(is_reverse)
    {}
      RangeView(const std::vector<U>& data, std::function<OptT(OptU)> compose_func, int gen, bool has_gen, bool is_reverse)
        : data_(std::move(data)), compose_func_(compose_func), gen_(gen), has_gen_(has_gen), is_reverse_(is_reverse)
    {}

    RangeView(const std::vector<U>& v) : data_(std::move(v)), gen_(0), has_gen_(false), is_reverse_(false) {
        compose_func_ = [](OptU data){
            return std::move(data);
        };

    }
    RangeView(std::vector<U>&& v) : data_(std::move(v)), gen_(0), has_gen_(false), is_reverse_(false) {
        compose_func_ = [](OptU data){
            return std::move(data);
        };
    }

private:
    std::vector<U> data_ = std::vector<U>();
    std::function<OptT(OptU)> compose_func_ = [](OptU data){
        return std::move(data);
    };
    int gen_;
    bool has_gen_ = false;
    bool is_reverse_ = false;
};

}//rangeview

#endif // VIEW_H
