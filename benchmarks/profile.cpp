#include "engine.hpp"
#include "model.hpp"
#include "payoff.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

struct Measurement {
    std::uint64_t seed;
    double price;
    double seconds;
};

int main() {
    constexpr std::size_t sample_count = 1'000'000;

    const CallPayoff payoff{100.0};

    const GbmParameters parameters{
        .spot = 100.0,
        .rate = 0.05,
        .volatility = 0.20,
        .maturity = 1.0
    };

    double price_sum = 0.0;

    for (std::uint64_t seed = 42; seed < 1042; ++seed) {
        //price_sum +=
        //    monte_carlo_price(parameters, payoff, sample_count, seed);
        price_sum += monte_carlo_price_streaming(
            parameters, payoff, sample_count, seed);
    }

    std::cout << "Price sum : " << price_sum << "\n" ; 

    return 0;
}
