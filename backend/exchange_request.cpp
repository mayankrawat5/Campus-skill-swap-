#include "exchange_request.h"
#include <iostream>

// Initialize static counter
int ExchangeRequest::nextRequestId = 1;

ExchangeRequest::ExchangeRequest() : requestID("REQ" + std::to_string(nextRequestId++)),
                                   status("Pending") {}

ExchangeRequest::ExchangeRequest(std::string sender, std::string receiver, std::string wanted, std::string offered)
    : senderID(sender), receiverID(receiver), skillWanted(wanted),
      skillOffered(offered), status("Pending") {
    requestID = "REQ" + std::to_string(nextRequestId++);
}

// Getters
std::string ExchangeRequest::getID() const { return requestID; }
std::string ExchangeRequest::getSenderID() const { return senderID; }
std::string ExchangeRequest::getReceiverID() const { return receiverID; }
std::string ExchangeRequest::getSkillWanted() const { return skillWanted; }
std::string ExchangeRequest::getSkillOffered() const { return skillOffered; }
std::string ExchangeRequest::getStatus() const { return status; }

// Setters
void ExchangeRequest::setStatus(std::string newStatus) { status = newStatus; }

void ExchangeRequest::display() const {
    std::cout << "Request ID: " << requestID << std::endl;
    std::cout << "From: " << senderID << " To: " << receiverID << std::endl;
    std::cout << "Wants to learn: " << skillWanted << std::endl;
    std::cout << "Offers to teach: " << skillOffered << std::endl;
    std::cout << "Status: " << status << std::endl;
}