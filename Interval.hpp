#pragma once

#include <utility>
#include <boost/shared_ptr.hpp>

class Interval
{
public:
    typedef std::pair<long, long> PairT;


    Interval(long, long);
    Interval(const PairT &);
    Interval();

    bool operator< (const Interval &a) const;

    typedef boost::shared_ptr<Interval> PtrT;

    PairT GetValue() const throw();

private:
    void _Normalize() throw();

    PairT _coordinates;
};

