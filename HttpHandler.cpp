#include "HttpHandler.hpp"

#include "HttpException.hpp"
#include "IntervalStorage.hpp"
#include "Interval.hpp"

#include <sstream>
#include <cstdlib>
#include <climits>

using namespace Http;


HttpHandler::HttpHandler(IntervalStorage& intervalStorage)
    : _intervalStorage(intervalStorage)
{

}


bool HttpHandler::Run(const std::string &buffer) throw()
{
    try
    {
        if (!_parser.Parse(buffer))
            return false; // Need more data.

        _Handle(_parser.GetRequest());

    }
    catch(const HttpException &error) //
    {
        _answer.assign(_responseBuilder.Build(error));
    }

    return true; // Answer is ready.
}


void HttpHandler::_Handle(const Request& request)
{
    Interval _interval;
    switch(request.method)
    {
    case HM_ADD:
        _interval = _toInterval(request.data); // throw incorect data
        _intervalStorage.Add(_interval);
        _answer.assign(_responseBuilder.Build("ADD"));
        break;

    case HM_REMOVE:
        _interval = _toInterval(request.data); // throw incorect data
        _intervalStorage.Remove(_interval);
        _answer.assign(_responseBuilder.Build("REMOVE"));
        break;

    case HM_SEARCH:
        _interval = _toInterval(request.data); // throw incorect data
        {
            Interval::PtrT _intervalMore(_intervalStorage.Search(_interval));
            if (_intervalMore)
                _answer.assign(_responseBuilder.Build(_toString(*_intervalMore)));
            else
                _answer.assign(_responseBuilder.Build("Is absent"));
        }
        break;

    case HM_OPTIONS:
        _answer.assign(_responseBuilder.BuildOption());
        break;

    case HM_GET:
    case HM_HEAD:
        _answer.assign(_responseBuilder.Build(HttpException(HE_NOT_ALLOWED)));
        break;

    case HM_NOT_SUPPORTED:
    default:
        _answer.assign(_responseBuilder.Build(HttpException(HE_NOT_IMPLEMENTED)));
        break;
    }

}


const std::string& HttpHandler::GetAnswer() throw()
{
    return _answer;
}


Interval HttpHandler::_toInterval(std::string interval) const
{
    int _a(0), _b(0);
    interval.erase(std::remove(interval.begin(), interval.end(), ' '), interval.end());

    if (!interval.size() || interval[0] != '[' || interval[interval.size()-1] != ']')
        throw HttpException(HE_BAD_REQUEST);

    size_t _pos = interval.find(',');
    if (_pos == std::string::npos)
        throw HttpException(HE_BAD_REQUEST);
    _pos++;

    _a = strtol(interval.c_str()+1, NULL, 10);
    _b = strtol(interval.c_str()+_pos, NULL, 10);

    if (_a == LONG_MAX || _a == LONG_MIN || _b == LONG_MAX || _b == LONG_MIN)
            throw HttpException(HE_BAD_REQUEST);

    return Interval(_a, _b);
}


std::string HttpHandler::_toString(const Interval &interval) const
{
    std::ostringstream _oss;
    Interval::PairT _interval = interval.GetValue();
    _oss << "[" << _interval.first << ", " << _interval.second << "]";
    return _oss.str();
}


