#include <iomanip>
#include "student.h"
#include <iostream>
#include <algorithm>
#include <sstream>

// Skill Management
void Student::addSkillToTeach(std::string skill, std::string level) {
    // Check for duplicate
    for (const SkillInfo& skillInfo : skillsCanTeach) {
        if (skillInfo.skillName == skill) {
            std::cout << "Skill already in your teach list!" << std::endl;
            return;
        }
    }
    skillsCanTeach.push_back(SkillInfo(skill, level));
}

void Student::addSkillToLearn(std::string skill, std::string level) {
    // Check for duplicate
    for (const SkillInfo& skillInfo : skillsWantToLearn) {
        if (skillInfo.skillName == skill) {
            std::cout << "Skill already in your learn list!" << std::endl;
            return;
        }
    }
    skillsWantToLearn.push_back(SkillInfo(skill, level));
}

void Student::removeSkill(std::string skill) {
    // Remove from teach list
    auto itTeach = std::remove_if(skillsCanTeach.begin(), skillsCanTeach.end(),
                                 [&](const SkillInfo& s) { return s.skillName == skill; });
    if (itTeach != skillsCanTeach.end()) {
        skillsCanTeach.erase(itTeach, skillsCanTeach.end());
        std::cout << "Removed " << skill << " from teach list." << std::endl;
        return;
    }

    // Remove from learn list
    auto itLearn = std::remove_if(skillsWantToLearn.begin(), skillsWantToLearn.end(),
                                 [&](const SkillInfo& s) { return s.skillName == skill; });
    if (itLearn != skillsWantToLearn.end()) {
        skillsWantToLearn.erase(itLearn, skillsWantToLearn.end());
        std::cout << "Removed " << skill << " from learn list." << std::endl;
        return;
    }

    std::cout << "Skill not found in your lists." << std::endl;
}

void Student::displaySkills() const {
    std::cout << "\nSkills you can teach:" << std::endl;
    if (skillsCanTeach.empty()) {
        std::cout << "None" << std::endl;
    } else {
        for (const SkillInfo& skill : skillsCanTeach) {
            std::cout << "- " << skill.skillName << " (" << skill.level << ")" << std::endl;
        }
    }

    std::cout << "\nSkills you want to learn:" << std::endl;
    if (skillsWantToLearn.empty()) {
        std::cout << "None" << std::endl;
    } else {
        for (const SkillInfo& skill : skillsWantToLearn) {
            std::cout << "- " << skill.skillName << " (" << skill.level << ")" << std::endl;
        }
    }
}

// Preference Management
void Student::updatePreferences() {
    std::cout << "\nCurrent preferences:" << std::endl;
    std::cout << "Mode: " << preferredMode << std::endl;
    std::cout << "Location: " << location << std::endl;
    std::cout << "Availability: ";
    if (availability.empty()) {
        std::cout << "Not set" << std::endl;
    } else {
        for (const std::string& avail : availability) {
            std::cout << avail << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nUpdate preferences:" << std::endl;
    std::cout << "1. Change mode (Online/Offline/Both)" << std::endl;
    std::cout << "2. Change location" << std::endl;
    std::cout << "3. Change availability" << std::endl;
    std::cout << "4. Go back" << std::endl;
    std::cout << "Enter choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(); // Clear newline

    if (choice == 1) {
        std::cout << "Enter mode (Online/Offline/Both): ";
        std::getline(std::cin, preferredMode);
        // Validate
        while (preferredMode != "Online" && preferredMode != "Offline" &&
               preferredMode != "Both") {
            std::cout << "Invalid mode. Enter Online, Offline, or Both: ";
            std::getline(std::cin, preferredMode);
        }
    } else if (choice == 2) {
        std::cout << "Enter location: ";
        std::getline(std::cin, location);
    } else if (choice == 3) {
        std::cout << "Enter availability slots (comma-separated, e.g., Morning,Afternoon): ";
        std::string availInput;
        std::getline(std::cin, availInput);
        std::stringstream ss(availInput);
        std::string slot;
        availability.clear();
        while (std::getline(ss, slot, ',')) {
            // Trim whitespace
            slot.erase(0, slot.find_first_not_of(" \t"));
            slot.erase(slot.find_last_not_of(" \t") + 1);
            if (!slot.empty()) {
                availability.push_back(slot);
            }
        }
    }
}

// Rating and Feedback
void Student::addRating(double newRating) {
    if (newRating < 1.0 || newRating > 5.0) {
        std::cout << "Rating must be between 1 and 5." << std::endl;
        return;
    }

    if (numRatings == 0) {
        rating = newRating;
    } else {
        rating = (rating * numRatings + newRating) / (numRatings + 1);
    }
    numRatings++;
    std::cout << "Thank you for your rating!" << std::endl;
}

void Student::incrementCompletedExchanges() {
    completedExchanges++;
}

// Notifications
void Student::addNotification(std::string message) {
    notifications.push(Notification(message));
}

void Student::viewNotifications() {
    if (notifications.empty()) {
        std::cout << "No notifications." << std::endl;
        return;
    }

    // Create a temporary queue to view notifications without losing original
    std::queue<Notification> tempQueue;
    std::queue<Notification> originalCopy = notifications; // Copy the original
    int count = 1;

    std::cout << "\nYour Notifications:" << std::endl;
    while (!originalCopy.empty()) {
        Notification notif = originalCopy.front();
        originalCopy.pop();

        std::cout << count << ". ";
        if (notif.getIsRead()) {
            std::cout << "[READ] ";
        } else {
            std::cout << "[UNREAD] ";
        }
        std::cout << notif.getMessage() << std::endl;

        // Add to temp queue for restoration
        tempQueue.push(notif);
        count++;
    }

    // Restore the original queue
    notifications = tempQueue;

    // Mark all as read after viewing
    // We need to recreate the queue to avoid infinite loop
    std::queue<Notification> tempQueue2;
    while (!notifications.empty()) {
        Notification notif = notifications.front();
        notifications.pop();
        notif.markAsRead();
        tempQueue2.push(notif);
    }
    notifications = tempQueue2;
}

void Student::clearNotifications() {
    // Clear the queue
    while (!notifications.empty()) {
        notifications.pop();
    }
    std::cout << "Notifications cleared." << std::endl;
}

// Skill History
void Student::displaySkillHistory() const {
    std::cout << "\n=== SKILL HISTORY FOR " << name << " ===" << std::endl;

    std::cout << "\nSkills Taught:" << std::endl;
    if (skillsCanTeach.empty()) {
        std::cout << "None" << std::endl;
    } else {
        for (const SkillInfo& skill : skillsCanTeach) {
            std::cout << "- " << skill.skillName << " (" << skill.level << ")" << std::endl;
        }
    }

    std::cout << "\nSkills Learned:" << std::endl;
    if (skillsWantToLearn.empty()) {
        std::cout << "None" << std::endl;
    } else {
        for (const SkillInfo& skill : skillsWantToLearn) {
            std::cout << "- " << skill.skillName << " (" << skill.level << ")" << std::endl;
        }
    }

    std::cout << "\nCompleted Exchanges: " << completedExchanges << std::endl;
    std::cout << "Average Rating: " << std::fixed << std::setprecision(2)
         << rating << " (" << numRatings << " ratings)" << std::endl;
}

// Display methods
void Student::displayBrief() const {
    std::cout << "ID: " << studentID << " | Name: " << name
         << " | Dept: " << department << " | Year: " << year
         << " | Rating: " << std::fixed << std::setprecision(2) << rating << std::endl;
}

void Student::displayFull() const {
    std::cout << "\n=== STUDENT PROFILE ===" << std::endl;
    std::cout << "Student ID: " << studentID << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Department: " << department << std::endl;
    std::cout << "Year: " << year << std::endl;
    displaySkills();
    std::cout << "\nPreferred Mode: " << preferredMode << std::endl;
    std::cout << "Location: " << location << std::endl;
    std::cout << "Availability: ";
    if (availability.empty()) {
        std::cout << "Not set" << std::endl;
    } else {
        for (const std::string& avail : availability) {
            std::cout << avail << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Average Rating: " << std::fixed << std::setprecision(2)
         << rating << " (" << numRatings << " ratings)" << std::endl;
    std::cout << "Completed Exchanges: " << completedExchanges << std::endl;
}