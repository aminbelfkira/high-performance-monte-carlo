#include "engine.hpp"
#include "model.hpp"
#include "payoff.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>

struct TimedResult {
    double price;
    double seconds;
};

struct Measurement {
    std::uint64_t seed;
    TimedResult reference;
    TimedResult streaming;
};

template<typename Function>
TimedResult measure(const Function& function) {
    const auto start = std::chrono::steady_clock::now();

    const double price = function();

    const auto stop = std::chrono::steady_clock::now();

    const double seconds =
        std::chrono::duration<double>(stop - start).count();

    return {price, seconds};
}

template<std::size_t Count>
double median(std::array<double, Count> values) {
    static_assert(Count > 0);

    std::sort(values.begin(), values.end());

    if constexpr (Count % 2 == 0) {
        return (values[Count / 2 - 1] + values[Count / 2]) / 2.0;
    } else {
        return values[Count / 2];
    }
}

int main() {
    constexpr std::size_t sample_count = 1'000'000;
    constexpr std::size_t run_count = 10;

    const CallPayoff payoff{100.0};

    const GbmParameters parameters{
        .spot = 100.0,
        .rate = 0.05,
        .volatility = 0.20,
        .maturity = 1.0
    };

    double reference_warmup_sum = 0.0;
    double streaming_warmup_sum = 0.0;

    for (std::uint64_t seed = 39; seed < 42; ++seed) {
        reference_warmup_sum +=
            monte_carlo_price(parameters, payoff, sample_count, seed);

        streaming_warmup_sum +=
            monte_carlo_price_streaming(
                parameters, payoff, sample_count, seed);
    }

    std::array<Measurement, run_count> measurements{};

    for (std::size_t i = 0; i < run_count; ++i) {
        const std::uint64_t seed = 42 + i;

        const auto run_reference = [&] {
            return monte_carlo_price(
                parameters, payoff, sample_count, seed);
        };

        const auto run_streaming = [&] {
            return monte_carlo_price_streaming(
                parameters, payoff, sample_count, seed);
        };

        measurements[i].seed = seed;

        if (i % 2 == 0) {
            measurements[i].reference = measure(run_reference);
            measurements[i].streaming = measure(run_streaming);
        } else {
            measurements[i].streaming = measure(run_streaming);
            measurements[i].reference = measure(run_reference);
        }
    }

    std::array<double, run_count> reference_times{};
    std::array<double, run_count> streaming_times{};

    std::cout << std::setprecision(17);
    std::cout << "Samples per run: " << sample_count << '\n';
    std::cout << "Reference warmup sum: "
              << reference_warmup_sum << '\n';
    std::cout << "Streaming warmup sum: "
              << streaming_warmup_sum << '\n';

    for (std::size_t i = 0; i < run_count; ++i) {
        const auto& measurement = measurements[i];

        const double reference_price = measurement.reference.price;
        const double streaming_price = measurement.streaming.price;

        const double tolerance =
            1e-10 + 1e-12 *
                std::max(std::abs(reference_price),
                         std::abs(streaming_price));

        if (!std::isfinite(reference_price) ||
            !std::isfinite(streaming_price) ||
            std::abs(reference_price - streaming_price) > tolerance) {
            std::cerr << "Echec : prix differents pour la seed "
                      << measurement.seed << '\n';
            std::cerr << "Reference: " << reference_price
                      << ", streaming: " << streaming_price << '\n';
            return 1;
        }

        reference_times[i] = measurement.reference.seconds;
        streaming_times[i] = measurement.streaming.seconds;

        std::cout << "\nSeed: " << measurement.seed << '\n';

        std::cout << "Reference: price=" << reference_price
                  << ", seconds=" << measurement.reference.seconds
                  << '\n';

        std::cout << "Streaming: price=" << streaming_price
                  << ", seconds=" << measurement.streaming.seconds
                  << '\n';
    }

    const double reference_median = median(reference_times);
    const double streaming_median = median(streaming_times);

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "\nReference median: "
              << reference_median * 1000.0 << " ms\n";
    std::cout << "Streaming median: "
              << streaming_median * 1000.0 << " ms\n";

    std::cout << "Ratio reference / streaming: "
              << reference_median / streaming_median << "x\n";

    return 0;
}
