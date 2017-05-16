#include <iostream>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <cstring>
#include <math.h>

#include "instance.hpp"
#include "initialization.hpp"
#include "localsearch.hpp"
#include "perturbation.hpp"
#include "genetic.hpp"

#include "const.hpp"

int cutoff = 0;

Solution simple(const Instance& instance) {
    Solution sol = lrInit(instance);
    sol = irz(instance, sol);
    auto best_solution  = sol;
    std::cout << instance.score(sol) << std::endl;
    const clock_t begin_time = clock();

    while((float( clock () - begin_time ) /  CLOCKS_PER_SEC) <= cutoff) {
        auto new_sol =  ilsPerturbation(sol);
        new_sol = irz(instance, new_sol);
        if(instance.score(new_sol) < instance.score(sol)) {
            sol = new_sol;
            if(instance.score(new_sol) < instance.score(best_solution)) {
                best_solution = new_sol;
                std::cout << ((float( clock () - begin_time ) /  CLOCKS_PER_SEC)) << " " <<  instance.score(best_solution) << std::endl;
            }
        } else {
            if(ilsAccept(sol, new_sol, instance)) {
                sol = new_sol;
                std::cout << ((float( clock () - begin_time ) /  CLOCKS_PER_SEC)) << " " << instance.score(sol) << std::endl;
            }
        }
    }
    return best_solution;

}


Solution genetic(const Instance& instance) {
    static std::mt19937 gen(42);
    std::uniform_real_distribution<double> uniform(0, 1);

    // Initialization
    auto population = std::vector<Solution>();
    population.push_back(lrInit(instance));

    for (int i=0; i < S - 1; ++i) {
        population.push_back(randomPermutation(instance));
    }

    auto best_solution = population[0];
    auto best_score = instance.score(best_solution);
    size_t unsuccessfull = 0;

    // Run
    const clock_t begin_time = clock();
    while((float( clock () - begin_time ) /  CLOCKS_PER_SEC) <= cutoff) {

        // Generate a new population
        auto next_population = crossoverElite(population, instance);

        // Mutate solutions
        std::transform(
            next_population.begin(), next_population.end(), next_population.begin(),
            [&uniform, unsuccessfull](Solution x) {
                if(uniform(gen) < (MUT_RATE * sqrt(unsuccessfull + 1))){
                    return ilsPerturbation(x);
                } else {
                    return x;
                }
            }
        );

        // Apply RZ on each sol
        std::transform(
            next_population.begin(), next_population.end(), next_population.begin(),
            [&instance, unsuccessfull](Solution x) {
                if(unsuccessfull > 5) {
                    return irz(instance, x);
                } else {
                    return rzNeigh(instance, x);
                }
            }
        );

        // Merge the old an new pop
        population.insert(population.end(), next_population.begin(), next_population.end());

        // Select a subset of the pop
        population = selectBestPopulation(population, instance);


        // Find the best chromosome
        auto pos_of_best = std::min_element(
            population.begin(), population.end(),
            [&instance](const Solution &a, const Solution &b) -> bool{
            return instance.score(a) < instance.score(b);
            }
        );

        auto best_of_population = population[std::distance(population.begin(), pos_of_best)];

        // Store it if it's the best
        auto score = instance.score(best_of_population);
        if(score < best_score) {
            best_score = score,
            best_solution = best_of_population;
            std::cout << ((float( clock () - begin_time ) /  CLOCKS_PER_SEC)) << " " << best_score << std::endl;
            unsuccessfull = 0;
        } else {
            unsuccessfull++;
        }

    }

    return best_solution;
}

int main(int argc, char *argv[])
{
    if(argc != 4) {
        std::cout << "Provide input files, cutoff time, algo" << std::endl;
        exit(-1);
    }

    cutoff = atoi(argv[2]);

    std::function<Solution(const Instance&)> algo;

    if(strcmp(argv[3], "genetic") == 0) {
        algo = genetic;
    } else if(strcmp(argv[3], "ils") == 0) {
        algo = simple;
    } else {
        std::cout << "Unknown algo" << std::endl;
        exit(-1);
    }

    std::srand(42);

    char const * fn = argv[1];
    Instance instance = Instance(fn);

    auto solution = algo(instance);
    std::cout << instance.score(solution) << std::endl;

    return 0;
}
