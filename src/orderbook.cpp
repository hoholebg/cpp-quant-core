#include "orderbook.hpp"

namespace QuantCore {

void LimitOrderBook::add_order(uint64_t id, Side side, double price, double quantity) {
    uint64_t now = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()).count();

    order_lookup_[id] = {id, side, price, quantity, now};

    if (side == Side::BUY) {
        bids_[price] += quantity;
    } else {
        asks_[price] += quantity;
    }
}

void LimitOrderBook::cancel_order(uint64_t id) {
    auto it = order_lookup_.find(id);
    if (it == order_lookup_.end()) return;

    const Order& order = it->second;
    if (order.side == Side::BUY) {
        bids_[order.price] -= order.quantity;
        if (bids_[order.price] <= 0) bids_.erase(order.price);
    } else {
        asks_[order.price] -= order.quantity;
        if (asks_[order.price] <= 0) asks_.erase(order.price);
    }
    order_lookup_.erase(it);
}

double LimitOrderBook::get_best_bid() const {
    return bids_.empty() ? 0.0 : bids_.begin()->first;
}

double LimitOrderBook::get_best_ask() const {
    return asks_.empty() ? 0.0 : asks_.begin()->first;
}

double LimitOrderBook::get_mid_price() const {
    double b = get_best_bid();
    double a = get_best_ask();
    if (b > 0 && a > 0) return (b + a) / 2.0;
    return 0.0;
}

} // namespace QuantCore
