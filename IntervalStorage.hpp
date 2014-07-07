#pragma once

#include "Interval.hpp"

#include <boost/thread/shared_mutex.hpp>

#include <set>

class IntervalStorage
{
public:
    IntervalStorage();

    void Add(const Interval &) throw();
    void Remove(const Interval &) throw();

    Interval::PtrT Search(const Interval &) const throw();
private:

    mutable boost::shared_mutex _mapGuard;

    typedef std::set<Interval> SetT;
    SetT _intervals;

};
