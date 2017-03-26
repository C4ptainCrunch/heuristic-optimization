#include "perturbation.hpp"
#define GAMMA 4
#define LAMBDA 4

Solution ilsPerturbation(Solution& solution) {
    static std::mt19937 gen(42);

    std::uniform_int_distribution<> dis(0, solution.size() - 1);
    std::uniform_int_distribution<> dis2(0, solution.size() - 2);

    for (int i = 0; i < GAMMA; ++i) {
        auto from = dis(gen);
        auto to = dis2(gen);

        auto job = solution[from];
        solution.erase(solution.begin() + from);
        solution.insert(solution.begin() + to, job);
    }

    return solution;
}


bool ilsAccept(const Solution& solution, const Solution& new_solution, const Instance& instance) {
    static std::mt19937 gen(42);

    int processing_times = 0;
    for (int i = 0; i < instance.getNbJob(); ++i) {
        processing_times += instance.getTotalProcessingTime(i);
    }

    auto temperature = LAMBDA * processing_times / 10;

    std::uniform_real_distribution<double> uniform(0, 1);

    return uniform(gen) < std::exp(
        (instance.score(solution) - instance.score(new_solution)) / temperature
    );
}
