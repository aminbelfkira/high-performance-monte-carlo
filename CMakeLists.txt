cmake_minimum_required(VERSION 3.20)

project(HighPerformanceMonteCarlo LANGUAGES CXX)

add_library(mc_core STATIC
    src/payoff.cpp
    src/random.cpp
    src/model.cpp
)

target_compile_features(mc_core PUBLIC cxx_std_20)

target_include_directories(mc_core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

add_executable(monte_carlo src/main.cpp)
target_link_libraries(monte_carlo PRIVATE mc_core)

add_executable(payoff_tests tests/test_payoff.cpp)
target_link_libraries(payoff_tests PRIVATE mc_core)

set_target_properties(mc_core monte_carlo payoff_tests PROPERTIES
    CXX_EXTENSIONS OFF
)

enable_testing()
add_executable(random_tests tests/test_random.cpp)

target_link_libraries(random_tests PRIVATE mc_core)

set_target_properties(random_tests PROPERTIES
    CXX_EXTENSIONS OFF
)
add_executable(model_tests tests/test_model.cpp)
target_link_libraries(model_tests PRIVATE mc_core)

set_target_properties(model_tests PROPERTIES
    CXX_EXTENSIONS OFF
)

add_test(NAME terminal_prices COMMAND model_tests)
add_test(NAME normal_generation COMMAND random_tests)
add_test(NAME call_payoff_cases COMMAND payoff_tests)
