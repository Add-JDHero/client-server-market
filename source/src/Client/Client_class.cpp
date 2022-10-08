#include "Client_class.h"

Client::Client(boost::asio::io_service& io_service)
: resolver_(io_service),
socket_(io_service)
{
    tcp::resolver::query query(tcp::v4(), "127.0.0.1", std::to_string(port));
    tcp::resolver::iterator iterator = resolver_.resolve(query);
    socket_.connect(*iterator);
}

void Client::SendMessage(
        const Requests& aRequestType,
        const std::string& aUserName,
        const std::string& aPassword ) {

    nlohmann::json req;
    req["ReqType"] = aRequestType;
    req["UserName"] = aUserName;
    req["Password"] = aPassword;

    std::string request = req.dump();
    boost::asio::write(socket_, boost::asio::buffer(request, request.size()));
}

void Client::SendRequest(
        const Requests& aRequestType,
        const std::string& aUserName ) {

    nlohmann::json req;
    req["ReqType"] = aRequestType;
    req["UserName"] = aUserName;

    std::string request = req.dump();
    boost::asio::write(socket_, boost::asio::buffer(request, request.size()));
}

std::vector<std::string> Client::GetActiveUserTrades(const std::string& aUserName,
                                             const Requests& aRequestType) {
    nlohmann::json req;
    req["ReqType"] = aRequestType;
    req["UserName"] = aUserName;

    std::string request = req.dump();
    boost::asio::write(socket_, boost::asio::buffer(request, request.size()));
    std::vector<std::string> result;
    for (const auto& str: ReadMessages()) {
        result.push_back(move(str));
    }
    return result;
}

std::vector<std::string> Client::GetCompletedTradesList(const std::string& aUserName,
                                                const Requests& aRequestType) {

    nlohmann::json req;
    req["ReqType"] = aRequestType;
    req["UserName"] = aUserName;

    std::string request = req.dump();
    boost::asio::write(socket_, boost::asio::buffer(request, request.size()));
    std::vector<std::string> result;
    for (const auto& str: ReadMessages()) {
        result.push_back(move(str));
    }
    return result;
}

std::string Client::SendTradeRequest(
        const std::string& aUserName,
        const Requests& aRequestType,
        const int sum,
        const int cost ) {

    nlohmann::json req;
    req["UserName"] = aUserName;
    req["Sum"] = sum;
    req["Cost"] = cost;
    req["ReqType"] = aRequestType;

    std::string request = req.dump();
    boost::asio::write(socket_, boost::asio::buffer(request, request.size()));
    return ReadMessage();
}

std::vector<std::string> Client::ReadMessages() {
    boost::asio::streambuf b;
    boost::asio::read_until(socket_, b, "\n");
    std::istream is(&b);
    std::vector<std::string> result;
    if (is) {
        std::string str;
        while (is >> str) {
            result.push_back(str);
        }
    }
    is.clear();

    return move(result);
}

std::string Client::ReadMessage() {
    boost::asio::streambuf b;
    boost::asio::read_until(socket_, b, "\0");
    std::istream is(&b);
    std::string line(std::istreambuf_iterator<char>(is), {});
    return line;
}

std::string Client::ProcessRegistration(Requests req) {
    system("clear");
    std::string RegResult = "";
    short int counter = 0;
    std::string login;
    while (RegResult == "") {
        if (counter++ != 0) {
            std::cout << "Send \"exit\" instead a UserName to exit to the main menu" << std::endl;
        }

        std::cout << "Enter your user name: ";
        std::cin >> login;
        if (login == "exit") {
            return "";
        }

        std::cout << "Enter your password: ";
        std::string psw;
        std::cin >> psw;

        SendMessage(req, login, psw);
        std::string message = ReadMessage();
        RegResult = message[0] == 'U' || message[0] == 'A' ? "" : message;
        system("clear");
        std::cout << message << std::endl;
    }

    user_Name_ = login;
    return RegResult;
}

std::string Client::GetBalance() {
    SendRequest(Requests::BALANCE, user_Name_);
    return ReadMessage();
}

std::string Client::GetUserName() const {
    return user_Name_;
}