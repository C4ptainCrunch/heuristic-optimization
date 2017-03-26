#ifndef INITIALIZATION_HPP
#define INITIALIZATION_HPP

#include "instance.hpp"

Solution orderedSolution(const Instance &instance);
Solution randomPermutation(const Instance &instance);
Solution simplifiedRZHeuristic(const Instance &instance);
Solution lrInit(const Instance &instance);

#endif
