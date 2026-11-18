#include "campus_system.h"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <cctype>
#include <limits>
#include <iomanip>
#include <sstream>
#include <queue>

// Forward declaration for adminMenu
void adminMenu();

// Global data definitions
std::vector<Student> students;
std::map<std::string, int> studentIDToIndex;
std::vector<ExchangeRequest> exchangeRequests;
std::vector<ScheduledExchange> scheduledExchanges;
SkillGraph skillGraph;

// Helper functions implementations
int findStudentIndexByID(const std::string& id) {
    if (studentIDToIndex.find(id) != studentIDToIndex.end()) {
        return studentIDToIndex[id];
    }
    return -1;
}

Student* findStudentByID(const std::string& id) {
    int index = findStudentIndexByID(id);
    if (index != -1) {
        return &students[index];
    }
    return nullptr;
}

bool isValidStudentID(const std::string& id) {
    return findStudentIndexByID(id) == -1; // Returns true if ID is unique
}

// Compatibility Score Calculation
int calculateCompatibilityScore(const Student& student1, const Student& student2) {
    int score = 0;

    // Skill Compatibility (40 points)
    int skillScore = 0;
    bool hasTeachLearnMatch = false;
    bool hasLearnTeachMatch = false;

    // Check if student1 can teach what student2 wants to learn
    for (const SkillInfo& teach1 : student1.getSkillsCanTeach()) {
        for (const SkillInfo& learn2 : student2.getSkillsWantToLearn()) {
            if (teach1.skillName == learn2.skillName) {
                // Bonus for matching levels
                if (teach1.level == learn2.level ||
                    (teach1.level == "Intermediate" && learn2.level == "Beginner") ||
                    (teach1.level == "Advanced" && (learn2.level == "Intermediate" || learn2.level == "Beginner"))) {
                    skillScore += 20;
                } else {
                    skillScore += 10;
                }
                hasTeachLearnMatch = true;
                break;
            }
        }
        if (hasTeachLearnMatch) break;
    }

    // Check if student2 can teach what student1 wants to learn
    for (const SkillInfo& teach2 : student2.getSkillsCanTeach()) {
        for (const SkillInfo& learn1 : student1.getSkillsWantToLearn()) {
            if (teach2.skillName == learn1.skillName) {
                // Bonus for matching levels
                if (teach2.level == learn1.level ||
                    (teach2.level == "Intermediate" && learn1.level == "Beginner") ||
                    (teach2.level == "Advanced" && (learn1.level == "Intermediate" || learn1.level == "Beginner"))) {
                    skillScore += 20;
                } else {
                    skillScore += 10;
                }
                hasLearnTeachMatch = true;
                break;
            }
        }
        if (hasLearnTeachMatch) break;
    }

    // If both directions match, full skill points
    if (hasTeachLearnMatch && hasLearnTeachMatch) {
        skillScore = std::min(40, skillScore + 20); // Boost to max 40
    } else if (hasTeachLearnMatch || hasLearnTeachMatch) {
        skillScore = std::min(20, skillScore); // Half points for one-way match
    }

    score += skillScore;

    // Mode Compatibility (20 points)
    std::string mode1 = student1.getPreferredMode();
    std::string mode2 = student2.getPreferredMode();

    if (mode1 == mode2) {
        score += 20;
    } else if ((mode1 == "Both" || mode2 == "Both") &&
               (mode1 != "Both" || mode2 != "Both")) {
        score += 10;
    }
    // Else 0 points

    // Location Compatibility (15 points)
    // For offline or both modes, locations should match
    bool needsOffline = (mode1 == "Offline" || mode1 == "Both" ||
                         mode2 == "Offline" || mode2 == "Both");

    if (needsOffline) {
        if (student1.getLocation() == student2.getLocation()) {
            score += 15;
        }
        // Else 0 points for location mismatch when offline needed
    } else {
        // Both online only - location doesn't matter
        score += 15;
    }

    // Availability Compatibility (15 points)
    std::vector<std::string> avail1 = student1.getAvailability();
    std::vector<std::string> avail2 = student2.getAvailability();

    bool availabilityMatch = false;
    for (const std::string& a1 : avail1) {
        for (const std::string& a2 : avail2) {
            if (a1 == a2) {
                availabilityMatch = true;
                break;
            }
        }
        if (availabilityMatch) break;
    }

    if (availabilityMatch) {
        score += 15;
    }
    // If either has empty availability, we'll be lenient and give partial points
    else if (avail1.empty() || avail2.empty()) {
        score += 7; // Half points
    }

    // Rating Compatibility (10 points)
    double rating1 = student1.getRating();
    double rating2 = student2.getRating();
    double avgRating = (rating1 + rating2) / 2.0;

    // Convert rating (1-5) to points (0-10)
    int ratingPoints = static_cast<int>((avgRating - 1.0) / 4.0 * 10.0 + 0.5);
    ratingPoints = std::max(0, std::min(10, ratingPoints)); // Clamp to 0-10
    score += ratingPoints;

    return std::min(100, std::max(0, score)); // Ensure score is between 0-100
}

// Function to find skill matches
void findSkillMatches(const std::string& currentStudentID) {
    std::cout << "\n=== FIND SKILL MATCHES ===" << std::endl;

    Student* currentStudent = findStudentByID(currentStudentID);
    if (!currentStudent) {
        std::cout << "Error: Student not found!" << std::endl;
        return;
    }

    std::vector<std::pair<int, Student*>> matches; // Compatibility score, student pointer

    for (Student& otherStudent : students) {
        if (otherStudent.getID() == currentStudentID) continue; // Skip self

        int score = calculateCompatibilityScore(*currentStudent, otherStudent);
        if (score > 0) { // Only show matches with some compatibility
            matches.push_back({score, &otherStudent});
        }
    }

    // Sort by compatibility score descending
    std::sort(matches.begin(), matches.end(),
              [](const std::pair<int, Student*>& a, const std::pair<int, Student*>& b) {
                  return a.first > b.first;
              });

    if (matches.empty()) {
        std::cout << "No matches found." << std::endl;
        return;
    }

    std::cout << "\n=== TOP MATCHES ===" << std::endl;
    for (size_t i = 0; i < matches.size() && i < 5; ++i) { // Show top 5
        std::cout << i + 1 << ". ";
        matches[i].second->displayBrief();

        // Show what skills match
        std::cout << "   Skill Match: ";
        bool foundMatch = false;
        for (const SkillInfo& teach : currentStudent->getSkillsCanTeach()) {
            for (const SkillInfo& learn : matches[i].second->getSkillsWantToLearn()) {
                if (teach.skillName == learn.skillName) {
                    std::cout << teach.skillName;
                    foundMatch = true;
                    break;
                }
            }
            if (foundMatch) break;
        }
        if (!foundMatch) {
            for (const SkillInfo& teach : matches[i].second->getSkillsCanTeach()) {
                for (const SkillInfo& learn : currentStudent->getSkillsWantToLearn()) {
                    if (teach.skillName == learn.skillName) {
                        std::cout << teach.skillName;
                        foundMatch = true;
                        break;
                    }
                }
                if (foundMatch) break;
            }
        }
        if (!foundMatch) {
            std::cout << "Multiple skills";
        }
        std::cout << std::endl;

        std::cout << "   Compatibility: " << matches[i].first << "%" << std::endl;
        std::cout << std::endl;
    }
}

// Function to view compatibility ranking
void viewCompatibilityRanking(const std::string& currentStudentID) {
    std::cout << "\n=== VIEW COMPATIBILITY RANKING ===" << std::endl;

    Student* currentStudent = findStudentByID(currentStudentID);
    if (!currentStudent) {
        std::cout << "Error: Student not found!" << std::endl;
        return;
    }

    // This is similar to findSkillMatches but we'll show all matches ranked
    std::vector<std::pair<int, Student*>> matches;

    for (Student& otherStudent : students) {
        if (otherStudent.getID() == currentStudentID) continue;

        int score = calculateCompatibilityScore(*currentStudent, otherStudent);
        matches.push_back({score, &otherStudent});
    }

    // Sort by compatibility score descending
    std::sort(matches.begin(), matches.end(),
              [](const std::pair<int, Student*>& a, const std::pair<int, Student*>& b) {
                  return a.first > b.first;
              });

    std::cout << "\n=== COMPATIBILITY RANKING FOR " << currentStudent->getName() << " ===" << std::endl;
    std::cout << std::left << std::setw(4) << "No." << std::setw(15) << "Name"
              << std::setw(12) << "Dept" << std::setw(6) << "Year"
              << std::setw(12) << "Skill Match" << "Compatibility" << std::endl;
    std::cout << std::string(65, '-') << std::endl;

    size_t count = 0;
    for (const auto& match : matches) {
        if (count >= 10) break; // Show top 10
        if (match.first == 0) break; // Stop at zero compatibility

        std::cout << std::left << std::setw(4) << (count + 1)
                  << std::setw(15) << match.second->getName().substr(0, 14)
                  << std::setw(12) << match.second->getDepartment().substr(0, 11)
                  << std::setw(6) << match.second->getYear()
                  << std::setw(12) << "Skills" // Simplified
                  << match.first << "%" << std::endl;
        count++;
    }

    if (count == 0) {
        std::cout << "No matches found." << std::endl;
    }
}

// Module implementations
void studentRegistration() {
    std::cout << "\n=== STUDENT REGISTRATION ===" << std::endl;

    std::string id, name, dept;
    int year;

    std::cout << "Enter Student ID: ";
    std::getline(std::cin, id);

    if (!isValidStudentID(id)) {
        std::cout << "Error: Student ID already exists!" << std::endl;
        return;
    }

    std::cout << "Enter Name: ";
    std::getline(std::cin, name);

    std::cout << "Enter Department: ";
    std::getline(std::cin, dept);

    std::cout << "Enter Year (1-4): ";
    while (!(std::cin >> year) || year < 1 || year > 4) {
        std::cout << "Invalid year. Enter 1-4: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(); // Clear newline

    // Create student
    Student newStudent(id, name, dept, year);

    // Set default preferences
    newStudent.setPreferredMode("Both");
    newStudent.setLocation("Campus");
    newStudent.setAvailability({"Morning", "Afternoon", "Evening"});

    // Add to collections
    students.push_back(newStudent);
    studentIDToIndex[id] = students.size() - 1;

    std::cout << "\nRegistration successful!" << std::endl;
    std::cout << "Welcome, " << name << "!" << std::endl;
}

void studentLogin() {
    std::cout << "\n=== STUDENT LOGIN ===" << std::endl;

    std::string id;
    std::cout << "Enter Student ID: ";
    std::getline(std::cin, id);

    Student* student = findStudentByID(id);
    if (!student) {
        std::cout << "Error: Student not found!" << std::endl;
        return;
    }

    std::cout << "\nLogin successful! Welcome, " << student->getName() << "!" << std::endl;

    // Variables for profile updates
    std::string name;
    std::string dept;
    int year;

    // Student menu
    while (true) {
        std::cout << "\n--- STUDENT MENU ---" << std::endl;
        std::cout << "1. View Profile" << std::endl;
        std::cout << "2. Manage Profile" << std::endl;
        std::cout << "3. Manage Skills" << std::endl;
        std::cout << "4. Update Preferences" << std::endl;
        std::cout << "5. View Skill History" << std::endl;
        std::cout << "6. View Notifications" << std::endl;
        std::cout << "7. Logout" << std::endl;
        std::cout << "Enter choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Clear newline

        if (choice == 1) {
            student->displayFull();
        } else if (choice == 2) {
            std::cout << "\nUpdate profile:" << std::endl;
            std::cout << "1. Change name" << std::endl;
            std::cout << "2. Change department" << std::endl;
            std::cout << "3. Change year" << std::endl;
            std::cout << "4. Go back" << std::endl;
            std::cout << "Enter choice: ";
            int subChoice;
            std::cin >> subChoice;
            std::cin.ignore();

            if (subChoice == 1) {
                std::cout << "Enter new name: ";
                std::getline(std::cin, name);
                student->setName(name);
            } else if (subChoice == 2) {
                std::cout << "Enter new department: ";
                std::getline(std::cin, dept);
                student->setDepartment(dept);
            } else if (subChoice == 3) {
                std::cout << "Enter new year (1-4): ";
                while (!(std::cin >> year) || year < 1 || year > 4) {
                    std::cout << "Invalid year. Enter 1-4: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                std::cin.ignore();
                student->setYear(year);
            }
        } else if (choice == 3) {
            // Manage Skills
            while (true) {
                std::cout << "\n--- SKILL MANAGEMENT ---" << std::endl;
                std::cout << "1. Add skill you can teach" << std::endl;
                std::cout << "2. Add skill you want to learn" << std::endl;
                std::cout << "3. Remove a skill" << std::endl;
                std::cout << "4. Display your skills" << std::endl;
                std::cout << "5. Go back" << std::endl;
                std::cout << "Enter choice: ";
                int subChoice;
                std::cin >> subChoice;
                std::cin.ignore();

                if (subChoice == 1) {
                    std::string skill, level;
                    std::cout << "Enter skill name: ";
                    std::getline(std::cin, skill);
                    std::cout << "Enter level (Beginner/Intermediate/Advanced): ";
                    std::getline(std::cin, level);
                    // Validate level
                    while (level != "Beginner" && level != "Intermediate" && level != "Advanced") {
                        std::cout << "Invalid level. Enter Beginner, Intermediate, or Advanced: ";
                        std::getline(std::cin, level);
                    }
                    student->addSkillToTeach(skill, level);
                } else if (subChoice == 2) {
                    std::string skill, level;
                    std::cout << "Enter skill name: ";
                    std::getline(std::cin, skill);
                    std::cout << "Enter level (Beginner/Intermediate/Advanced): ";
                    std::getline(std::cin, level);
                    // Validate level
                    while (level != "Beginner" && level != "Intermediate" && level != "Advanced") {
                        std::cout << "Invalid level. Enter Beginner, Intermediate, or Advanced: ";
                        std::getline(std::cin, level);
                    }
                    student->addSkillToLearn(skill, level);
                } else if (subChoice == 3) {
                    std::string skill;
                    std::cout << "Enter skill name to remove: ";
                    std::getline(std::cin, skill);
                    student->removeSkill(skill);
                } else if (subChoice == 4) {
                    student->displaySkills();
                } else if (subChoice == 5) {
                    break;
                }
            }
        } else if (choice == 4) {
            student->updatePreferences();
        } else if (choice == 5) {
            student->displaySkillHistory();
        } else if (choice == 6) {
            student->viewNotifications();
        } else if (choice == 7) {
            std::cout << "Logged out successfully." << std::endl;
            break;
        }
    }
}

void sendExchangeRequest() {
    std::cout << "\n=== SEND EXCHANGE REQUEST ===" << std::endl;

    std::string senderID, receiverID;
    std::cout << "Enter your Student ID (sender): ";
    std::getline(std::cin, senderID);

    Student* sender = findStudentByID(senderID);
    if (!sender) {
        std::cout << "Error: Sender not found!" << std::endl;
        return;
    }

    std::cout << "Enter Receiver Student ID: ";
    std::getline(std::cin, receiverID);

    Student* receiver = findStudentByID(receiverID);
    if (!receiver) {
        std::cout << "Error: Receiver not found!" << std::endl;
        return;
    }

    if (senderID == receiverID) {
        std::cout << "Error: Cannot send request to yourself!" << std::endl;
        return;
    }

    // Show sender's teach skills and receiver's learn skills for matching
    std::cout << "\nYour skills you can teach:" << std::endl;
    sender->displaySkills();

    std::cout << "\nReceiver's skills they want to learn:" << std::endl;
    receiver->displaySkills();

    std::string skillWanted, skillOffered;
    std::cout << "\nEnter the skill you want to learn from them: ";
    std::getline(std::cin, skillWanted);

    std::cout << "Enter the skill you want to teach them: ";
    std::getline(std::cin, skillOffered);

    // Validate that sender actually offers this skill
    bool canOffer = false;
    for (const SkillInfo& skill : sender->getSkillsCanTeach()) {
        if (skill.skillName == skillOffered) {
            canOffer = true;
            break;
        }
    }

    // Validate that receiver actually wants this skill
    bool wantsToLearn = false;
    for (const SkillInfo& skill : receiver->getSkillsWantToLearn()) {
        if (skill.skillName == skillWanted) {
            wantsToLearn = true;
            break;
        }
    }

    if (!canOffer) {
        std::cout << "Warning: You don't have '" << skillOffered << "' in your teach list." << std::endl;
        std::cout << "Send request anyway? (y/n): ";
        char confirm;
        std::cin >> confirm;
        std::cin.ignore();
        if (std::tolower(confirm) != 'y') {
            return;
        }
    }

    if (!wantsToLearn) {
        std::cout << "Warning: They don't have '" << skillWanted << "' in their learn list." << std::endl;
        std::cout << "Send request anyway? (y/n): ";
        char confirm;
        std::cin >> confirm;
        std::cin.ignore();
        if (std::tolower(confirm) != 'y') {
            return;
        }
    }

    // Create request
    ExchangeRequest request(senderID, receiverID, skillWanted, skillOffered);
    exchangeRequests.push_back(request);

    // Add notifications
    std::string notifMsg = sender->getName() + " has sent you a skill exchange request for " + skillWanted;
    receiver->addNotification(notifMsg);

    std::cout << "\nRequest sent successfully!" << std::endl;
    std::cout << "Request ID: " << request.getID() << std::endl;
}

void manageRequests() {
    std::cout << "\n=== MANAGE REQUESTS ===" << std::endl;

    std::string id;
    std::cout << "Enter your Student ID: ";
    std::getline(std::cin, id);

    Student* student = findStudentByID(id);
    if (!student) {
        std::cout << "Error: Student not found!" << std::endl;
        return;
    }

    while (true) {
        std::cout << "\n--- REQUEST MANAGEMENT ---" << std::endl;
        std::cout << "1. View Sent Requests" << std::endl;
        std::cout << "2. View Received Requests" << std::endl;
        std::cout << "3. Go back" << std::endl;
        std::cout << "Enter choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) {
            // View sent requests
            std::vector<ExchangeRequest> sentRequests;
            for (const ExchangeRequest& req : exchangeRequests) {
                if (req.getSenderID() == id) {
                    sentRequests.push_back(req);
                }
            }

            if (sentRequests.empty()) {
                std::cout << "You haven't sent any requests." << std::endl;
            } else {
                std::cout << "\n=== YOUR SENT REQUESTS ===" << std::endl;
                for (size_t i = 0; i < sentRequests.size(); ++i) {
                    std::cout << i + 1 << "." << std::endl;
                    sentRequests[i].display();
                    std::cout << std::endl;
                }
            }
        } else if (choice == 2) {
            // View received requests
            std::vector<ExchangeRequest> receivedRequests;
            for (const ExchangeRequest& req : exchangeRequests) {
                if (req.getReceiverID() == id) {
                    receivedRequests.push_back(req);
                }
            }

            if (receivedRequests.empty()) {
                std::cout << "You haven't received any requests." << std::endl;
            } else {
                std::cout << "\n=== YOUR RECEIVED REQUESTS ===" << std::endl;
                for (size_t i = 0; i < receivedRequests.size(); ++i) {
                    std::cout << i + 1 << "." << std::endl;
                    receivedRequests[i].display();

                    // Options for received requests
                    if (receivedRequests[i].getStatus() == "Pending") {
                        std::cout << "Options: [A]ccept, [R]eject, [C]ancel: ";
                        char option;
                        std::cin >> option;
                        std::cin.ignore();

                        if (std::tolower(option) == 'a') {
                            // Find the original request and modify it
                            for (ExchangeRequest& req : exchangeRequests) {
                                if (req.getID() == receivedRequests[i].getID()) {
                                    req.setStatus("Accepted");
                                    break;
                                }
                            }
                            // Notify sender
                            Student* sender = findStudentByID(receivedRequests[i].getSenderID());
                            if (sender) {
                                std::string notifMsg = student->getName() + " has accepted your request for " +
                                                      receivedRequests[i].getSkillWanted();
                                sender->addNotification(notifMsg);
                            }
                            std::cout << "Request accepted!" << std::endl;
                        } else if (std::tolower(option) == 'r') {
                            // Find the original request and modify it
                            for (ExchangeRequest& req : exchangeRequests) {
                                if (req.getID() == receivedRequests[i].getID()) {
                                    req.setStatus("Rejected");
                                    break;
                                }
                            }
                            // Notify sender
                            Student* sender = findStudentByID(receivedRequests[i].getSenderID());
                            if (sender) {
                                std::string notifMsg = student->getName() + " has rejected your request for " +
                                                      receivedRequests[i].getSkillWanted();
                                sender->addNotification(notifMsg);
                            }
                            std::cout << "Request rejected!" << std::endl;
                        } else if (std::tolower(option) == 'c') {
                            // Find the original request and modify it
                            for (ExchangeRequest& req : exchangeRequests) {
                                if (req.getID() == receivedRequests[i].getID()) {
                                    req.setStatus("Cancelled");
                                    break;
                                }
                            }
                            // Notify sender
                            Student* sender = findStudentByID(receivedRequests[i].getSenderID());
                            if (sender) {
                                std::string notifMsg = student->getName() + " has cancelled your request for " +
                                                      receivedRequests[i].getSkillWanted();
                                sender->addNotification(notifMsg);
                            }
                            std::cout << "Request cancelled!" << std::endl;
                        }
                    }
                    std::cout << std::endl;
                }
            }
        } else if (choice == 3) {
            break;
        }
    }
}

void scheduleExchange() {
    std::cout << "\n=== SCHEDULE EXCHANGE ===" << std::endl;

    std::string requestID;
    std::cout << "Enter Request ID to schedule: ";
    std::getline(std::cin, requestID);

    // Find the request
    ExchangeRequest* requestPtr = nullptr;
    for (ExchangeRequest& req : exchangeRequests) {
        if (req.getID() == requestID) {
            requestPtr = &req;
            break;
        }
    }

    if (!requestPtr) {
        std::cout << "Error: Request not found!" << std::endl;
        return;
    }

    if (requestPtr->getStatus() != "Accepted") {
        std::cout << "Error: Only accepted requests can be scheduled!" << std::endl;
        std::cout << "Current status: " << requestPtr->getStatus() << std::endl;
        return;
    }

    std::string date, time, mode, location;
    std::cout << "Enter date (YYYY-MM-DD): ";
    std::getline(std::cin, date);

    std::cout << "Enter time (HH:MM): ";
    std::getline(std::cin, time);

    std::cout << "Enter mode (Online/Offline/Both): ";
    std::getline(std::cin, mode);
    while (mode != "Online" && mode != "Offline" && mode != "Both") {
        std::cout << "Invalid mode. Enter Online, Offline, or Both: ";
        std::getline(std::cin, mode);
    }

    std::cout << "Enter location: ";
    std::getline(std::cin, location);

    // Create scheduled exchange
    ScheduledExchange exchange(requestPtr->getSenderID(), requestPtr->getReceiverID(),
                              requestPtr->getSkillWanted(), date, time, mode, location);
    scheduledExchanges.push_back(exchange);

    // Update request status to Completed (not Scheduled)
    // Find the original request and modify it
    for (ExchangeRequest& req : exchangeRequests) {
        if (req.getID() == requestID) {
            req.setStatus("Completed");
            break;
        }
    }

    // Update student exchange counts ONLY when exchange is actually completed
    Student* student1 = findStudentByID(requestPtr->getSenderID());
    Student* student2 = findStudentByID(requestPtr->getReceiverID());
    if (student1) student1->incrementCompletedExchanges();
    if (student2) student2->incrementCompletedExchanges();

    // Add notifications
    std::string notifMsg1 = "Your exchange for " + requestPtr->getSkillWanted() +
                           " has been scheduled on " + date + " at " + time;
    std::string notifMsg2 = "Your exchange for " + requestPtr->getSkillOffered() +
                           " has been scheduled on " + date + " at " + time;

    if (student1) student1->addNotification(notifMsg1);
    if (student2) student2->addNotification(notifMsg2);

    std::cout << "\nExchange scheduled successfully!" << std::endl;
    std::cout << "Exchange ID: " << exchange.getID() << std::endl;
}

void viewSkillHistory() {
    std::cout << "\n=== VIEW SKILL HISTORY ===" << std::endl;

    std::string id;
    std::cout << "Enter your Student ID: ";
    std::getline(std::cin, id);

    Student* student = findStudentByID(id);
    if (!student) {
        std::cout << "Error: Student not found!" << std::endl;
        return;
    }

    student->displaySkillHistory();
}

void searchStudents(const std::string& currentStudentID) {
    std::cout << "\n=== SEARCH STUDENTS ===" << std::endl;

    std::cout << "Search by:" << std::endl;
    std::cout << "1. Skill" << std::endl;
    std::cout << "2. Department" << std::endl;
    std::cout << "3. Year" << std::endl;
    std::cout << "4. Location" << std::endl
              << "5. Mode" << std::endl
              << "6. Minimum Rating" << std::endl
              << "7. Skill Level" << std::endl
              << "8. Go back" << std::endl
              << "Enter choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    std::vector<Student*> results;

    if (choice == 1) {
        std::string skill;
        std::cout << "Enter skill name: ";
        std::getline(std::cin, skill);

        for (Student& s : students) {
            bool hasSkill = false;
            for (const SkillInfo& teach : s.getSkillsCanTeach()) {
                if (teach.skillName == skill) {
                    hasSkill = true;
                    break;
                }
            }
            if (!hasSkill) {
                for (const SkillInfo& learn : s.getSkillsWantToLearn()) {
                    if (learn.skillName == skill) {
                        hasSkill = true;
                        break;
                    }
                }
            }
            if (hasSkill) {
                results.push_back(&s);
            }
        }
    } else if (choice == 2) {
        std::string dept;
        std::cout << "Enter department: ";
        std::getline(std::cin, dept);

        for (Student& s : students) {
            if (s.getDepartment() == dept) {
                results.push_back(&s);
            }
        }
    } else if (choice == 3) {
        int year;
        std::cout << "Enter year (1-4): ";
        std::cin >> year;
        std::cin.ignore();

        for (Student& s : students) {
            if (s.getYear() == year) {
                results.push_back(&s);
            }
        }
    } else if (choice == 4) {
        std::string loc;
        std::cout << "Enter location: ";
        std::getline(std::cin, loc);

        for (Student& s : students) {
            if (s.getLocation() == loc) {
                results.push_back(&s);
            }
        }
    } else if (choice == 5) {
        std::string mode;
        std::cout << "Enter mode (Online/Offline/Both): ";
        std::getline(std::cin, mode);

        for (Student& s : students) {
            if (s.getPreferredMode() == mode) {
                results.push_back(&s);
            }
        }
    } else if (choice == 6) {
        double minRating;
        std::cout << "Enter minimum rating: ";
        std::cin >> minRating;
        std::cin.ignore();

        for (Student& s : students) {
            if (s.getRating() >= minRating) {
                results.push_back(&s);
            }
        }
    } else if (choice == 7) {
        std::string level;
        std::cout << "Enter skill level (Beginner/Intermediate/Advanced): ";
        std::getline(std::cin, level);

        for (Student& s : students) {
            bool hasLevel = false;
            for (const SkillInfo& teach : s.getSkillsCanTeach()) {
                if (teach.level == level) {
                    hasLevel = true;
                    break;
                }
            }
            if (!hasLevel) {
                for (const SkillInfo& learn : s.getSkillsWantToLearn()) {
                    if (learn.level == level) {
                        hasLevel = true;
                        break;
                    }
                }
            }
            if (hasLevel) {
                results.push_back(&s);
            }
        }
    } else if (choice == 8) {
        return;
    }

    if (results.empty()) {
        std::cout << "No students found matching your criteria." << std::endl;
        return;
    }

    std::cout << "\n=== SEARCH RESULTS ===" << std::endl;
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << i + 1 << ". ";
        results[i]->displayBrief();
    }
}

void rateFeedback() {
    std::cout << "\n=== RATE AND FEEDBACK ===" << std::endl;

    std::string raterID, rateeID;
    std::cout << "Enter your Student ID (rater): ";
    std::getline(std::cin, raterID);

    Student* rater = findStudentByID(raterID);
    if (!rater) {
        std::cout << "Error: Rater not found!" << std::endl;
        return;
    }

    std::cout << "Enter Student ID to rate: ";
    std::getline(std::cin, rateeID);

    Student* ratee = findStudentByID(rateeID);
    if (!ratee) {
        std::cout << "Error: Student to rate not found!" << std::endl;
        return;
    }

    if (raterID == rateeID) {
        std::cout << "Error: You cannot rate yourself!" << std::endl;
        return;
    }

    int rating;

std::cout << "\nSelect your rating:\n";
std::cout << "1 - Fine\n";
std::cout << "2 - Good\n";
std::cout << "3 - Very Good\n";
std::cout << "4 - Great\n";
std::cout << "5 - Excellent\n";

std::cout << "Enter your rating (1-5): ";

while (!(std::cin >> rating) || rating < 1 || rating > 5) {
    std::cout << "Invalid input! Please enter a number between 1 and 5: ";
    std::cin.clear();
    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(), '\n'
    );
}

std::cin.ignore(
    std::numeric_limits<std::streamsize>::max(), '\n'
);

    std::string feedback;
    std::cout << "Enter feedback (optional): ";
    std::getline(std::cin, feedback);

    // Add rating to ratee
    ratee->addRating(rating);

    // Add notification
   std::string ratingDescription;

switch (rating) {
    case 1: ratingDescription = "Fine"; break;
    case 2: ratingDescription = "Good"; break;
    case 3: ratingDescription = "Very Good"; break;
    case 4: ratingDescription = "Great"; break;
    case 5: ratingDescription = "Excellent"; break;
}

std::string notifMsg = rater->getName() +
    " has rated you " + std::to_string(rating) +
    "/5 (" + ratingDescription + ").";
    
    if (!feedback.empty()) {
        notifMsg += " Feedback: " + feedback;
    }
    ratee->addNotification(notifMsg);

    std::cout << "Thank you for your feedback!" << std::endl;
}

// Sample data initialization
void initializeSampleData() {
    // Clear any existing data
    students.clear();
    studentIDToIndex.clear();
    exchangeRequests.clear();
    scheduledExchanges.clear();

    // Sample Students
    Student s1("S001", "Rahul", "Computer Science", 3);
    s1.addSkillToTeach("C++", "Advanced");
    s1.addSkillToTeach("Data Structures", "Advanced");
    s1.addSkillToLearn("Photoshop", "Beginner");
    s1.setPreferredMode("Both");
    s1.setLocation("Lab 3");
    s1.setAvailability({"Morning", "Evening"});
    students.push_back(s1);
    studentIDToIndex["S001"] = 0;

    Student s2("S002", "Aman", "Design", 2);
    s2.addSkillToTeach("Photoshop", "Advanced");
    s2.addSkillToTeach("UI Design", "Intermediate");
    s2.addSkillToLearn("C++", "Beginner");
    s2.setPreferredMode("Offline");
    s2.setLocation("Lab 3");
    s2.setAvailability({"Afternoon", "Evening"});
    students.push_back(s2);
    studentIDToIndex["S002"] = 1;

    Student s3("S003", "Priya", "Computer Science", 2);
    s3.addSkillToTeach("HTML", "Intermediate");
    s3.addSkillToTeach("CSS", "Intermediate");
    s3.addSkillToLearn("C++", "Beginner");
    s3.setPreferredMode("Online");
    s3.setLocation("Hostel B");
    s3.setAvailability({"Morning", "Afternoon"});
    students.push_back(s3);
    studentIDToIndex["S003"] = 2;

    Student s4("S004", "Riya", "Computer Science", 3);
    s4.addSkillToTeach("JavaScript", "Advanced");
    s4.addSkillToLearn("HTML", "Beginner");
    s4.setPreferredMode("Both");
    s4.setLocation("Lab 1");
    s4.setAvailability({"Evening"});
    students.push_back(s4);
    studentIDToIndex["S004"] = 3;

    // Add some ratings for demo
    s1.addRating(4.5);
    s2.addRating(4.2);
    s3.addRating(3.8);
    s4.addRating(4.0);

    // Initialize Skill Graph for Evolution Map
    // Web Development path
    skillGraph.addEdge("HTML", "CSS");
    skillGraph.addEdge("CSS", "JavaScript");
    skillGraph.addEdge("JavaScript", "Frontend Development");
    skillGraph.addEdge("Frontend Development", "Backend Development");
    skillGraph.addEdge("Backend Development", "Full Stack Development");

    // Programming path
    skillGraph.addEdge("C++", "Data Structures");
    skillGraph.addEdge("Data Structures", "Algorithms");
    skillGraph.addEdge("Algorithms", "Competitive Programming");
    skillGraph.addEdge("Competitive Programming", "Problem Solving");

    // Design path
    skillGraph.addEdge("Photoshop", "UI Design");
    skillGraph.addEdge("UI Design", "UX Design");
    skillGraph.addEdge("UX Design", "Product Design");

    // Add some cross-discipline edges
    skillGraph.addEdge("JavaScript", "Web Development");
    skillGraph.addEdge("Web Development", "Full Stack Development");

    std::cout << "Sample data initialized successfully!" << std::endl;
    std::cout << "Added " << students.size() << " sample students." << std::endl;
}

// Main function
void runCampusSkillSwapNetwork() {
    // Initialize sample data
    initializeSampleData();

    std::cout << "========================================" << std::endl;
    std::cout << "     CAMPUS SKILL SWAP NETWORK          " << std::endl;
    std::cout << "========================================" << std::endl;

    while (true) {
        std::cout << "\nMAIN MENU:" << std::endl;
        std::cout << "1. Student Registration" << std::endl;
        std::cout << "2. Student Login" << std::endl;
        std::cout << "3. Manage Profile" << std::endl;
        std::cout << "4. Manage Skills" << std::endl;
        std::cout << "5. Find Skill Matches" << std::endl;
        std::cout << "6. View Compatibility Ranking" << std::endl;
        std::cout << "7. Send Exchange Request" << std::endl;
        std::cout << "8. Manage Requests" << std::endl;
        std::cout << "9. Schedule Exchange" << std::endl;
        std::cout << "10. View Skill History" << std::endl;
        std::cout << "11. Search Students" << std::endl;
        std::cout << "12. Skill Evolution Map" << std::endl;
        std::cout << "13. Notifications" << std::endl;
        std::cout << "14. Rate/Feedback" << std::endl;
        std::cout << "15. Admin" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Clear newline from input buffer

        switch (choice) {
            case 1:
                studentRegistration();
                break;
            case 2:
                studentLogin();
                break;
            case 3:
                // This would require login first - simplified for demo
                std::cout << "Please login first to manage your profile." << std::endl;
                break;
            case 4:
                // This would require login first - simplified for demo
                std::cout << "Please login first to manage your skills." << std::endl;
                break;
            case 5: {
                std::string id;
                std::cout << "Enter your Student ID: ";
                std::getline(std::cin, id);
                findSkillMatches(id);
                break;
            }
            case 6: {
                std::string id;
                std::cout << "Enter your Student ID: ";
                std::getline(std::cin, id);
                viewCompatibilityRanking(id);
                break;
            }
            case 7:
                sendExchangeRequest();
                break;
            case 8:
                manageRequests();
                break;
            case 9:
                scheduleExchange();
                break;
            case 10:
                viewSkillHistory();
                break;
            case 11: {
                std::string id;
                std::cout << "Enter your Student ID: ";
                std::getline(std::cin, id);
                searchStudents(id);
                break;
            }
            case 12:
            {
                // Skill Evolution Map
                std::cout << "\n=== SKILL EVOLUTION MAP ===" << std::endl;

                std::string startSkill, targetSkill;
                std::cout << "Enter starting skill: ";
                std::getline(std::cin, startSkill);

                std::cout << "Enter target skill/goal: ";
                std::getline(std::cin, targetSkill);

                // Find learning path using BFS
                std::vector<std::string> path = skillGraph.findLearningPath(startSkill, targetSkill);

                if (path.empty()) {
                    std::cout << "No learning path found from " << startSkill << " to "
                             << targetSkill << std::endl;
                    std::cout << "Try different skills or check the skill evolution map." << std::endl;
                } else {
                    std::cout << "\n=== LEARNING PATH FROM " << startSkill << " TO "
                             << targetSkill << " ===" << std::endl;
                    for (size_t i = 0; i < path.size(); ++i) {
                        std::cout << path[i];
                        if (i < path.size() - 1) {
                            std::cout << " → ";
                        }
                    }
                    std::cout << std::endl;

                    // Find teachers for each skill in the path
                    std::cout << "\n=== AVAILABLE TEACHERS ON CAMPUS ===" << std::endl;
                    for (const std::string& skill : path) {
                        std::cout << skill << ":" << std::endl;
                        bool foundTeacher = false;

                        for (const Student& s : students) {
                            for (const SkillInfo& teach : s.getSkillsCanTeach()) {
                if (teach.skillName == skill) {
                                    std::cout << "  Teacher: " << s.getName()
                                             << " (ID: " << s.getID()
                                             << ", Level: " << teach.level << ")" << std::endl;
                                    foundTeacher = true;
                                    break;
                                }
                            }
                            if (foundTeacher) break;
                        }

                        if (!foundTeacher) {
                            std::cout << "  No teacher available for this skill on campus." << std::endl;
                        }
                        std::cout << std::endl;
                    }
                }
            }
            break;
            case 13:
            {
                // Notifications
                std::cout << "\n=== NOTIFICATIONS ===" << std::endl;

                std::string id;
                std::cout << "Enter your Student ID: ";
                std::getline(std::cin, id);

                Student* student = findStudentByID(id);
                if (!student) {
                    std::cout << "Error: Student not found!" << std::endl;
                    break;
                }

                student->viewNotifications();
            }
            break;
            case 14:
            {
                // Rate/Feedback
                std::cout << "\n=== RATE AND FEEDBACK ===" << std::endl;

                std::string raterID, rateeID;
                std::cout << "Enter your Student ID (rater): ";
                std::getline(std::cin, raterID);

                Student* rater = findStudentByID(raterID);
                if (!rater) {
                    std::cout << "Error: Rater not found!" << std::endl;
                    break;
                }

                std::cout << "Enter Student ID to rate: ";
                std::getline(std::cin, rateeID);

                Student* ratee = findStudentByID(rateeID);
                if (!ratee) {
                    std::cout << "Error: Student to rate not found!" << std::endl;
                    break;
                }

                if (raterID == rateeID) {
                    std::cout << "Error: You cannot rate yourself!" << std::endl;
                    break;
                }

                double rating;
                std::cout << "Enter rating (1-5 stars): ";
                while (!(std::cin >> rating) || rating < 1.0 || rating > 5.0) {
                    std::cout << "Invalid rating. Enter 1-5: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                std::cin.ignore(); // Clear newline

                std::string feedback;
                std::cout << "Enter feedback (optional): ";
                std::getline(std::cin, feedback);

                // Add rating to ratee
                ratee->addRating(rating);

                // Add notification
                std::string notifMsg = rater->getName() + " has rated you " +
                                       std::to_string(static_cast<int>(rating)) + "/5 stars.";
                if (!feedback.empty()) {
                    notifMsg += " Feedback: " + feedback;
                }
                ratee->addNotification(notifMsg);

                std::cout << "Thank you for your feedback!" << std::endl;
            }
            break;
            case 15:
            {
                // Admin
                adminMenu();
            }
            break;
            case 0:
            {
                std::cout << "Thank you for using Campus Skill Swap Network!" << std::endl;
                std::cout << "Goodbye!" << std::endl;
                return;
            }
            default:
            {
                std::cout << "Invalid choice. Please try again." << std::endl;
            }
    }
}
}