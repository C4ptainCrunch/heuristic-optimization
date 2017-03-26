#include "localsearch.hpp"
#include <iostream>

Solution rzNeigh(const Instance& instance, Solution& solution) {
    auto best_solution = solution;
    auto best_score = instance.score(solution);

    for (size_t i=0; i < solution.size(); ++i) {
        for (size_t j=0; j < solution.size(); ++j) {
            auto job = solution[i];
            solution.erase(solution.begin() + i);
            solution.insert(solution.begin() + j, job);
            auto score = instance.score(solution);
            if(score < best_score){
                best_solution = solution;
                best_score = score;
            }
            solution.erase(solution.begin() + j);
            solution.insert(solution.begin() + i, job);
        }
    }
    return best_solution;
}


Solution irz(const Instance& instance, Solution& sol) {
    auto new_sol = sol;
    do {
        sol = new_sol;
        new_sol = rzNeigh(instance, sol);
    } while(sol != new_sol);
    return new_sol;
}
