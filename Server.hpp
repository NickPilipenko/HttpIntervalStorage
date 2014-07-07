#pragma once

#include "Connection.hpp"
#include "IProtocolHandler.hpp"
#include "NetworkCommon.hpp"

#include <boost/noncopyable.hpp>


namespace Network {

class Server
    : private boost::noncopyable
{
public:
    explicit Server(const std::string& address, const std::string& port, std::size_t sizeThreadPool,
                    IProtocolHandlerFabrica& handlerFabrica);
    void Run();

private:
    void _StartAccept();
    void _HandleAccept(const boost::system::error_code& err);
    void _HandleStop();

    std::size_t _sizeThreadPool;
    BA::io_service _ioService;
    BA::signal_set _signals;
    BA::ip::tcp::acceptor _acceptor;

    Connection::PtrT _newConnection;
    IProtocolHandlerFabrica& _handlerFabrica;
};

} // namespace Network
