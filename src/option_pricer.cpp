#include "option_pricer.hpp"
#include <chrono>
#include <numeric>
#include <iostream>

namespace QuantCore {

SIMDMonteCarloPricer::SIMDMonteCarloPricer(double S0, double K, double T, double r, double sigma)
    : S0_(S0), K_(K), T_(T), r_(r), sigma_(sigma) {}

SimulationResult SIMDMonteCarloPricer::price_european_call(size_t num_simulations) {
    auto start = std::chrono::high_resolution_clock::now();

    double drift = (r_ - 0.5 * sigma_ * sigma_) * T_;
    double vol_sqrt_T = sigma_ * std::sqrt(T_);
    double discount = std::exp(-r_ * T_);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0.0, 1.0);

    double sum_payoffs = 0.0;
    double sum_sq_payoffs = 0.0;

    for (size_t i = 0; i < num_simulations; ++i) {
        double z = dist(gen);
        double ST = S0_ * std::exp(drift + vol_sqrt_T * z);
        double payoff = std::max(0.0, ST - K_);
        sum_payoffs += payoff;
        sum_sq_payoffs += payoff * payoff;
    }

    auto end = std::chrono::high_resolution_clock::now();
    double time_ms = std::chrono::duration<double, std::milli>(end - start).count();

    double mean_payoff = sum_payoffs / num_simulations;
    double price = mean_payoff * discount;
    double variance = (sum_sq_payoffs / num_simulations) - (mean_payoff * mean_payoff);
    double stderr_val = std::sqrt(variance / num_simulations) * discount;

    return {price, stderr_val, time_ms};
}

SimulationResult SIMDMonteCarloPricer::price_asian_call(size_t num_simulations, size_t steps) {
    auto start = std::chrono::high_resolution_clock::now();
    double dt = T_ / steps;
    double drift = (r_ - 0.5 * sigma_ * sigma_) * dt;
    double vol_dt = sigma_ * std::sqrt(dt);
    double discount = std::exp(-r_ * T_);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0.0, 1.0);

    double sum_payoffs = 0.0;

    for (size_t i = 0; i < num_simulations; ++i) {
        double current_S = S0_;
        double path_sum = current_S;

        for (size_t t = 1; t <= steps; ++t) {
            current_S *= std::exp(drift + vol_dt * dist(gen));
            path_sum += current_S;
        }

        double average_price = path_sum / (steps + 1);
        sum_payoffs += std::max(0.0, average_price - K_);
    }

    auto end = std::chrono::high_resolution_clock::now();
    double time_ms = std::chrono::duration<double, std::milli>(end - start).count();

    double price = (sum_payoffs / num_simulations) * discount;
    return {price, 0.0, time_ms};
}

} // namespace QuantCore
