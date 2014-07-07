#include "Connection.hpp"

#include <boost/bind.hpp>

#include <vector>
#include <string>


namespace Network {

using namespace boost;

Connection::Connection(BA::io_service& io_service, IProtocolHandler::PtrT requestHandler)
    : _strand(io_service),
      _socket(io_service),
      _requestHandler(requestHandler)
{

}

BA::ip::tcp::socket& Connection::GetSocket()
{
    return _socket;
}


void Connection::Start()
{
    _socket.async_read_some(BA::buffer(_buffer),
                            _strand.wrap(bind(&Connection::_HandleRead, shared_from_this(),
                                              BA::placeholders::error, BA::placeholders::bytes_transferred)));
}


void Connection::_HandleRead(const system::error_code& err, std::size_t bytesTransferred)
{
    if (!err)
    {
        if (_requestHandler->Run(std::string(_buffer.data(), bytesTransferred)))
        {
            std::string _answer(_requestHandler->GetAnswer());

            //std::cout << "ANSWER: " << _answer << std::endl << std::flush;
            BA::async_write(_socket, BA::buffer(_answer.c_str(), _answer.size()),
                            _strand.wrap(bind(&Connection::_HandleWrite, shared_from_this(),
                                              BA::placeholders::error)));
        }
        else
        { // Need more data.
            _socket.async_read_some(BA::buffer(_buffer),_strand.wrap(bind(&Connection::_HandleRead, shared_from_this(),
                                                                          BA::placeholders::error,
                                                                          BA::placeholders::bytes_transferred)));
        }
    }
}


void Connection::_HandleWrite(const boost::system::error_code& err)
{
    if (!err)
    {
        system::error_code ignored_ec;
        _socket.shutdown(BA::ip::tcp::socket::shutdown_both, ignored_ec);
    }
}

} // namespace Network

