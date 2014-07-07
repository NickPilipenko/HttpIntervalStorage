#include "Interval.hpp"

Interval::Interval()
    : _coordinates(0,0)
{

}

Interval::Interval(long begin, long end)
    : _coordinates(begin, end)
{
    _Normalize();
}


Interval::Interval(const PairT &coord)
    : _coordinates(coord)
{
    _Normalize();
}



void Interval::_Normalize() throw()
{
    if (_coordinates.first > _coordinates.second)
        std::swap(_coordinates.first, _coordinates.second);
}


Interval::PairT Interval::GetValue() const throw()
{
    return _coordinates;
}


bool Interval::operator< (const Interval &a) const
{
    if (_coordinates.first == a._coordinates.first)
        return _coordinates.second > a._coordinates.second;
    return _coordinates.first < a._coordinates.first;
}


