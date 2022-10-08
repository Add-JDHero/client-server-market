#include "Server_class.h"

Server::Server(boost::asio::io_service& io_service)
        : io_service_(io_service),
          acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
{
    std::cout << "Server started! Listen " << port << " port" << std::endl;

    Session* new_session = new Session(io_service_);
    acceptor_.async_accept(new_session->Socket(),
                           boost::bind(&Server::Handle_accept, this, new_session,
                                       boost::asio::placeholders::error));
}

void Server::Handle_accept(Session* new_session,
                   const boost::system::error_code& error) {
    if (!error) {
        new_session->Start();
        new_session = new Session(io_service_);
        acceptor_.async_accept(new_session->Socket(),
                               boost::bind(&Server::Handle_accept, this, new_session,
                                           boost::asio::placeholders::error));
    } else {
        delete new_session;
    }
}
