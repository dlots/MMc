#include <iostream>
#include <sstream>

#include "mmc.hpp"

const std::string verbose_parameter("--verbose");

int main(int argc, char* argv[])
{
    double mean_arrival_time;
    double mean_departure_time;
    int    number_of_servers;
    double simulation_time;
    bool verbose = false;

    if(argc >= 5)
    {
        std::stringstream ss;
        ss << argv[1] << " " << argv[2] << " "
           << argv[3] << " " << argv[4] << "\n";
        std::cerr << ss.str();

        ss >> mean_arrival_time;
        ss >> mean_departure_time;
        ss >> number_of_servers;
        ss >> simulation_time;
        
        if ((argc == 6) && (verbose_parameter == argv[5]))
        {
            verbose = true;
        }
    }
    else
    {
        std::cerr << "wrong parameters" << "\n";
        exit(1);
    }

    queue::mmc_loss queue_model(mean_arrival_time, mean_departure_time, number_of_servers, verbose);

    queue_model.simulate(simulation_time);
    
    std::ostringstream results;
    results.precision(3);
    queue_model.print_simulation_results(results);
    std::cout << results.str();

    return 0;
}
