#pragma once

#include "IProtocolHandler.hpp"
#include "HttpSimpleParser.hpp"
#include "HttpResponseBuilder.hpp"

#include <string>


class IntervalStorage;
class Interval;

namespace Http {

class HttpHandler
    : public Network::IProtocolHandler
{
public:
    explicit HttpHandler(IntervalStorage& intervalStorage);

    virtual bool Run(const std::string &buffer) throw();
    virtual const std::string& GetAnswer() throw();

private:
    void _Handle(const Request& request);
    Interval _toInterval(std::string interval) const;
    std::string _toString(const Interval &interval) const;

    IntervalStorage&    _intervalStorage;
    std::string         _answer;
    HttpSimpleParser    _parser;
    HttpResponseBuilder _responseBuilder;

};

} // namespace Http
