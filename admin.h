#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include <vector>
#include "student.h"
#include "exchange_request.h"
#include "scheduler.h"

// Forward declarations
class Student;
class ExchangeRequest;
class ScheduledExchange;

// Function declarations
int findStudentIndexByID(const std::string& id);

void adminMenu();
void viewAllStudents();
void viewAllSkills();
void viewAllExchangeRequests();
void viewAllScheduledExchanges();
void viewReportsFeedback();
void removeStudent(std::string studentID);
void displaySystemStatistics();

#endif // ADMIN_H
