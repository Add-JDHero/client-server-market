#pragma once
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

using boost::asio::ip::tcp;
using std::string_literals::operator""s;

#include "Session.h"
#include "Common.hpp"


class Server {
public:
    Server(boost::asio::io_service& io_service);

    void Handle_accept(Session* new_session,
                       const boost::system::error_code& error);

private:
    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor_;
};

