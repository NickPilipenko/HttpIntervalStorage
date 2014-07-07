#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>


namespace Network {

class IProtocolHandler
    : private boost::noncopyable
{
public:
    IProtocolHandler(){};
    virtual ~IProtocolHandler(){};

    typedef boost::shared_ptr<IProtocolHandler> PtrT;

    virtual bool Run(const std::string &buffer) throw() = 0;
    virtual const std::string& GetAnswer() throw() = 0;
};


class IProtocolHandlerFabrica
    : private boost::noncopyable
{
public:
    IProtocolHandlerFabrica(){};
    virtual ~IProtocolHandlerFabrica(){};

    virtual IProtocolHandler::PtrT GetProtocolHandler() throw() = 0;
};

} // namespace Network
