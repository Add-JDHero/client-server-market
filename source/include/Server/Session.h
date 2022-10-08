#pragma once
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

using boost::asio::ip::tcp;
using std::string_literals::operator""s;

#include "Core.h"

class Session {
public:
    Session(boost::asio::io_service& io_service);

    tcp::socket& Socket();
    void Start();

    void Handle_read(const boost::system::error_code& error,
                     size_t bytes_transferred);
    void Handle_write(const boost::system::error_code& error);

private:
    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

