#pragma once

#include <cstddef>
#include <iostream>
#include <vector>
#include <optional>
#include <algorithm>
#include <random>
#include <sstream>
#include <limits>
#include <chrono>

namespace queue
{
    using time_t = double;
    using rate_t = double;

    class server
    {
    public:
        server();

        void set_time(time_t next_departure_time);

        void make_empty() noexcept;

        [[nodiscard]] bool is_empty() const noexcept;

        [[nodiscard]] time_t get_time() const;

    private:
        std::optional<time_t> next_departure_time_;

        friend inline bool operator < (const server& lhs, const server& rhs)
        {
            return lhs.next_departure_time_ < rhs.next_departure_time_;
        }

        friend inline bool operator == (const server& lhs, const server& rhs)
        {
            return lhs.next_departure_time_ == rhs.next_departure_time_;
        }
    };

    class mmc_loss
    {
        using server_it = std::vector<server>::iterator;

    public:
        mmc_loss(time_t mean_arrival_time, time_t mean_departure_time,
                 std::size_t servers_number, bool verbose);

        void simulate(time_t time_end);

        void print_simulation_results(std::ostream& os = std::cout);

    private:

        [[nodiscard]] server_it find_next_server();

        [[nodiscard]] double lambda_exponential_value();
        [[nodiscard]] double mu_exponential_value();

        void compute_stationary_probabilities();

        void debug_print(std::ostream& debug_stream = std::cerr) const;

        // input parameters:

        time_t mean_arrival_time;    // mean time for the next arrival;
        time_t mean_departure_time;  // mean time for the next departure;
        std::size_t servers_number;  // total number of services in system
        bool verbose;                // whether to print the debug output

        rate_t arrival_rate;    // departure rate in customers per second
        rate_t departure_rate;  // arrival rate in customers per second

        // current parameters in simulation:

        std::exponential_distribution<> lambda;
        std::exponential_distribution<> mu;
        std::mt19937 gen_lambda;
        std::mt19937 gen_mu;

        std::size_t servers_active = 0;  // total number of active services

        std::vector<server> servers;
        server_it next_server;  // next server to depart a customer

        time_t current_time = 0;    // current time in simulation
        time_t next_arrival = 0;    // time for the next arrival
        time_t next_departure = 0;  // time for the next departure

        // simulation results:

        std::size_t customers_served = 0;  // total number of served customers
        std::size_t customers_denied = 0;  // total number of denied customers

        time_t simulation_time = 0;

        std::vector<std::size_t> states_history;
        std::vector<double> stationary_probabilities;
        double average_state = 0;
    };
}
