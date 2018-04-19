#ifndef STREAM_H
#define STREAM_H

#include <vector>

#include "area.h"

template<class T,class Container, class Generator, class Iterator>
class Stream{
public:
    using size_type = std::size_t;
public:

    Stream(Iterator begin, Iterator end): area_(begin, end){}

    Stream(const Container& cont): container_(cont), area_(std::begin(container_), std::end(container_)){
    }

    Stream(const Container&& cont): container_(std::move(cont)), area_(std::begin(container_), std::end(container_)){}

    Stream(std::initializer_list<T> init):container_(init), area_(std::begin(container_), std::end(container_)){}

    Stream(Generator&& generator): generator_(generator){
    }


    template <typename Argument, typename... Args>
    Stream(Argument&& first, Args&&... others):
    {
        varConst(std::move(first), std::move(others));
    }


    void get(size_type n){

    }

private:
    Generator generator_ = Generator();
    Container container_ = Container();
    Area<Iterator> area_ = Area();

private:
    template <typename Argument>
    void varConst(Argument&& value)
    {
        container_->push_back(std::forward<Argument>(value));
    }

    template <typename Argument, typename... Args>
    void varConst(Argument&& value, Args&&... others)
    {
        container_->push_back(std::forward<Argument>(value));
        varConst(std::forward<Args>(others)...);
    }


}
#endif // STREAM_H
