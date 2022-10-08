#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using std::string_literals::operator""s;

#include "Server_class.h"

int main() {
    try {
        boost::asio::io_service io_service;
        static Core core;

        Server s(io_service);

        io_service.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}