#pragma once

#include <string>

namespace Http {

enum eMethod
{
    HM_NOT_SUPPORTED = 0,
    HM_GET,
    HM_HEAD,
    HM_OPTIONS,

    HM_SEARCH,
    HM_ADD,
    HM_REMOVE
};

struct Request
{
    eMethod method;
    std::string data;
};

class HttpSimpleParser
{
public:
    HttpSimpleParser();
    bool Parse(const std::string &data);
    void Reset()throw();
    const Request& GetRequest()throw();
private:
    bool _Consume();
    void _ParseHeader(std::string headerLine);

    enum eState
    {
        ST_STARTING_LINE,
        ST_HEADERS,
        ST_BODY,
        ST_DONE
    };

    eState _state;
    std::string _data;
    size_t _contentLength;
    Request _request;

};

} // namespace Http
