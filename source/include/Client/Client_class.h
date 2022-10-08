#pragma once

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "Common.hpp"
#include "json.hpp"

using boost::asio::ip::tcp;
using std::string_literals::operator""s;


class Client {
public:
    Client(boost::asio::io_service& io_service);

    void SendMessage(const Requests& aRequestType, const std::string& aUserName,
                        const std::string& aPassword );

    void SendRequest(const Requests& aRequestType,
                        const std::string& aUserName );

    std::vector<std::string> GetActiveUserTrades(const std::string& aUserName,
                                                    const Requests& aRequestType);

    std::vector<std::string> GetCompletedTradesList(const std::string& aUserName,
                                                        const Requests& aRequestType);

    std::string SendTradeRequest(const std::string& aUserName, const Requests& aRequestType,
                                    const int sum, const int cost );

    std::vector<std::string> ReadMessages();

    std::string ReadMessage();

    std::string ProcessRegistration(Requests req);

    std::string GetBalance();

    std::string GetUserName() const;

private:
    tcp::resolver resolver_;
    tcp::socket socket_;
    std::string user_Name_;
};

