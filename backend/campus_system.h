#ifndef CAMPUS_SYSTEM_H
#define CAMPUS_SYSTEM_H

#include <string>
#include <vector>
#include <map>
#include "student.h"
#include "exchange_request.h"
#include "scheduler.h"
#include "skill_graph.h"

// Global data declarations
extern std::vector<Student> students;
extern std::map<std::string, int> studentIDToIndex;
extern std::vector<ExchangeRequest> exchangeRequests;
extern std::vector<ScheduledExchange> scheduledExchanges;
extern SkillGraph skillGraph;

// Helper functions declarations
int findStudentIndexByID(const std::string& id);
Student* findStudentByID(const std::string& id);
bool isValidStudentID(const std::string& id);

// Module implementations
void studentRegistration();
void studentLogin();
void findSkillMatches();
void viewCompatibilityRanking();
void sendExchangeRequest();
void manageRequests();
void scheduleExchange();
void viewSkillHistory();
void searchStudents();
void rateFeedback();

// Sample data initialization
void initializeSampleData();

// Main function
void runCampusSkillSwapNetwork();

#endif // CAMPUS_SYSTEM_H
