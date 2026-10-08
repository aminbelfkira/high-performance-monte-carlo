#include <iostream>
#include <array>
#include "model.hpp"
#include "payoff.hpp"
#include "aggregate.hpp"
#include "engine.hpp"
#include <cstdint>

int main(){
    const CallPayoff payoff{100.0} ; 
    const GbmParameters parameters{
        .spot = 100.0,
        .rate = 0.05,
        .volatility = 0.20,
        .maturity = 1.0
    };
    const std::uint64_t seed = 42;
    std::cout << monte_carlo_price(parameters, payoff, 1000, seed) << "\n" ; 

    return 0 ;
}
