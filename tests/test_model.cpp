#include "model.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <span>

bool check_prices(std::span<const double> actual,
                  std::span<const double> expected,
                  const char* label) {
    if (actual.size() != expected.size()) {
        std::cerr << label << " : taille incorrecte\n";
        return false;
    }

    constexpr double tolerance = 1e-10;

    for (std::size_t i = 0; i < actual.size(); ++i) {
        if (!(std::abs(actual[i] - expected[i]) <= tolerance)) {
            std::cerr << label << " : prix incorrect a l'indice " << i
                      << ", obtenu " << actual[i]
                      << ", attendu " << expected[i] << '\n';
            return false;
        }
    }

    return true;
}

int main() {
    const std::array<double, 3> draws{-1.0, 0.0, 1.0};

    GbmParameters parameters{
        .spot = 100.0,
        .rate = 0.05,
        .volatility = 0.20,
        .maturity = 0.0
    };

    // Maturité nulle.
    const auto at_zero = generate_terminal_prices(draws, parameters);
    const std::array<double, 3> expected_zero{100.0, 100.0, 100.0};

    if (!check_prices(at_zero, expected_zero, "Maturite nulle")) {
        return 1;
    }

    // Volatilité nulle, maturité différente de 1.
    parameters.maturity = 2.0;
    parameters.volatility = 0.0;

    const auto deterministic = generate_terminal_prices(draws, parameters);
    constexpr double deterministic_price = 110.51709180756476;
    const std::array<double, 3> expected_deterministic{
        deterministic_price, deterministic_price, deterministic_price
    };

    if (!check_prices(deterministic, expected_deterministic,
                      "Volatilite nulle")) {
        return 1;
    }

    parameters.volatility = 0.20;

    const auto stochastic = generate_terminal_prices(draws, parameters);
    const std::array<double, 3> expected_stochastic{
        80.02407072769061,
        106.18365465453596,
        140.89471346891000
    };

    if (!check_prices(stochastic, expected_stochastic, "Cas non trivial")) {
        return 1;
    }

    if (!generate_terminal_prices(
            std::span<const double>{}, parameters).empty()) {
        std::cerr << "Entree vide : resultat non vide\n";
        return 1;
    }

    std::cout << "Les tests du modele ont reussi\n";
    return 0;
}
