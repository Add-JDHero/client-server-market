#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <map>
#include <list>
#include <deque>

using boost::asio::ip::tcp;
using std::string_literals::operator""s;

#include "Common.hpp"
#include "json.hpp"


class Core {
public:
    std::string RegisterNewUser(const std::string& aUserName, const std::string& aUserPsw);

    std::string Authorisation(const std::string& aUserName, const std::string& aUserPsw);

    std::string HandleRequest(const std::string& aUserName, const Requests req, int sum, int cost);

    std::map<std::string, std::string>& GetUsersList();

    std::string GetBalance(const std::string& aUserName);

    std::vector<std::string> GetActiveTrades(const std::string& aUserName);

    std::vector<std::string> GetCompletedTradesList(const std::string& aUserName);

private:
    struct TradeDeal {
        Requests req;
        int Sum;
        int Cost;
    };
    using iterator = std::list<std::pair<std::string, Core::TradeDeal>>::iterator;

    iterator Max_element(iterator first, iterator last, Requests req);

    static iterator Min_element(iterator first, iterator last, Requests req);

    void UpdateUsersBalance(const std::string& aTraderName, const std::string& aBuyerName, iterator it);

    void MakeDeal(iterator it, const std::string& initiator, const std::string& partner, int& result_sum);

    std::map<std::string, std::string> mUsers_;
    std::map<std::string, std::pair<int, int>> user_balance_;
    std::list<std::pair<std::string, TradeDeal>> active_trades_;
    std::map<std::string, std::deque<TradeDeal>> completed_trades_;
};

Core& GetCore();

