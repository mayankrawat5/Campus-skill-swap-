#ifndef SKILL_H
#define SKILL_H

#include <string>

// Skill information with level
struct SkillInfo {
    std::string skillName;
    std::string level; // Beginner, Intermediate, Advanced

    SkillInfo() : skillName(""), level("Beginner") {}
    SkillInfo(std::string name, std::string lvl) : skillName(name), level(lvl) {}
};

#endif // SKILL_H