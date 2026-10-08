#ifndef HPMC_PAYOFF_HPP
#define HPMC_PAYOFF_HPP
#include <span>
#include <vector>

struct GbmParameters {
    double spot ;
    double rate ; 
    double volatility ; 
    double maturity ; 
} ; 

std::vector<double> generate_terminal_prices(
    std::span<const double> normal_draws, 
    const GbmParameters& parameters
) ;

#endif
