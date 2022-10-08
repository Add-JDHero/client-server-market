#ifndef CLIENSERVERECN_COMMON_HPP
#define CLIENSERVERECN_COMMON_HPP

#include <string>

static short port = 5555;

enum class  Requests {
    REGISTRATION,
    SIGNIN,
    BUY,
    SELL,
    BALANCE,
    COMPLETEDTRADES,
    ACTIVEUSERTRADES,
};

#endif //CLIENSERVERECN_COMMON_HPP
