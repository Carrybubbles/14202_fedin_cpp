#ifndef STREAM_H
#define STREAM_H

#include <vector>
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
    Stream(Argument&& first, Args&&... others)
    {
        container_->push_back(std::forward<Argument>(head));
        this(std::forward<Args>(tail)...);
    }


    void get(size_type n){

    }

private:
    Generator generator_;
    Container container_;
    Area<Iterator> area_;

}
#endif // STREAM_H
