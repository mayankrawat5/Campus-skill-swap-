#ifndef SKILL_GRAPH_H
#define SKILL_GRAPH_H

#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>

class SkillGraph {
private:
    std::map<std::string, std::vector<std::string>> adjacencyList; // skill -> list of related skills

public:
    void addEdge(std::string skill1, std::string skill2);

    // BFS to find path from startSkill to targetSkill
    std::vector<std::string> findLearningPath(std::string startSkill, std::string targetSkill);

    void displayGraph() const;
};

#endif // SKILL_GRAPH_H
