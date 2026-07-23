#ifndef OPTION_PRICER_HPP
#define OPTION_PRICER_HPP

#include <vector>
#include <cmath>
#include <random>

namespace QuantCore {

struct SimulationResult {
    double price;
    double std_error;
    double execution_time_ms;
};

class SIMDMonteCarloPricer {
public:
    SIMDMonteCarloPricer(double S0, double K, double T, double r, double sigma);
    
    // High-performance vectorized Monte Carlo Option Pricer
    SimulationResult price_european_call(size_t num_simulations = 10000000);
    SimulationResult price_asian_call(size_t num_simulations = 10000000, size_t steps = 252);

private:
    double S0_, K_, T_, r_, sigma_;
};

} // namespace QuantCore

#endif // OPTION_PRICER_HPP
