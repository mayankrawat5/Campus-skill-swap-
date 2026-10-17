#ifndef EXCHANGE_REQUEST_H
#define EXCHANGE_REQUEST_H

#include <string>

class ExchangeRequest {
private:
    std::string requestID;
    std::string senderID;
    std::string receiverID;
    std::string skillWanted; // Skill sender wants to learn
    std::string skillOffered; // Skill sender offers to teach
    std::string status; // Pending, Accepted, Rejected, Cancelled, Completed

    // Static counter for deterministic IDs
    static int nextRequestId;

public:
    ExchangeRequest();
    ExchangeRequest(std::string sender, std::string receiver, std::string wanted, std::string offered);

    // Getters
    std::string getID() const;
    std::string getSenderID() const;
    std::string getReceiverID() const;
    std::string getSkillWanted() const;
    std::string getSkillOffered() const;
    std::string getStatus() const;

    // Setters
    void setStatus(std::string newStatus);

    void display() const;
};

#endif // EXCHANGE_REQUEST_H