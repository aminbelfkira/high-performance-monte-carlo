#include "random.hpp"
#include <random>

std::vector<double> generate_standard_normals(std::size_t count, std::uint64_t seed){
    std::mt19937_64 engine{seed} ; 
    std::normal_distribution<double> normal{0.0, 1.0} ; 
    std::vector<double> numbers(count) ;  
    for (double &number : numbers){
        number = normal(engine) ; 
    }

    return numbers ; 
}
