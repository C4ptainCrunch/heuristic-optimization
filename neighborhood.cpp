#include "neighborhood.hpp"


std::vector<Solution> transpose(const Solution &solution) {
    std::vector<Solution> solutions;
    for (size_t i = 0; i < solution.size()-1; ++i) {
        Solution newSolution = Solution(solution);
        std::swap(newSolution[i], newSolution[i+1]);
        solutions.push_back(newSolution);
    }
    return solutions;
}

std::vector<Solution> exchange(const Solution &solution) {
    std::vector<Solution> solutions;
    for (size_t i = 0; i < solution.size()-1; ++i) {
        for (size_t j = i+1; j < solution.size(); ++j) {
            Solution newSolution = Solution(solution);
            std::swap(newSolution[i], newSolution[j]);
            solutions.push_back(newSolution);
        }
    }
    return solutions;
}

std::vector<Solution> insert(const Solution &solution) {
    std::vector<Solution> solutions;
    for (size_t i = 0; i < solution.size()-1; ++i) {
        for (size_t j = i+1; j < solution.size(); ++j) {
            Solution newSolution = Solution(solution);
            int element = newSolution[i];
            newSolution.erase(newSolution.begin() + i);
            newSolution.insert(newSolution.begin() + j-1, element);
            solutions.push_back(newSolution);
        }
    }
    return solutions;
}

