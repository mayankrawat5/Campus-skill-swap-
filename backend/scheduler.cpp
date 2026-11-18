#include "scheduler.h"
#include <iostream>

// Initialize static counter
int ScheduledExchange::nextExchangeId = 1;

ScheduledExchange::ScheduledExchange() : exchangeID("EX" + std::to_string(nextExchangeId++)),
                                       status("Scheduled") {}

ScheduledExchange::ScheduledExchange(std::string s1, std::string s2, std::string sk, std::string dt, std::string tm,
                                    std::string md, std::string loc)
    : student1ID(s1), student2ID(s2), skill(sk), date(dt), time(tm),
      mode(md), location(loc), status("Scheduled") {
    exchangeID = "EX" + std::to_string(nextExchangeId++);
}

// Getters
std::string ScheduledExchange::getID() const { return exchangeID; }
std::string ScheduledExchange::getStudent1ID() const { return student1ID; }
std::string ScheduledExchange::getStudent2ID() const { return student2ID; }
std::string ScheduledExchange::getSkill() const { return skill; }
std::string ScheduledExchange::getDate() const { return date; }
std::string ScheduledExchange::getTime() const { return time; }
std::string ScheduledExchange::getMode() const { return mode; }
std::string ScheduledExchange::getLocation() const { return location; }
std::string ScheduledExchange::getStatus() const { return status; }

// Setters
void ScheduledExchange::setStatus(std::string newStatus) { status = newStatus; }

void ScheduledExchange::display() const {
    std::cout << "Exchange ID: " << exchangeID << std::endl;
    std::cout << "Between: " << student1ID << " and " << student2ID << std::endl;
    std::cout << "Skill: " << skill << std::endl;
    std::cout << "Date: " << date << " Time: " << time << std::endl;
    std::cout << "Mode: " << mode << " Location: " << location << std::endl;
    std::cout << "Status: " << status << std::endl;
}