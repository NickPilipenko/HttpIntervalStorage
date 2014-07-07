#pragma once

#include "IProtocolHandler.hpp"

class IntervalStorage;

namespace Http {

class HttpHandlerFabrica
    : public Network::IProtocolHandlerFabrica
{
public:
    explicit HttpHandlerFabrica(IntervalStorage &intervalStorage);
    virtual ~HttpHandlerFabrica();

    virtual Network::IProtocolHandler::PtrT GetProtocolHandler() throw();
protected:
    IntervalStorage& _intervalStorage;

};

} // namespace Http
