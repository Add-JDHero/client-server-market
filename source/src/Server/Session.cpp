#include "Session.h"

Session::Session(boost::asio::io_service& io_service)
        : socket_(io_service)
{
}

tcp::socket& Session::Socket() {
    return socket_;
}

void Session::Start() {
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            boost::bind(&Session::Handle_read, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}

// Обработка полученного сообщения.
void Session::Handle_read(const boost::system::error_code& error,
                 size_t bytes_transferred) {
    if (!error) {
        data_[bytes_transferred] = '\0';

        // Парсим json, который пришёл нам в сообщении.
        auto j = nlohmann::json::parse(data_);
        auto reqType = j["ReqType"];

        std::string reply = "Error! Unknown request type";
        std::vector<std::string> spec_reply;
        if (reqType == Requests::REGISTRATION){
            reply = GetCore().RegisterNewUser(j["UserName"], j["Password"]);
        } else if (reqType == Requests::SIGNIN) {
            reply = GetCore().Authorisation(j["UserName"], j["Password"]);
        } else if (reqType == Requests::BUY || reqType == Requests::SELL) {
            reply = GetCore().HandleRequest(j["UserName"], reqType, j["Sum"], j["Cost"]);
        } else if (reqType == Requests::BALANCE) {
            reply = GetCore().GetBalance(j["UserName"]);
        } else if (reqType == Requests::COMPLETEDTRADES) {
            spec_reply = GetCore().GetCompletedTradesList(j["UserName"]);
        } else if (reqType == Requests::ACTIVEUSERTRADES) {
            spec_reply = GetCore().GetActiveTrades(j["UserName"]);
        }
        if (!spec_reply.empty()) {
            reply.clear();
            for (const auto& str: spec_reply) {
                reply += str;
            }
        }

        boost::asio::async_write(socket_,
                                 boost::asio::buffer(reply, reply.size()),
                                 boost::bind(&Session::Handle_write, this,
                                             boost::asio::placeholders::error));
    } else {
        delete this;
    }
}

void Session::Handle_write(const boost::system::error_code& error) {
    if (!error) {
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                boost::bind(&Session::Handle_read, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    } else {
        delete this;
    }
}
