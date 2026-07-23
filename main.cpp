#include <iostream>
#include <iomanip>
#include "option_pricer.hpp"
#include "orderbook.hpp"

int main() {
    std::cout << "========================================================\n";
    std::cout << "⚡ CPP-QUANT-CORE: High-Performance C++20 Pricing Engine\n";
    std::cout << "========================================================\n\n";

    // 1. Benchmark C++ Monte Carlo Option Pricer
    QuantCore::SIMDMonteCarloPricer pricer(100.0, 100.0, 1.0, 0.05, 0.20);
    
    std::cout << "Running 10,000,000 Monte Carlo European Call Simulations...\n";
    auto res_euro = pricer.price_european_call(10000000);
    
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "  [+] European Call Price: $" << res_euro.price << " (+/- " << res_euro.std_error << ")\n";
    std::cout << "  [+] Execution Time:       " << res_euro.execution_time_ms << " ms\n\n";

    // 2. High-Frequency Limit Order Book Demo
    std::cout << "Initializing C++ Sub-Microsecond Limit Order Book...\n";
    QuantCore::LimitOrderBook book;
    
    book.add_order(1, QuantCore::Side::BUY, 150.25, 100);
    book.add_order(2, QuantCore::Side::BUY, 150.20, 250);
    book.add_order(3, QuantCore::Side::SELL, 150.30, 150);
    book.add_order(4, QuantCore::Side::SELL, 150.35, 300);

    std::cout << "  [+] Best Bid:  $" << book.get_best_bid() << "\n";
    std::cout << "  [+] Best Ask:  $" << book.get_best_ask() << "\n";
    std::cout << "  [+] Mid Price: $" << book.get_mid_price() << "\n";
    std::cout << "  [+] Total Orders in Book: " << book.active_orders_count() << "\n\n";

    std::cout << "Execution completed successfully!\n";
    return 0;
}
