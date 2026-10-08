#ifndef HPMC_AGGREGATE_HPP
#define HPMC_AGGREGATE_HPP

#include <span>

template<typename Payoff> 
double sum_payoffs(
    std::span<const double> final_prices,
    const Payoff& payoff
) {
    double accumulator = 0.0 ; 
    for (const double price : final_prices){
        accumulator+= payoff(price) ; 
    }
    return accumulator ; 
}

#endif
