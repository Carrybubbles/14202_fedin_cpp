#ifndef STREAM_H
#define STREAM_H

#include <vector>
template<class T>
class Stream{
public:
    using size_type = std::size_t;
public:
    template<class Iterator>
    Stream(Iterator begin, Iterator end){
        for(auto it = begin; it != end; ++it){
            data_.push_back(*it);
        }
    }

    Stream(const Container& cont): container_(cont){
    }

    Stream(const Container&& cont){
        container_ = std::move(cont);
    }

    Stream(std::initializer_list<T> init){
        for(auto it = list.begin(); it != init.end(); ++it){
            container_.push_back(*it);
        }
    }

    Stream(Generator&& generator){
        generator_ = generator;
    }

    template<class Arg, class.. Args>
    Stream(Arg arg, Args.. rest){
        container_.push_back(arg);
        Stream(rest..);
    }

    void get(size_type n){

    }

private:
    std::vector<T> data_;
    Generator generator_;
    Container container_;
}
#endif // STREAM_H
