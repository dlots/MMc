#include <iostream>
#include <sstream>

#include "mmc.hpp"

int main(int argc, char* argv[])
{
    double mean_arrival_time;
    double mean_departure_time;
    int    number_of_servers;
    double simulation_time;

    if(argc == 5)
    {
        std::stringstream ss;
        ss << argv[1] << " " << argv[2] << " "
           << argv[3] << " " << argv[4];
        std::cerr << ss.str();

        ss >> mean_arrival_time;
        ss >> mean_departure_time;
        ss >> number_of_servers;
        ss >> simulation_time;
    }
    else
    {
        std::cerr << "wrong parameters" << std::endl;
        exit(1);
    }

    queue::mmc_loss queue_model(mean_arrival_time, mean_departure_time, number_of_servers);

    queue_model.simulate(simulation_time);
    
    std::ostringstream results;
    queue_model.print_simulation_results(results);
    std::cout << results.str();

    return 0;
}
