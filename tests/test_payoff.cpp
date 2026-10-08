#include <iostream>
#include "../src/payoff.hpp"

int main(){
    if (call_payoff(120.0, 100.0) != 20.0) {
        std::cerr << "Echec : prix final superieur au strike\n";
        return 1;
    }

    if (call_payoff(100.0, 100.0) != 0.0) {
        std::cerr << "Echec : prix final egal au strike\n";
        return 1;
    }

    if (call_payoff(80.0, 100.0) != 0.0) {
        std::cerr << "Echec : prix final inferieur au strike\n";
        return 1;
    }

    if (call_payoff(110.75, 100.5) != 10.25) {
        std::cerr << "Echec : prix avec partie decimale\n";
        return 1;
    }

    std::cout << "Les 4 verifications ont reussi\n";
    return 0;
}
