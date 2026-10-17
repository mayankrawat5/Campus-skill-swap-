#include "scheduler.h"
#include <iostream>
#include <cassert>

void testScheduledExchangeIDGeneration() {
    ScheduledExchange exch1;
    ScheduledExchange exch2("S001", "S002", "Python", "2026-10-08", "14:00", "Online", "Lab 1");

    // Test that IDs are sequential and follow EX1, EX2, etc.
    assert(exch1.getID().substr(0, 2) == "EX");
    assert(exch2.getID().substr(0, 2) == "EX");

    // Extract the numeric part
    int num1 = std::stoi(exch1.getID().substr(2));
    int num2 = std::stoi(exch2.getID().substr(2));

    // They should be sequential
    assert(num2 == num1 + 1);

    std::cout << "Scheduled exchange ID generation test passed!" << std::endl;
    std::cout << "Exch1 ID: " << exch1.getID() << std::endl;
    std::cout << "Exch2 ID: " << exch2.getID() << std::endl;
}

void testScheduledExchangeStatus() {
    ScheduledExchange exch;
    assert(exch.getStatus() == "Scheduled");

    exch.setStatus("Completed");
    assert(exch.getStatus() == "Completed");

    exch.setStatus("Cancelled");
    assert(exch.getStatus() == "Cancelled");

    std::cout << "Scheduled exchange status test passed!" << std::endl;
}

int main() {
    testScheduledExchangeIDGeneration();
    testScheduledExchangeStatus();
    std::cout << "All scheduled exchange tests passed!" << std::endl;
    return 0;
}