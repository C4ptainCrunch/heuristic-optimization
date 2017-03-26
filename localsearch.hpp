#ifndef LOCALSEARCH_HPP
#define LOCALSEARCH_HPP

#include <functional>
#include "instance.hpp"

Solution rzNeigh(const Instance& instance, Solution& sol);
Solution irz(const Instance& instance, Solution& sol);

#endif
