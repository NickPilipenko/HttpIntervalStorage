#include "HttpSimpleParser.hpp"

#include "HttpException.hpp"

#include <cstdlib>
#include <climits>

using namespace Http;

using namespace std;


static eMethod _GetMethod(string startingLine)
{
    size_t posSpace = startingLine.find(' ');
    if (posSpace == string::npos)
        throw HttpException(HE_BAD_REQUEST);
    startingLine.erase(posSpace);
    if (startingLine == "ADD")
        return HM_ADD;
    if (startingLine == "SEARCH")
        return HM_SEARCH;
    if (startingLine == "REMOVE")
        return HM_REMOVE;
    if (startingLine == "GET")
        return HM_GET;
    if (startingLine == "OPTIONS")
        return HM_OPTIONS;
    return HM_NOT_SUPPORTED;

}


HttpSimpleParser::HttpSimpleParser()
{
    Reset();
}



bool HttpSimpleParser::Parse(const string &data)
{
    _data.append(data);
    while (_Consume());
    return _state == ST_DONE;
}


bool HttpSimpleParser::_Consume()
{
    size_t pos = _data.find("\r\n");

    switch (_state)
    {
    case ST_STARTING_LINE:
        if (pos == string::npos)
            return false;

        _request.method = _GetMethod(_data.substr(0,pos+2));
        _state = ST_HEADERS;
        break;
    case ST_HEADERS:
        if (pos == string::npos)
            return false;

        if (!pos)
        {
            // String == "\r\n" - empty string, end of headers section
            _state = ST_BODY;
        }
        else
        {
            _ParseHeader(_data.substr(0, pos+2));
        }
        break;
    case ST_BODY:
        if (_data.size() != _contentLength)
            return false;

        _request.data.assign(_data);
        _state = ST_DONE;
        return false;
    case ST_DONE:
        return false;
    }

    _data.erase(0, pos+2);
    return true;
}


void HttpSimpleParser::_ParseHeader(string headerLine)
{
    size_t posSpace = headerLine.find(": ");
    if (posSpace == string::npos)
        throw HttpException(HE_BAD_REQUEST);

    if (headerLine.substr(0, posSpace) == "Content-Length")
    {
        _contentLength = strtol(headerLine.substr(posSpace+2).c_str(), NULL, 10);
        if (_contentLength == LONG_MAX)
            throw HttpException(HE_BAD_REQUEST);
    }
}


void HttpSimpleParser::Reset() throw()
{
    _data.clear();
    _contentLength = 0;
    _state = ST_STARTING_LINE;
    _request.method = HM_NOT_SUPPORTED;
    _request.data.clear();
}


const Request& HttpSimpleParser::GetRequest()throw()
{
    return _request;
}
