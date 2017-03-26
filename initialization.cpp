#include <iostream>
#include <algorithm>
#include <limits>
#include "initialization.hpp"


Solution orderedSolution(const Instance &instance) {
    int nJobs = instance.getNbJob();
    Solution solution = Solution(nJobs);
    for (int i = 0; i < nJobs; ++i) {
        solution[i] = i;
    }
    return solution;
}

Solution randomPermutation(const Instance &instance) {
    Solution solution = orderedSolution(instance);
    std::random_shuffle(solution.begin(), solution.end());

    return solution;
}

Solution simplifiedRZHeuristic(const Instance &instance){
    Solution jobs_by_weight = orderedSolution(instance);
    Solution RZsolution = Solution();

    std::stable_sort(
        jobs_by_weight.begin(),
        jobs_by_weight.end(),
        [&instance](const int &a, const int &b) -> bool{
            return instance.weightedProcessingTime(a) > instance.weightedProcessingTime(b);
        }
    );


    RZsolution.push_back(jobs_by_weight.back());
    jobs_by_weight.pop_back();

    while(jobs_by_weight.size() != 0) {
        int toBeInserted = jobs_by_weight.back();
        jobs_by_weight.pop_back();

        auto scores = std::vector<int>();
        for (unsigned int i = 0; i <= RZsolution.size(); ++i) {
            RZsolution.insert(RZsolution.begin() + i, toBeInserted);
            scores.push_back(instance.score(RZsolution));
            RZsolution.erase(RZsolution.begin() + i);
        }
        int best_position = std::distance(
            scores.begin(),
            std::min_element(scores.begin(), scores.end())
        );
        RZsolution.insert(RZsolution.begin() + best_position, toBeInserted);

    }
    return RZsolution;
}


Solution lrInit(const Instance &instance) {
    Solution solution = orderedSolution(instance);

    std::stable_sort(
        solution.begin(),
        solution.end(),
        [&instance](const int &a, const int &b) -> bool{
            return instance.weightedProcessingTime(a) < instance.weightedProcessingTime(b);
        }
    );

    auto best_score = std::numeric_limits<int>::max();
    Solution best_solution;

    size_t LRX = instance.getNbJob() / instance.getNbMac();
    for (size_t i=0; i < LRX; ++i){
        auto job = solution[i];
        solution.erase(solution.begin() + i);
        solution.insert(solution.begin(), job);
        auto score = instance.score(solution);
        if(score < best_score){
            best_solution = solution;
            best_score = score;
        }
        solution.erase(solution.begin());
        solution.insert(solution.begin() + i, job);
    }

    return best_solution;
}
