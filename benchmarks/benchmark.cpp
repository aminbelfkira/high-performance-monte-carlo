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

    double warmup_sum = 0.0;

    for (std::uint64_t seed = 39; seed < 42; ++seed) {
        warmup_sum +=
            monte_carlo_price(parameters, payoff, sample_count, seed);
    }

    std::vector<Measurement> measurements;
    measurements.reserve(10);

    for (std::uint64_t seed = 42; seed < 52; ++seed) {
        const auto start = std::chrono::steady_clock::now();

        const double price =
            monte_carlo_price(parameters, payoff, sample_count, seed);

        const auto stop = std::chrono::steady_clock::now();

        const double seconds =
            std::chrono::duration<double>(stop - start).count();

        measurements.push_back({seed, price, seconds});
    }

    std::cout << std::setprecision(17);
    std::cout << "Samples per run: " << sample_count << '\n';
    std::cout << "Warmup sum: " << warmup_sum << '\n';

    for (const auto& measurement : measurements) {
        std::cout << "Seed: " << measurement.seed
                  << ", price: " << measurement.price
                  << ", seconds: " << measurement.seconds << '\n';
    }

    return 0;
}
