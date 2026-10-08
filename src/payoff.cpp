#include "payoff.hpp"
#include <algorithm>

double call_payoff(double final_price, double strike){
    return std::max(final_price - strike, 0.0) ; 
}

CallPayoff::CallPayoff(double strike) :strike_{strike}
{
}
double CallPayoff::operator()(double final_price) const {
    return call_payoff(final_price, strike_) ;
}

