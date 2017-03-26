#ifndef GENETIC_HPP
#define GENETIC_HPP

#include <vector>
#include <random>
#include <algorithm>
#include "instance.hpp"
#include "const.hpp"

std::vector<Solution> crossover(std::vector<Solution> population);
std::vector<Solution> crossoverElite(std::vector<Solution> population, const Instance& instance);

std::vector<Solution> selectBestPopulation(std::vector<Solution> population, const Instance& instance);
std::vector<Solution> selectUniformPopulation(std::vector<Solution> population, const Instance& instance);
std::vector<Solution> selectRoulettePopulation(std::vector<Solution> population, const Instance& instance);


#endif
