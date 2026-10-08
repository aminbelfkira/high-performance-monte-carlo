#include "model.hpp" 
#include <cmath>
#include <cstddef>

std::vector<double> generate_terminal_prices(std::span<const double> normal_draws, const GbmParameters& parameters){
    std::vector<double> prices(normal_draws.size()) ; 
    const double log_drift = (parameters.rate - 0.5 * parameters.volatility * parameters.volatility)*parameters.maturity ; 
    const double diffusion_scale = parameters.volatility * std::sqrt(parameters.maturity) ; 
    for (std::size_t i = 0 ; i <normal_draws.size(); i++) {
        prices[i] = parameters.spot * std::exp(log_drift + diffusion_scale * normal_draws[i] ) ;
    }
    return prices ; 
} 
