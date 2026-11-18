#include "skill_graph.h"
#include <iostream>

void SkillGraph::addEdge(std::string skill1, std::string skill2) {
    // Directed edge: skill1 -> skill2 (learn skill1 then skill2)
    adjacencyList[skill1].push_back(skill2);
    // Also add reverse edge for bidirectional exploration if needed
    // adjacencyList[skill2].push_back(skill1);
}

// BFS to find path from startSkill to targetSkill
std::vector<std::string> SkillGraph::findLearningPath(std::string startSkill, std::string targetSkill) {
    if (startSkill == targetSkill) {
        return {startSkill};
    }
    
    std::map<std::string, bool> visited;
    std::map<std::string, std::string> parent;
    std::queue<std::string> q;
    
    q.push(startSkill);
    visited[startSkill] = true;
    parent[startSkill] = "";
    
    while (!q.empty()) {
        std::string current = q.front();
        q.pop();
        
        if (adjacencyList.find(current) != adjacencyList.end()) {
            for (std::string neighbor : adjacencyList[current]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    parent[neighbor] = current;
                    q.push(neighbor);
                    
                    if (neighbor == targetSkill) {
                        // Reconstruct path
                        std::vector<std::string> path;
                        std::string step = targetSkill;
                        while (step != "") {
                            path.insert(path.begin(), step);
                            step = parent[step];
                        }
                        return path;
                    }
                }
            }
        }
    }
    
    return {}; // Empty path if not found
}

void SkillGraph::displayGraph() const {
    std::cout << "Skill Evolution Map:" << std::endl;
    for (const auto& pair : adjacencyList) {
        std::cout << pair.first << " -> ";
        if (!pair.second.empty()) {
            for (size_t i = 0; i < pair.second.size(); ++i) {
                std::cout << pair.second[i];
                if (i < pair.second.size() - 1) std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }
}
