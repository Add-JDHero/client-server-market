#include "Core.h"

std::string Core::RegisterNewUser(const std::string& aUserName, const std::string& aUserPsw) {
    if (mUsers_.count(aUserName) == 0) {
        mUsers_[aUserName] = aUserPsw;
        user_balance_[aUserName] = {0, 0};
    } else {
        return "User name " + aUserName + " already in use.";
    }
    return "Welcome, " + aUserName + "!";
}

std::string Core::Authorisation(const std::string& aUserName, const std::string& aUserPsw) {
    const auto userIt = mUsers_.find(aUserName);
    if (userIt == mUsers_.cend() || userIt->second != aUserPsw) {
        return "Authorisation failed: invalid user name or password"s;
    } else {
        return "Success"s;
    }
}

std::string Core::HandleRequest(const std::string& aUserName, const Requests req, int sum, int cost) {
    int result_sum = sum;
    if (req == Requests::BUY) {
        int max = cost;
        auto it = active_trades_.begin();
        while ((it = Min_element(active_trades_.begin(), active_trades_.end(), Requests::SELL)) != active_trades_.end() && it->second.Cost <= max) {
            if (result_sum >= it->second.Sum) {
                std::string partner_name = it->first;
                MakeDeal(it, aUserName, partner_name, result_sum);
            }
        }

    } else if (req == Requests::SELL){
        int min = cost;
        auto it = active_trades_.begin();
        while ((it = Max_element(active_trades_.begin(), active_trades_.end(), Requests::BUY)) != active_trades_.end() && it->second.Cost >= min) {
            if (result_sum >= it->second.Sum) {
                std::string partner_name = it->first;
                MakeDeal(it, aUserName, partner_name, result_sum);
            }
        }

    } else {
        return "Unknown request.";
    }

    if (result_sum != 0) {
        active_trades_.push_back({aUserName, {req, result_sum, cost}});
    }
    return "The request has been sent successfully.";
}

std::map<std::string, std::string>& Core::GetUsersList () {
    return mUsers_;
}

std::string Core::GetBalance(const std::string& aUserName) {
    std::string balance = "Usd: " + std::to_string(user_balance_[aUserName].first) + "  Rub: " + std::to_string(user_balance_[aUserName].second);
    return balance;
}

std::vector<std::string> Core::GetActiveTrades(const std::string& aUserName) {
    std::vector<std::string> result;
    std::string request;
    for (const auto& order: active_trades_) {
        if (order.first == aUserName) {
            request = order.second.req != Requests::BUY ? "SELL"s : "BUY"s;
            result.push_back(request + "_usd:"s + std::to_string(order.second.Sum) + "_rub:"s + std::to_string(order.second.Cost) + " "s);
        }
    }
    if (!result.empty()) {
        result.back() += '\n';
    } else {
        result.push_back("empty_list\n");
    }
    return std::move(result);
}

std::vector<std::string> Core::GetCompletedTradesList(const std::string& aUserName) {
    std::vector<std::string> result;
    std::string request;
    for (const auto& TradeDeal: completed_trades_[aUserName]) {
        request = TradeDeal.req != Requests::BUY ? "SELL"s : "BUY"s;
        result.push_back(request + "_usd:"s + std::to_string(TradeDeal.Sum) + "_rub:"s + std::to_string(TradeDeal.Cost) + " "s);
    }
    if (!result.empty()) {
        result.back() += '\n';
    } else {
        result.push_back("empty_list\n");
    }
    return std::move(result);
}

Core::iterator Core::Max_element(iterator first, iterator last, Requests req) {
    if (first == last) return last;
    first = std::find_if(
            first, last,
            [&req](auto& element) {
                return element.second.req == req;
            });
    iterator largest = first;
    while ( (first = std::find_if(
            ++first, last,
            [&req](auto& element){
                return element.second.req == req;
            })) != last ) {

        if (largest->second.Cost < first->second.Cost) {
            largest = first;
        }
    }
    return largest;
}

Core::iterator Core::Min_element(iterator first, iterator last, Requests req) {
    if (first == last) return last;
    first = std::find_if(
            first, last,
            [&req](auto& element) {
                return element.second.req == req;
            });
    iterator smallest = first;
    while ( (first = std::find_if(
            ++first, last,
            [&req](auto& element){
                return element.second.req == req;
            })) != last ) {

        if (first->second.Cost < smallest->second.Cost) {
            smallest = first;
        }
    }
    return smallest;
}

void Core::UpdateUsersBalance(const std::string& aTraderName, const std::string& aBuyerName, iterator it) {
    user_balance_[aTraderName].first -= it->second.Sum;
    user_balance_[aTraderName].second += it->second.Sum * it->second.Cost;

    user_balance_[aBuyerName].first += it->second.Sum;
    user_balance_[aBuyerName].second -= it->second.Cost * it->second.Sum;
}

void Core::MakeDeal(iterator it, const std::string& initiator, const std::string& partner, int& result_sum) {
    result_sum -= it->second.Sum;

    int usd_trade_count = it->second.Sum;
    int rub_cost_per_one = it->second.Cost;
    Requests request = it->second.req == Requests::BUY ? Requests::SELL : Requests::BUY;
    completed_trades_[partner].push_back({it->second.req, usd_trade_count, rub_cost_per_one});
    completed_trades_[initiator].push_back({request, usd_trade_count, rub_cost_per_one});

    if (request == Requests::BUY) {
        UpdateUsersBalance(partner, initiator, it);
    } else {
        UpdateUsersBalance(initiator, partner, it);
    }
    active_trades_.erase(it);
}

Core& GetCore() {
    static Core core;
    return core;
}