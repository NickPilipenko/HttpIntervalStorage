#pragma once

#include <utility>
#include <boost/shared_ptr.hpp>

class Interval
{
public:
    typedef std::pair<long, long> PairT;
    typedef boost::shared_ptr<Interval> PtrT;

    Interval(long, long);
    Interval(const PairT &);
    Interval();

    bool operator< (const Interval &a) const;
    const PairT& GetValue() const throw();
    bool Containe(const Interval &a) const throw();

private:
    void _Normalize() throw();

    PairT _coordinates;
};

