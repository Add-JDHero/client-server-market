#include "Client_class.h"
#include "Menu_func.h"


void IncorrectCin(std::istream& cin) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void PrintMenu() {
    system("clear");
    std::cout <<
              "\tMenu\n"
              "1) Registration\n"
              "2) Sign in\n"
              << std::endl;
}

void PrintSecondMenu() {
    system("clear");
    std::cout<<
             "1) Go to profile\n"
             "2) Go to currency market\n"
             "3) Exit to start menu\n"
             << std::endl;
}

void PrintProfileInformation(Client& client) {
    while (true) {
        system("clear");
        std::cout<< "\t" << client.GetUserName() << std::endl << std::endl;
        std::cout << "Balance: " << client.GetBalance() <<std::endl << std::endl;
        std:: cout << "Completed trades: " << std::endl;
        std::vector<std::string> completed_trades = client.GetCompletedTradesList(client.GetUserName(), Requests::COMPLETEDTRADES);
        if (!completed_trades.empty()) {
            for (const auto& trade_deal: completed_trades) {
                std::cout << "\t" << trade_deal << std::endl;
            }
        } else {
            std::cout << "empty list" << std::endl;
        }
        std::cout << std::endl;

        std::cout << "Send \"exit\" to exit to the main menu" << std::endl << std::endl;

        std::string str;
        std::cin >> str;
        if (std::cin) {
            if (str == "exit") {
                break;
            }
        } else {
            IncorrectCin(std::cin);
        }
        if (str == "exit") {
            break;
        }
    }
}

void PrintCurrencyMarketMenu(Client& client) {
    while (true) {
        system("clear");
        std::cout << "1) view all active requests or \n"
                     "2) make a request\n";

        short your_answer;
        std::cin >> your_answer;
        if (std::cin) {
            switch (your_answer) {
                case 1: {
                    std::cout << "\n" << "active trades:\n";
                    std::vector<std::string> active_trades = client.GetActiveUserTrades(client.GetUserName(), Requests::ACTIVEUSERTRADES);
                    if (!active_trades.empty()) {
                        for (const auto& trade_deal: active_trades) {
                            std::cout << "\t" << trade_deal << std::endl;
                        }
                    } else {
                        std::cout << "empty list" << std::endl;
                    }
                    break;
                }
                case 2: {
                    std::string req;
                    int sum;
                    int cost;
                    Requests request;
                    std::cout << "Write ReqType: BUY/SELL" << std::endl;
                    std::cin >> req;
                    if (req == "BUY" || req == "SELL") {
                        std::cout << "Write Sum (usd)" << std::endl;
                        std::cin >> sum;
                        if (!std::cin) {
                            IncorrectCin(std::cin);
                            break;
                        }

                        std::cout << "Write Cost (rub)" << std::endl;
                        std::cin >> cost;
                        if (!std::cin) {
                            IncorrectCin(std::cin);
                            break;
                        }
                        std::cout << std::endl << std::endl;
                    } else {
                        IncorrectCin(std::cin);
                        break;
                    }

                    if (req == "BUY") {
                        request = Requests::BUY;
                    } else {
                        request = Requests::SELL;
                    }
                    std::cout << client.SendTradeRequest(client.GetUserName(), request, sum, cost) << std::endl;
                }
                default: {
                    break;
                }
            }
            std::cout << std::endl << std::endl;
            std::cout << "Write \"exit\" to return to the second menu or any key to continue" << std::endl;
            std::string str;
            std::cin >> str;
            if (str == "exit") {
                break;
            } else {
                continue;
            }

        } else {
            IncorrectCin(std::cin);
        }
    }
}

void SecondMenu(Client& client) {
    while (true) {
        PrintSecondMenu();

        short menu_option_num;
        std::cin >> menu_option_num;
        if (std::cin) {
            switch (menu_option_num) {
                case 1: {
                    PrintProfileInformation(client);
                    break;
                }
                case 2: {
                    PrintCurrencyMarketMenu(client);
                    break;
                }
                case 3: {
                    return;
                }
                default: {
                    std::cout << "Unknown menu option\n" << std::endl;
                    break;
                }
            }
        } else {
            IncorrectCin(std::cin);
        }
    }
}