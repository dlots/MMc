#pragma once

#include <cstddef>
#include <iostream>
#include <vector>
#include <optional>
#include <algorithm>
#include <random>
#include <sstream>

namespace queue
{
    class mmc_loss
    {
        using time = double;
        using rate = double;
        using server_it = std::vector<std::optional<double>>::iterator;

    public:
        mmc_loss(time mean_arrival_time, time mean_departure_time, std::size_t servers_number, bool verbose);

        void simulate(time time_end);

        void print_simulation_results(std::ostream& os = std::cout);

    private:

        [[nodiscard]] server_it find_next_server();

        [[nodiscard]] double exponential_value(double lambda) const;

        void compute_stationary_probabilities();

        void debug_print(std::ostream& debug_stream = std::cerr) const;

        // input parameters:

        time mean_arrival_time;       // mean time for the next arrival;
        time mean_departure_time;     // mean time for the next departure;
        std::size_t servers_number;  // total number of services in system
        bool verbose;                // whether to print the debug output

        rate arrival_rate;    // departure rate in customers per second
        rate departure_rate;  // arrival rate in customers per second

        // current parameters in simulation:

        std::size_t servers_active = 0;  // total number of active services

        std::vector<std::optional<time>> servers;
        server_it next_server;  // next server to depart a customer

        time current_time = 0;    // current time in simulation
        time next_arrival = 0;    // time for the next arrival
        time next_departure = 0;  // time for the next departure

        // simulation results:

        std::size_t customers_served = 0;  // total number of served customers
        std::size_t customers_denied = 0;  // total number of denied customers

        time simulation_time = 0;

        std::vector<std::size_t> states_history;
        std::vector<double> stationary_probabilities;
        double average_state = 0;
    };
}
