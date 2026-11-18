#include "admin.h"
#include <iostream>
#include <algorithm>
#include <map>
#include <iomanip>

// Global data declarations (defined in campus_system.cpp)
extern std::vector<Student> students;
extern std::map<std::string, int> studentIDToIndex;
extern std::vector<ExchangeRequest> exchangeRequests;
extern std::vector<ScheduledExchange> scheduledExchanges;

void adminMenu() {
    std::string adminID;
    std::cout << "\n=== ADMIN MENU ===" << std::endl;

    std::cout << "Enter Admin ID (use 'admin001' for demo): ";
    std::getline(std::cin, adminID);

    // Simple admin check - in reality this would be more secure
    if (adminID != "admin001") {
        std::cout << "Error: Invalid Admin ID!" << std::endl;
        return;
    }

    while (true) {
        std::cout << "\n--- ADMIN MENU ---" << std::endl;
        std::cout << "1. View all students" << std::endl;
        std::cout << "2. View all skills" << std::endl;
        std::cout << "3. View all exchange requests" << std::endl;
        std::cout << "4. View all scheduled exchanges" << std::endl;
        std::cout << "5. View reports/feedback" << std::endl;
        std::cout << "6. Remove a student" << std::endl;
        std::cout << "7. Display system statistics" << std::endl;
        std::cout << "8. Go back" << std::endl;
        std::cout << "Enter choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) {
            viewAllStudents();
        } else if (choice == 2) {
            viewAllSkills();
        } else if (choice == 3) {
            viewAllExchangeRequests();
        } else if (choice == 4) {
            viewAllScheduledExchanges();
        } else if (choice == 5) {
            viewReportsFeedback();
        } else if (choice == 6) {
            std::string studentID;
            std::cout << "Enter Student ID to remove: ";
            std::getline(std::cin, studentID);
            removeStudent(studentID);
        } else if (choice == 7) {
            displaySystemStatistics();
        } else if (choice == 8) {
            break;
        }
    }
}

void viewAllStudents() {
    std::cout << "\n=== ALL STUDENTS ===" << std::endl;
    for (size_t i = 0; i < students.size(); ++i) {
        std::cout << i + 1 << ". ";
        students[i].displayBrief();
    }
}

void viewAllSkills() {
    std::cout << "\n=== ALL SKILLS IN SYSTEM ===" << std::endl;
    std::map<std::string, int> skillCount;

    for (const Student& s : students) {
        for (const SkillInfo& skill : s.getSkillsCanTeach()) {
            skillCount[skill.skillName]++;
        }
        for (const SkillInfo& skill : s.getSkillsWantToLearn()) {
            skillCount[skill.skillName]++;
        }
    }

    for (const auto& pair : skillCount) {
        std::cout << "- " << pair.first << ": " << pair.second << " occurrences" << std::endl;
    }
}

void viewAllExchangeRequests() {
    std::cout << "\n=== ALL EXCHANGE REQUESTS ===" << std::endl;
    if (exchangeRequests.empty()) {
        std::cout << "No exchange requests." << std::endl;
    } else {
        for (size_t i = 0; i < exchangeRequests.size(); ++i) {
            std::cout << i + 1 << "." << std::endl;
            exchangeRequests[i].display();
            std::cout << std::endl;
        }
    }
}

void viewAllScheduledExchanges() {
    std::cout << "\n=== ALL SCHEDULED EXCHANGES ===" << std::endl;
    if (scheduledExchanges.empty()) {
        std::cout << "No scheduled exchanges." << std::endl;
    } else {
        for (size_t i = 0; i < scheduledExchanges.size(); ++i) {
            std::cout << i + 1 << "." << std::endl;
            scheduledExchanges[i].display();
            std::cout << std::endl;
        }
    }
}

void viewReportsFeedback() {
    std::cout << "\n=== REPORTS AND FEEDBACK ===" << std::endl;
    std::cout << "Total students: " << students.size() << std::endl;
    std::cout << "Total exchange requests: " << exchangeRequests.size() << std::endl;
    std::cout << "Total scheduled exchanges: " << scheduledExchanges.size() << std::endl;

    double totalRating = 0.0;
    int ratedStudents = 0;
    for (const Student& s : students) {
        if (s.getNumRatings() > 0) {
            totalRating += s.getRating();
            ratedStudents++;
        }
    }

    if (ratedStudents > 0) {
        std::cout << "Average rating across all students: "
                 << std::fixed << std::setprecision(2)
                 << (totalRating / ratedStudents) << std::endl;
    } else {
        std::cout << "No ratings yet." << std::endl;
    }
}

void removeStudent(std::string studentID) {
    int index = findStudentIndexByID(studentID);
    if (index == -1) {
        std::cout << "Error: Student not found!" << std::endl;
        return;
    }

    // Remove from vector and update map
    students.erase(students.begin() + index);

    // Rebuild the map (simple approach)
    studentIDToIndex.clear();
    for (size_t i = 0; i < students.size(); ++i) {
        studentIDToIndex[students[i].getID()] = i;
    }

    std::cout << "Student removed successfully!" << std::endl;
}

void displaySystemStatistics() {
    std::cout << "\n=== SYSTEM STATISTICS ===" << std::endl;
    std::cout << "Total Students: " << students.size() << std::endl;

    int totalTeachSkills = 0;
    int totalLearnSkills = 0;
    for (const Student& s : students) {
        totalTeachSkills += s.getSkillsCanTeach().size();
        totalLearnSkills += s.getSkillsWantToLearn().size();
    }
    std::cout << "Total Teach Skills: " << totalTeachSkills << std::endl;
    std::cout << "Total Learn Skills: " << totalLearnSkills << std::endl;

    int pendingRequests = 0;
    int acceptedRequests = 0;
    int completedRequests = 0;
    for (const ExchangeRequest& req : exchangeRequests) {
        if (req.getStatus() == "Pending") pendingRequests++;
        else if (req.getStatus() == "Accepted") acceptedRequests++;
        else if (req.getStatus() == "Completed") completedRequests++;
    }
    std::cout << "Pending Requests: " << pendingRequests << std::endl;
    std::cout << "Accepted Requests: " << acceptedRequests << std::endl;
    std::cout << "Completed Requests: " << completedRequests << std::endl;

    std::cout << "Scheduled Exchanges: " << scheduledExchanges.size() << std::endl;

    int totalCompletedExchanges = 0;
    for (const Student& s : students) {
        totalCompletedExchanges += s.getCompletedExchanges();
    }
    std::cout << "Total Completed Exchanges: " << totalCompletedExchanges << std::endl;
}