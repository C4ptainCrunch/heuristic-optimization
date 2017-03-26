#ifndef PIVOTING_HPP
#define PIVOTING_HPP

#include <functional>
#include <experimental/optional>
#include <iostream>
#include "instance.hpp"

std::experimental::optional<Solution> firstImprovement(
    const Instance &instance,
    const Solution &solution,
    const std::vector<Solution> &solutions
);

std::experimental::optional<Solution> bestImprovement(
    const Instance &instance,
    const Solution &solution,
    const std::vector<Solution> &solutions
);


#endif
