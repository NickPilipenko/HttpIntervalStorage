#include "Server.hpp"


#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

#include <vector>


namespace Network {

using namespace boost;

Server::Server(const std::string& address, const std::string& port, std::size_t sizeThreadPool,
               IProtocolHandlerFabrica& handlerFabrica)
    : _sizeThreadPool(sizeThreadPool),
      _signals(_ioService),
      _acceptor(_ioService),
      _newConnection(),
      _handlerFabrica(handlerFabrica)
{
    _signals.add(SIGTERM);
    _signals.add(SIGINT);
    _signals.async_wait(bind(&Server::_HandleStop, this));

    BA::ip::tcp::resolver _resolver(_ioService);
    BA::ip::tcp::resolver::query _query(address, port);
    BA::ip::tcp::endpoint _endpoint = *_resolver.resolve(_query);

    _acceptor.open(_endpoint.protocol());
    _acceptor.set_option(BA::ip::tcp::acceptor::reuse_address(true));
    _acceptor.bind(_endpoint);
    _acceptor.listen();

    _StartAccept();
}


void Server::Run()
{
    // Create pool of threads for all of io_services.
    std::vector<shared_ptr<thread> > _threads;
    for (std::size_t i = 0; i < _sizeThreadPool; ++i)
    {
        shared_ptr<thread> _thread(new thread(bind(&BA::io_service::run, &_ioService)));
        _threads.push_back(_thread);
    }

    for (std::size_t i = 0; i < _threads.size(); ++i)
        _threads[i]->join();
}


void Server::_StartAccept()
{
    _newConnection.reset(new Connection(_ioService, _handlerFabrica.GetProtocolHandler()));
    _acceptor.async_accept(_newConnection->GetSocket(), bind(&Server::_HandleAccept, this, BA::placeholders::error));
}


void Server::_HandleAccept(const system::error_code& err)
{
    if (!err)
    {
        _newConnection->Start();
    }
    _StartAccept();
}


void Server::_HandleStop()
{
    _ioService.stop();
}

} // namespace Network
