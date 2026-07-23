#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include <map>
#include <vector>
#include <cstdint>
#include <chrono>

namespace QuantCore {

enum class Side { BUY, SELL };

struct Order {
    uint64_t id;
    Side side;
    double price;
    double quantity;
    uint64_t timestamp;
};

class LimitOrderBook {
public:
    void add_order(uint64_t id, Side side, double price, double quantity);
    void cancel_order(uint64_t id);
    
    double get_best_bid() const;
    double get_best_ask() const;
    double get_mid_price() const;
    
    size_t active_orders_count() const { return order_lookup_.size(); }

private:
    std::map<double, double, std::greater<double>> bids_; // Price -> Total Quantity
    std::map<double, double, std::less<double>> asks_;    // Price -> Total Quantity
    std::map<uint64_t, Order> order_lookup_;
};

} // namespace QuantCore

#endif // ORDERBOOK_HPP
