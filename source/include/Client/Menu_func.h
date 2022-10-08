#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include "Client_class.h"

void IncorrectCin(std::istream& cin);

void PrintMenu();

void PrintSecondMenu();

void PrintProfileInformation(Client& client);

void PrintCurrencyMarketMenu(Client& client);

void SecondMenu(Client& client);

