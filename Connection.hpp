#pragma once

#include "NetworkCommon.hpp"
#include "IProtocolHandler.hpp"

#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>

#include <string>

namespace Network {

class Connection
    : public boost::enable_shared_from_this<Connection>,
      private boost::noncopyable
{
public:
    explicit Connection(BA::io_service& io_service, IProtocolHandler::PtrT requestHandler);

    typedef boost::shared_ptr<Connection> PtrT;

    BA::ip::tcp::socket& GetSocket();
    void Start();

private:
    void _HandleRead(const boost::system::error_code& err, std::size_t bytesTransferred);
    void _HandleWrite(const boost::system::error_code& err);

    BA::io_service::strand _strand;
    BA::ip::tcp::socket _socket;
    boost::array<char, 1024> _buffer;

    IProtocolHandler::PtrT _requestHandler;
};

} // namespace Network
