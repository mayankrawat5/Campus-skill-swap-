#include "exchange_request.h"
#include <iostream>
#include <cassert>

void testExchangeRequestIDGeneration() {
    ExchangeRequest req1;
    ExchangeRequest req2("S001", "S002", "Python", "Java");

    // Test that IDs are sequential and follow REQ1, REQ2, etc.
    assert(req1.getID().substr(0, 3) == "REQ");
    assert(req2.getID().substr(0, 3) == "REQ");

    // Extract the numeric part
    int num1 = std::stoi(req1.getID().substr(3));
    int num2 = std::stoi(req2.getID().substr(3));

    // They should be sequential
    assert(num2 == num1 + 1);

    std::cout << "Exchange request ID generation test passed!" << std::endl;
    std::cout << "Req1 ID: " << req1.getID() << std::endl;
    std::cout << "Req2 ID: " << req2.getID() << std::endl;
}

void testExchangeRequestStatus() {
    ExchangeRequest req;
    assert(req.getStatus() == "Pending");

    req.setStatus("Accepted");
    assert(req.getStatus() == "Accepted");

    req.setStatus("Completed");
    assert(req.getStatus() == "Completed");

    std::cout << "Exchange request status test passed!" << std::endl;
}

int main() {
    testExchangeRequestIDGeneration();
    testExchangeRequestStatus();
    std::cout << "All exchange request tests passed!" << std::endl;
    return 0;
}