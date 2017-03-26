#ifndef PERTURBATION_HPP
#define PERTURBATION_HPP

#include <functional>
#include <random>
#include <cmath>
#include "instance.hpp"

Solution ilsPerturbation(Solution& sol);
bool ilsAccept(const Solution& solution, const Solution& new_solution, const Instance& instance);

#endif
