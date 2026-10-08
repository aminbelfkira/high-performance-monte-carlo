#ifndef HPMC_RANDOM_HPP
#define HPMC_RANDOM_HPP
#include <cstddef>
#include <cstdint>
#include <vector>

std::vector<double> generate_standard_normals(std::size_t count, std::uint64_t seed);

#endif
