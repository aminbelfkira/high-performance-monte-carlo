#ifndef HPMC_PAYOFF_CONCEPT_HPP
#define HPMC_PAYOFF_CONCEPT_HPP

#include <concepts>

template <typename T>
concept PayoffFunction = requires(const T &payoff, double price) {
  { payoff(price) } -> std::convertible_to<double>;
};

#endif