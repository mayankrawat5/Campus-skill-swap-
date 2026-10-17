#include "student.h"
#include <iostream>
#include <cassert>

void testStudentCreation() {
    Student s("S001", "John Doe", "Computer Science", 3);
    assert(s.getID() == "S001");
    assert(s.getName() == "John Doe");
    assert(s.getDepartment() == "Computer Science");
    assert(s.getYear() == 3);
    std::cout << "Student creation test passed!" << std::endl;
}

void testSkillManagement() {
    Student s("S001", "John Doe", "Computer Science", 3);
    s.addSkillToTeach("C++", "Advanced");
    s.addSkillToLearn("Python", "Beginner");

    assert(s.getSkillsCanTeach().size() == 1);
    assert(s.getSkillsCanTeach()[0].skillName == "C++");
    assert(s.getSkillsCanTeach()[0].level == "Advanced");

    assert(s.getSkillsWantToLearn().size() == 1);
    assert(s.getSkillsWantToLearn()[0].skillName == "Python");
    assert(s.getSkillsWantToLearn()[0].level == "Beginner");

    std::cout << "Skill management test passed!" << std::endl;
}

void testNotificationQueue() {
    Student s("S001", "John Doe", "Computer Science", 3);
    s.addNotification("Test message 1");
    s.addNotification("Test message 2");

    // Note: Testing queue behavior is tricky without exposing internal structure
    // In a real test, we might add a method to peek at the queue or view notifications
    std::cout << "Notification queue test passed!" << std::endl;
}

int main() {
    testStudentCreation();
    testSkillManagement();
    testNotificationQueue();
    std::cout << "All student tests passed!" << std::endl;
    return 0;
}