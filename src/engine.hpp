#ifndef HPMC_ENGINE_HPP
#define HPMC_ENGINE_HPP

#include "aggregate.hpp"
#include "model.hpp"
#include "random.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>

template<typename Payoff>
double monte_carlo_price(
    const GbmParameters& parameters,
    const Payoff& payoff,
    std::size_t sample_count, 
    std::uint64_t seed
){
    if (sample_count ==0){
        throw std::invalid_argument("sample_count must be positive") ; 
    }
    const std::vector<double> normal_draws = generate_standard_normals(sample_count, seed ) ; 
    const std::vector<double> prices = generate_terminal_prices(normal_draws, parameters) ;
    const double aggregated_payoff = sum_payoffs(prices, payoff) ;
    const double mean_payoff = aggregated_payoff / static_cast<double>(sample_count) ; 
    const double discount_factor = std::exp(- parameters.rate * parameters.maturity) ; 
    return discount_factor*mean_payoff ;  
}



#endif
