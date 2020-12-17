#include "mmc.hpp"

using namespace queue;

mmc_loss::mmc_loss(mmc_loss::time mean_arrival_time,
                   mmc_loss::time mean_departure_time,
                   std::size_t    servers_number,
                   bool verbose)
{
    this->mean_arrival_time   = mean_arrival_time;
    this->mean_departure_time = mean_departure_time;
    this->servers_number      = servers_number;
    this->verbose             = verbose;

    arrival_rate   = 1.0 / mean_arrival_time;
    departure_rate = 1.0 / mean_departure_time;

    servers = std::vector<std::optional<mmc_loss::time>>(servers_number, std::nullopt);
    next_server = servers.begin();
    servers_active = 0;
}

void mmc_loss::simulate(mmc_loss::time time_end)
{
    current_time = 0;

    next_arrival = current_time + exponential_value(arrival_rate);
    next_departure = time_end;

    std::ostringstream debug_stream;
    
    while (current_time < time_end)
    {
        if (this->verbose)
        {
            debug_print(debug_stream);
        }
        
        states_history.push_back(servers_active);

        if(next_arrival < next_departure)
        {
            // arrival happened

            current_time = next_arrival;
            next_arrival = current_time + exponential_value(arrival_rate);

            if(servers_active < servers_number)
            {
                // assigning a server to the customer:

                // find an available server
                auto available_server_it = std::find(servers.begin(), servers.end(), std::nullopt);

                // set serving time to the server
                *available_server_it = current_time + exponential_value(departure_rate);
                servers_active++;

                if(servers_active == 1)
                {
                    // there is only one customer in the system - no one else to serve but him
                    next_server =  available_server_it;
                    next_departure = available_server_it->value();
                }
            }
            else
            {
                // customer denied - no idle server available

                customers_denied++;
            }

        }
        else
        {
            // departure happened
            current_time = next_departure;
            customers_served++;

            // free the server
            *next_server = std::nullopt;
            servers_active--;

            if(servers_active == 0)
            {
                // no one to depart
                next_server = servers.end();
                next_departure = 2 * time_end;
            }
            else
            {
                // find the nearest time from planned departures
                next_server = find_next_server();
                next_departure = next_server->value();
            }
        }
    }

    if (this->verbose)
    {
        std::cerr << debug_stream.str();
    }
    
    simulation_time = time_end;
}

mmc_loss::server_it mmc_loss::find_next_server()
{
    std::sort(servers.begin(), servers.end());
    auto server_iterator = std::upper_bound(servers.begin(), servers.end(), std::nullopt);
    return server_iterator;
}

double mmc_loss::exponential_value(double lambda) const
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::exponential_distribution<> d(lambda);

    return d(gen);
}

void mmc_loss::compute_stationary_probabilities()
{
    stationary_probabilities = std::vector<double>(servers_number + 1, 0);

    // count occurrences of every state
    for(const auto state : states_history)
    {
        stationary_probabilities[state]++;  // state is in [0, servers_number)
    }

    const std::size_t number_of_transitions = states_history.size();

    // compute an average state
    average_state = 0.0;
    for(std::size_t state{0}; state < servers_number + 1; state++)
    {
        // (stationary_probabilities[] is now containing counter for an each state)
        average_state += state * stationary_probabilities[state];
    }
    average_state /= static_cast<double>(number_of_transitions);

    for(auto& value : stationary_probabilities)
    {
        value /= number_of_transitions;
    }
}

void mmc_loss::print_simulation_results(std::ostream& os)
{
    compute_stationary_probabilities();

    os << "'total_time': "  << simulation_time  << "\n";
    os << "'served': "        << customers_served << "\n";
    os << "'denied': "        << customers_denied << "\n";
    os << "'average_state': " << average_state    << "\n";
    os << "stationary probabilities: "          << "\n";

    for(std::size_t state{0}; state < stationary_probabilities.size(); state++)
    {
        if (stationary_probabilities[state] == 0)
        {
            os << "stationary probabilities for states " << state << "-" << stationary_probabilities.size() - 1 << " are 0\n";
            break;
        }
        os << state << ": " << stationary_probabilities[state] << "\n";
    }

    auto sum_of_sp = std::accumulate(stationary_probabilities.cbegin(),
                                     stationary_probabilities.cend(),
                                     0.0);
    os << "sum of stationary probabilities: " << sum_of_sp;
}

void mmc_loss::debug_print(std::ostream& debug_stream) const
{
    int servers_active_modifier;

    if(next_arrival < next_departure)
    {
        debug_stream << next_arrival << ": " << "arrival ";
        if(servers_active < servers_number)
        {
            debug_stream << "accepted";
            servers_active_modifier = 1;
        }
        else
        {
            debug_stream << "denied";
            servers_active_modifier = 0;
        }
    }
    else
    {
        debug_stream << next_departure << ": " << "departure";

        servers_active_modifier = -1;
    }

    debug_stream << ", servers active - ";
    debug_stream << servers_active + servers_active_modifier << "/" << servers_number << "\n";
}
