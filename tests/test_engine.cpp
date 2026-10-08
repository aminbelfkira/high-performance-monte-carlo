#include "engine.hpp"
#include "payoff.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>

bool check_close(double actual, double expected, const char* label) {
    constexpr double tolerance = 1e-10;

    if (!(std::abs(actual - expected) <= tolerance)) {
        std::cerr << std::setprecision(17)
                  << label << " : obtenu " << actual
                  << ", attendu " << expected << '\n';
        return false;
    }

    return true;
}

int main() {
    const double strike = 100.0;
    const CallPayoff payoff{strike};

    GbmParameters parameters{
        .spot = 120.0,
        .rate = 0.05,
        .volatility = 0.20,
        .maturity = 0.0
    };

    // Maturité nulle : payoff immédiat, sans actualisation.
    if (!check_close(monte_carlo_price(parameters, payoff, 64, 42),
                     20.0, "Maturite nulle")) {
        return 1;
    }

    // Volatilité nulle : résultat déterministe.
    parameters.spot = 100.0;
    parameters.volatility = 0.0;
    parameters.maturity = 2.0;

    if (!check_close(monte_carlo_price(parameters, payoff, 64, 42),
                     9.51625819640404, "Volatilite nulle")) {
        return 1;
    }

    parameters.volatility = 0.20;
    const auto constant_payoff = [](double) { return 1.0; };

    if (!check_close(monte_carlo_price(parameters, constant_payoff, 17, 42),
                     0.9048374180359596, "Payoff constant")) {
        return 1;
    }

    const double first = monte_carlo_price(parameters, payoff, 512, 42);
    const double second = monte_carlo_price(parameters, payoff, 512, 42);

    if (!std::isfinite(first) || first < 0.0 || first != second) {
        std::cerr << "Echec : resultat invalide ou non reproductible\n";
        return 1;
    }

    const auto lambda_payoff = [strike](double final_price) {
        return call_payoff(final_price, strike);
    };

    const double lambda_price =
        monte_carlo_price(parameters, lambda_payoff, 512, 42);

    if (lambda_price != first) {
        std::cerr << "Echec : classe et lambda donnent des prix differents\n";
        return 1;
    }

    bool rejected = false;

    try {
        monte_carlo_price(parameters, payoff, 0, 42);
    } catch (const std::invalid_argument&) {
        rejected = true;
    }

    if (!rejected) {
        std::cerr << "Echec : zero echantillon accepte\n";
        return 1;
    }

    std::cout << "Les tests du moteur ont reussi\n";
    return 0;
}
