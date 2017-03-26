#include "pivoting.hpp"
#include "neighborhood.hpp"

std::experimental::optional<Solution> firstImprovement(
    const Instance &instance,
    const Solution &solution,
    const std::vector<Solution> &solutions) {

    int bestScore = instance.score(solution);
    for(auto newSolution: solutions) {
        if(instance.score(newSolution) < bestScore) {
            return newSolution;
        }
    }

    return {};
}

std::experimental::optional<Solution> bestImprovement(
    const Instance &instance,
    const Solution &solution,
    const std::vector<Solution> &solutions) {

    int bestScore = instance.score(solution);
    Solution bestSolution = solution;
    for(auto newSolution: solutions) {
        int newScore = instance.score(newSolution);
        if(newScore < bestScore) {
            bestSolution = newSolution;
            bestScore = newScore;
        }
    }

    if(solution == bestSolution){
        return {};
    } else {
        return bestSolution;
    }
}
