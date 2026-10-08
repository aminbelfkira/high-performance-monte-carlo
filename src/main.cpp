#include <iostream>
#include <array>
#include "payoff.hpp"
#include "aggregate.hpp"

int main(){
    const CallPayoff first{100.0} ; 
    const CallPayoff second {110.0} ; 

    double strike = 100.0 ; 
    
    const auto lambda_payoff = [&strike](double final_price) {
        return call_payoff(final_price , strike) ; 
    } ; 

    std::cout << lambda_payoff(120.0) << "\n" ;

    strike = 110.0 ; 
    std::cout << lambda_payoff(120.0) << "\n" ;

    const std::array<double, 4> prices{80.0, 100.0,120.0, 130.0} ; 
    const CallPayoff payoff{100.0} ; 

    std::cout << sum_payoffs(prices, payoff) << "\n" ; 

    return 0 ;
}
