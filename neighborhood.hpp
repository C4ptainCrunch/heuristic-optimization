#ifndef NEIGHBORHOOD_HPP
#define NEIGHBORHOOD_HPP

#include <functional>
#include "instance.hpp"

std::vector<Solution> transpose(const Solution &solution);
std::vector<Solution> exchange(const Solution &solution);
std::vector<Solution> insert(const Solution &solution);


#endif
