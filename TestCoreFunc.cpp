#include <iostream>

#include "source/include/Server/Core.h"
#include "source/include/Server/Common.hpp"

void TestRegistration(Core& core) {
    core.RegisterNewUser("abc", "abc");

    assert(core.GetUsersList().size() == 1);
    assert(core.GetUsersList().begin()->first == "abc");
    assert(core.GetUsersList().begin()->second == "abc");

    std::cout << "TestRegistration OK" << std::endl;
}

void TestHandleRequest(Core& core) {
    {
        core.HandleRequest("abc", Requests::BUY, 10, 62);

        assert(core.GetActiveTrades("abc")[0] != "empty_list");
    } // add first trade request

    {
        core.RegisterNewUser("cba", "cba");
        core.HandleRequest("cba", Requests::BUY, 20, 63);

        assert(core.GetActiveTrades("cba")[0] != "empty_list");

    } // add second trade request

    {
        core.RegisterNewUser("a", "a");
        core.HandleRequest("a", Requests::SELL, 50, 61);

        assert(core.GetActiveTrades("a")[0] != "empty_list");
        assert(core.GetActiveTrades("abc")[0] == "empty_list\n");
        assert(core.GetActiveTrades("cba")[0] == "empty_list\n");
        assert(core.GetActiveTrades("a").size() == 1);

        assert(core.GetCompletedTradesList("a").size() == 2);
    } // add third trade request and make a deal

    {
        core.HandleRequest("abc", Requests::BUY, 21, 62);

        assert(core.GetActiveTrades("abc").size() == 1);
    }


    std::cout << "TestHandleRequest OK" << std::endl;
}

void TestAll(Core& core) {
    TestRegistration(core);
    TestHandleRequest(core);

    std::cout << "Tests OK" << std::endl;
}

int main() {
    static Core core;
    TestAll(core);
}
