#include "IntervalStorage.hpp"

class IntervalStorageUpdater
{
public:
    IntervalStorageUpdater()
    : _maxLeftCoordinate(NULL)
    {

    }

    void operator()(IntervalStorage::MapT::value_type &mapMember)
    {
        if (!_maxLeftCoordinate)
        {
            _maxLeftCoordinate = &(mapMember.first);
            mapMember.second = _maxLeftCoordinate;
        }

        if (_maxLeftCoordinate->GetValue().second < mapMember.first.GetValue().second)
            _maxLeftCoordinate = &(mapMember.first);

        mapMember.second = _maxLeftCoordinate;
    }
private:
    const Interval *_maxLeftCoordinate;
};


IntervalStorage::IntervalStorage()
{

}


void IntervalStorage::Add(const Interval &interval) throw()
{
    boost::unique_lock<boost::shared_mutex> readLock(_mapGuard);
    _intervals.insert(MapT::value_type(interval,NULL));
    _Rebuild();
}


void IntervalStorage::Remove(const Interval &interval) throw()
{
    boost::unique_lock<boost::shared_mutex> readLock(_mapGuard);
    _intervals.erase(interval);
    _Rebuild();
}


void IntervalStorage::_Rebuild() throw()
{
    for_each(_intervals.begin(), _intervals.end(), IntervalStorageUpdater());
}


Interval::PtrT IntervalStorage::Search(const Interval &interval) const throw()
{
    boost::shared_lock<boost::shared_mutex> readLock(_mapGuard);

    MapT::const_iterator _iteratoLower = _intervals.upper_bound(interval);
    if (!_intervals.empty() && _iteratoLower != _intervals.begin())
    {
        --_iteratoLower;

        if (_iteratoLower->second->Containe(interval))
            return Interval::PtrT(new Interval(*(_iteratoLower->second)));

    }

    return Interval::PtrT();
}
