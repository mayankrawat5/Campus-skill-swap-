#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <string>

class ScheduledExchange {
private:
    std::string exchangeID;
    std::string student1ID;
    std::string student2ID;
    std::string skill;
    std::string date;
    std::string time;
    std::string mode;
    std::string location;
    std::string status; // Scheduled, Completed, Cancelled

    // Static counter for deterministic IDs
    static int nextExchangeId;

public:
    ScheduledExchange();
    ScheduledExchange(std::string s1, std::string s2, std::string sk, std::string dt, std::string tm,
                     std::string md, std::string loc);

    // Getters
    std::string getID() const;
    std::string getStudent1ID() const;
    std::string getStudent2ID() const;
    std::string getSkill() const;
    std::string getDate() const;
    std::string getTime() const;
    std::string getMode() const;
    std::string getLocation() const;
    std::string getStatus() const;

    // Setters
    void setStatus(std::string newStatus);

    void display() const;
};

#endif // SCHEDULER_H