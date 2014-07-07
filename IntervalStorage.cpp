#include "IntervalStorage.hpp"

IntervalStorage::IntervalStorage()
{

}

void IntervalStorage::Add(const Interval &interval) throw()
{
    boost::unique_lock<boost::shared_mutex> readLock(_mapGuard);
    _intervals.insert(interval);
}


void IntervalStorage::Remove(const Interval &interval) throw()
{
    boost::unique_lock<boost::shared_mutex> readLock(_mapGuard);
    _intervals.erase(interval);
}

Interval::PtrT IntervalStorage::Search(const Interval &interval) const throw()
{
    boost::shared_lock<boost::shared_mutex> readLock(_mapGuard);

    for (SetT::reverse_iterator _riteratorLefter(_intervals.upper_bound(interval));
         _riteratorLefter != _intervals.rend();
         ++_riteratorLefter)
    {
        if (_riteratorLefter->GetValue().second >= interval.GetValue().second)
            return Interval::PtrT(new Interval(*_riteratorLefter));
    }

    return Interval::PtrT();
}
