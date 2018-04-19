#ifndef AREA_H
#define AREA_H

#include <optional>

template <typename Iterator>
class Area
{
public:
    Area(){}
    Area(const Iterator& head,const std::optional<Iterator>& tail): head_(head), tail_(tail){}
private:
    Iterator head_;
    std::optional<Iterator> tail_;
};
#endif // AREA_H
