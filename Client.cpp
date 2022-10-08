#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using std::string_literals::operator""s;

#include "Client_class.h"
#include "Menu_func.h"
#include "Timer.h"


int main() {
    try {
        boost::asio::io_service io_service;
        Client client(io_service);

        while (true) {
            PrintMenu();

            short menu_option_num;
            std::cin >> menu_option_num;
            if (std::cin) {
                switch (menu_option_num) {
                    case 1: {
                        std::string str = client.ProcessRegistration(Requests::REGISTRATION);
                        if (str.empty()) {
                            break;
                        }
                        SystemWait(io_service, 1);
                        SecondMenu(client);
                        break;
                    }
                    case 2: {
                        std::string str = client.ProcessRegistration(Requests::SIGNIN);
                        if (str.empty()) {
                            break;
                        }
                        SystemWait(io_service, 1);
                        SecondMenu(client);
                        break;
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
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}