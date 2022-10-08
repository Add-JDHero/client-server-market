#include "Timer.h"

void SystemWait(boost::asio::io_service& io_service, int seconds) {
    boost::asio::steady_timer timer(io_service);
    timer.expires_after(std::chrono::seconds(seconds));
    timer.wait();
}
