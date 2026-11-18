#ifndef MATCHING_ENGINE_H
#define MATCHING_ENGINE_H

#include <string>
#include <vector>
#include "student.h"

// Forward declaration
class Student;

// Compatibility Score Calculation
int calculateCompatibilityScore(const Student& student1, const Student& student2);

// Function to find skill matches
void findSkillMatches(const std::string& currentStudentID);

// Function to view compatibility ranking
void viewCompatibilityRanking(const std::string& currentStudentID);

#endif // MATCHING_ENGINE_H