# Financial Trading Toolkit

A comprehensive C++ toolkit for quantitative finance applications, featuring option pricing, exchange simulation, and backtesting capabilities. This toolkit provides essential building blocks for algorithmic trading research and development.

## Features

### 1. Black-Scholes Option Pricer
- **Call Option Pricing**: Calculate theoretical prices for European call options
- **Put Option Pricing**: Calculate theoretical prices for European put options
- **Mathematical Foundation**: Implements the classic Black-Scholes formula with normal cumulative distribution function

### 2. Exchange Simulator
- **Order Book Management**: Maintains separate buy and sell order queues
- **Automatic Order Matching**: Executes trades when buy and sell orders cross
- **Price-Time Priority**: Orders sorted by best price (highest bid, lowest ask)
- **Real-time Trade Execution**: Immediate matching and execution reporting

### 3. Backtesting Engine
- **Historical Data Processing**: Analyze option strategies against historical price data
- **Strategy Testing**: Test simple option trading strategies with profit calculation
- **Performance Metrics**: Calculate total returns from backtested strategies

## Technical Implementation

### Core Classes

#### `OptionPricer`
```cpp
double blackScholesCall(double S, double K, double T, double r, double sigma)
double blackScholesPut(double S, double K, double T, double r, double sigma)
```

#### `ExchangeSimulator`
```cpp
int placeOrder(bool isBuy, double price, int quantity)
void matchOrders()
void printOrderBook()
```

#### `BackTester`
```cpp
void addHistoricalPrice(double price)
double testOptionStrategy(double K, double T, double r, double sigma, bool isCall)
```

## Usage Example

```cpp
// Initialize components
OptionPricer pricer;
ExchangeSimulator exchange;
BackTester backtester;

// Price options
double callPrice = pricer.blackScholesCall(100.0, 100.0, 1.0, 0.05, 0.2);
double putPrice = pricer.blackScholesPut(100.0, 100.0, 1.0, 0.05, 0.2);

// Simulate trading
exchange.placeOrder(true, 100.0, 100);   // Buy order
exchange.placeOrder(false, 100.0, 75);   // Sell order

// Backtest strategy
backtester.addHistoricalPrice(95.0);
backtester.addHistoricalPrice(105.0);
double profit = backtester.testOptionStrategy(100.0, 1.0, 0.05, 0.2, true);
```

## Parameters

### Black-Scholes Parameters
- **S**: Current stock price
- **K**: Strike price
- **T**: Time to expiration (in years)
- **r**: Risk-free interest rate (annual)
- **sigma**: Volatility (annual)

### Order Parameters
- **isBuy**: Boolean flag (true for buy orders, false for sell orders)
- **price**: Order price
- **quantity**: Number of shares/contracts

## Compilation

```bash
g++ -std=c++11 -o financial_toolkit main.cpp -lm
```

## Requirements

- C++11 or later
- Standard library support for:
  - `<cmath>` (for mathematical functions)
  - `<vector>` (for dynamic arrays)
  - `<algorithm>` (for sorting)
  - `<numeric>` (for accumulate)

## Sample Output

```
Call Option Price: $10.4506
Put Option Price: $5.5735
Trade executed: 75 units at $100

Buy Orders:
ID: 0, Price: $100, Qty: 25
ID: 1, Price: $99, Qty: 50

Sell Orders:
ID: 3, Price: $101, Qty: 25

Back-test profit from call option strategy: $15.2341
```

## Educational Purpose

This toolkit is designed for:
- Learning quantitative finance concepts
- Understanding market microstructure
- Prototyping trading strategies
- Academic research and coursework

## Limitations

- **European Options Only**: No support for American-style options
- **Simplified Exchange Model**: Basic order matching without advanced features
- **Basic Strategy Testing**: Simple backtesting without risk management
- **No Real Market Data**: Requires manual input of historical prices

## Future Enhancements

Potential areas for expansion:
- American option pricing models
- Advanced order types (stop-loss, market orders)
- Risk management tools (VaR, Greeks)
- Real-time market data integration
- More sophisticated backtesting metrics
- Multi-asset portfolio simulation

## Mathematical Foundation

The Black-Scholes formula implemented:

- **Call Option**: C = S₀N(d₁) - Ke^(-rT)N(d₂)
- **Put Option**: P = Ke^(-rT)N(-d₂) - S₀N(-d₁)

Where:
- d₁ = [ln(S₀/K) + (r + σ²/2)T] / (σ√T)
- d₂ = d₁ - σ√T
- N(x) = Cumulative standard normal distribution

## License

This code is provided for educational and research purposes. Please ensure compliance with relevant financial regulations if used in commercial applications.
