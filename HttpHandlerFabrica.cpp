#include "HttpHandlerFabrica.hpp"
#include "HttpHandler.hpp"

using namespace Http;

HttpHandlerFabrica::HttpHandlerFabrica(IntervalStorage &intervalStorage)
    : _intervalStorage(intervalStorage)
{

}


HttpHandlerFabrica::~HttpHandlerFabrica()
{

}


Network::IProtocolHandler::PtrT
HttpHandlerFabrica::GetProtocolHandler() throw()
{
    return Network::IProtocolHandler::PtrT(new HttpHandler(_intervalStorage));
}
