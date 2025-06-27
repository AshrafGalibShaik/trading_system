#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <random>
#include <algorithm>
#include <queue>

// Black-Scholes Option Pricer
class OptionPricer {
private:
    double norm_cdf(double x) {
        return 0.5 * (1.0 + std::erf(x / std::sqrt(2.0)));
    }

public:
    double blackScholesCall(double S, double K, double T, double r, double sigma) {
        double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
        double d2 = d1 - sigma * std::sqrt(T);
        return S * norm_cdf(d1) - K * std::exp(-r * T) * norm_cdf(d2);
    }

    double blackScholesPut(double S, double K, double T, double r, double sigma) {
        double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
        double d2 = d1 - sigma * std::sqrt(T);
        return K * std::exp(-r * T) * norm_cdf(-d2) - S * norm_cdf(-d1);
    }
};

// Order structure for exchange simulator
struct Order {
    int id;
    bool isBuy;
    double price;
    int quantity;
    Order(int _id, bool _isBuy, double _price, int _qty)
        : id(_id), isBuy(_isBuy), price(_price), quantity(_qty) {}
};

// Exchange Simulator
class ExchangeSimulator {
private:
    std::vector<Order> buyOrders;
    std::vector<Order> sellOrders;
    int orderIdCounter;

public:
    ExchangeSimulator() : orderIdCounter(0) {}

    int placeOrder(bool isBuy, double price, int quantity) {
        Order order(orderIdCounter++, isBuy, price, quantity);
        if (isBuy) {
            buyOrders.push_back(order);
            std::sort(buyOrders.begin(), buyOrders.end(),
                [](const Order& a, const Order& b) { return a.price > b.price; });
        } else {
            sellOrders.push_back(order);
            std::sort(sellOrders.begin(), sellOrders.end(),
                [](const Order& a, const Order& b) { return a.price < b.price; });
        }
        matchOrders();
        return order.id;
    }

    void matchOrders() {
        while (!buyOrders.empty() && !sellOrders.empty()) {
            if (buyOrders[0].price >= sellOrders[0].price) {
                int quantity = std::min(buyOrders[0].quantity, sellOrders[0].quantity);
                std::cout << "Trade executed: " << quantity << " units at $"
                    << sellOrders[0].price << std::endl;

                buyOrders[0].quantity -= quantity;
                sellOrders[0].quantity -= quantity;

                if (buyOrders[0].quantity == 0) buyOrders.erase(buyOrders.begin());
                if (sellOrders[0].quantity == 0) sellOrders.erase(sellOrders.begin());
            } else {
                break;
            }
        }
    }

    void printOrderBook() {
        std::cout << "\nBuy Orders:\n";
        for (const auto& order : buyOrders) {
            std::cout << "ID: " << order.id << ", Price: $" << order.price
                << ", Qty: " << order.quantity << std::endl;
        }
        std::cout << "\nSell Orders:\n";
        for (const auto& order : sellOrders) {
            std::cout << "ID: " << order.id << ", Price: $" << order.price
                << ", Qty: " << order.quantity << std::endl;
        }
    }
};

// Back-tester
class BackTester {
private:
    std::vector<double> historicalPrices;
    OptionPricer pricer;

public:
    void addHistoricalPrice(double price) {
        historicalPrices.push_back(price);
    }

    double testOptionStrategy(double K, double T, double r, double sigma, bool isCall) {
        double totalProfit = 0.0;
        double avgPrice = 0.0;
        if (!historicalPrices.empty()) {
            avgPrice = std::accumulate(historicalPrices.begin(),
                historicalPrices.end(), 0.0) /
                historicalPrices.size();
        }
        for (double S : historicalPrices) {
            double optionPrice = isCall ?
                pricer.blackScholesCall(S, K, T, r, sigma) :
                pricer.blackScholesPut(S, K, T, r, sigma);
            // Simple strategy: buy option if price is below average
            if (S < avgPrice) {
                totalProfit += optionPrice;
            }
        }
        return totalProfit;
    }
};

// Main function to demonstrate usage
int main() {
    // Initialize components
    OptionPricer pricer;
    ExchangeSimulator exchange;
    BackTester backtester;

    // Test Option Pricer
    double S = 100.0;  // Stock price
    double K = 100.0;  // Strike price
    double T = 1.0;    // Time to expiration (years)
    double r = 0.05;   // Risk-free rate
    double sigma = 0.2;// Volatility

    std::cout << "Call Option Price: $"
        << pricer.blackScholesCall(S, K, T, r, sigma) << std::endl;
    std::cout << "Put Option Price: $"
        << pricer.blackScholesPut(S, K, T, r, sigma) << std::endl;

    // Test Exchange Simulator
    exchange.placeOrder(true, 100.0, 100);  // Buy order
    exchange.placeOrder(true, 99.0, 50);    // Buy order
    exchange.placeOrder(false, 100.0, 75);  // Sell order
    exchange.placeOrder(false, 101.0, 25);  // Sell order
    exchange.printOrderBook();

    // Test Back-tester
    backtester.addHistoricalPrice(95.0);
    backtester.addHistoricalPrice(100.0);
    backtester.addHistoricalPrice(105.0);
    backtester.addHistoricalPrice(98.0);

    double profit = backtester.testOptionStrategy(K, T, r, sigma, true);
    std::cout << "\nBack-test profit from call option strategy: $"
        << profit << std::endl;

    return 0;
}