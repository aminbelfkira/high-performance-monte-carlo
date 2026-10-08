#ifndef HPMC_ENGINE_HPP
#define HPMC_ENGINE_HPP

#include "aggregate.hpp"
#include "model.hpp"
#include "random.hpp"
#include <random>

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

template<typename Payoff>
double monte_carlo_price_streaming(
    const GbmParameters& parameters,
    const Payoff& payoff,
    std::size_t sample_count,
    std::uint64_t seed
){
    if (sample_count ==0){
        throw std::invalid_argument("sample_count must be positive") ; 
    }
    std::mt19937_64 engine{seed} ; 
    std::normal_distribution<double> normal{0.0, 1.0} ; 

    const double log_drift = (parameters.rate - 0.5 * parameters.volatility * parameters.volatility)*parameters.maturity ; 
    const double diffusion_scale = parameters.volatility * std::sqrt(parameters.maturity) ; 
    double aggregated_payoff = 0.0 ; 
    for (std::size_t i = 0 ; i < sample_count ; i++){
        const double z = normal(engine);
        const double final_price = parameters.spot * std::exp(log_drift + diffusion_scale * z);
        aggregated_payoff += payoff(final_price);
    }
    const double discount_factor = std::exp(- parameters.rate * parameters.maturity) ; 
    const double mean_payoff = aggregated_payoff / static_cast<double>(sample_count) ; 
    return discount_factor*mean_payoff ;  
}


#endif
