#pragma once
#include <boost/asio.hpp>

void SystemWait(boost::asio::io_service& io_service, int seconds);
