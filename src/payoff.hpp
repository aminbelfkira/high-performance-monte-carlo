#ifndef HPMC_PAYOFF_HPP
#define HPMC_PAYOFF_HPP

double call_payoff(double final_price, double strike) ; 

class CallPayoff {
    public : 
        explicit CallPayoff(double strike) ;

        double operator() (double final_price) const ; 
    private : 
        double strike_;
} ; 

#endif
