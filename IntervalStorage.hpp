#pragma once

#include "Interval.hpp"

#include <boost/thread/shared_mutex.hpp>

#include <map>

class IntervalStorage
{
public:
    IntervalStorage();

    void Add(const Interval &) throw();
    void Remove(const Interval &) throw();

    Interval::PtrT Search(const Interval &) const throw();

    typedef std::map<Interval, const Interval*> MapT;
private:
    void _Rebuild() throw();

    mutable boost::shared_mutex _mapGuard;
    MapT _intervals;
};
