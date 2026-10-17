#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include <queue>
#include "notification.h"
#include "skill.h"

class Student {
private:
    std::string studentID;
    std::string name;
    std::string department;
    int year;
    std::vector<SkillInfo> skillsCanTeach;
    std::vector<SkillInfo> skillsWantToLearn;
    std::string preferredMode; // Online, Offline, Both
    std::string location;
    std::vector<std::string> availability; // Morning, Afternoon, Evening
    double rating;
    int numRatings;
    int completedExchanges;
    std::queue<Notification> notifications; // Changed from vector to queue

public:
    Student() : rating(0.0), numRatings(0), completedExchanges(0) {}

    Student(std::string id, std::string n, std::string dept, int y)
        : studentID(id), name(n), department(dept), year(y),
          rating(0.0), numRatings(0), completedExchanges(0) {}

    // Getters
    std::string getID() const { return studentID; }
    std::string getName() const { return name; }
    std::string getDepartment() const { return department; }
    int getYear() const { return year; }
    std::vector<SkillInfo> getSkillsCanTeach() const { return skillsCanTeach; }
    std::vector<SkillInfo> getSkillsWantToLearn() const { return skillsWantToLearn; }
    std::string getPreferredMode() const { return preferredMode; }
    std::string getLocation() const { return location; }
    std::vector<std::string> getAvailability() const { return availability; }
    double getRating() const { return rating; }
    int getNumRatings() const { return numRatings; }
    int getCompletedExchanges() const { return completedExchanges; }
    std::queue<Notification> getNotifications() const { return notifications; }

    // Setters
    void setName(std::string n) { name = n; }
    void setDepartment(std::string dept) { department = dept; }
    void setYear(int y) { year = y; }
    void setPreferredMode(std::string mode) { preferredMode = mode; }
    void setLocation(std::string loc) { location = loc; }
    void setAvailability(std::vector<std::string> avail) { availability = avail; }

    // Skill Management
    void addSkillToTeach(std::string skill, std::string level = "Beginner");
    void addSkillToLearn(std::string skill, std::string level = "Beginner");
    void removeSkill(std::string skill);
    void displaySkills() const;

    // Preference Management
    void updatePreferences();

    // Rating and Feedback
    void addRating(double newRating);
    void incrementCompletedExchanges();

    // Notifications
    void addNotification(std::string message);
    void viewNotifications();
    void clearNotifications();

    // Skill History
    void displaySkillHistory() const;

    // Display methods
    void displayBrief() const;
    void displayFull() const;
};

#endif // STUDENT_H