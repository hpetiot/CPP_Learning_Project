#include "AircraftManager.hpp"

#include <utility>

bool AircraftManager::move()
{
    for (auto it = aircrafts.begin(); it != aircrafts.end();)
    {
        if (!it->second->move())
        {
            it = aircrafts.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void AircraftManager::add_aircraft(std::unique_ptr<Aircraft> aircraft)
{
    aircrafts[aircraft->get_flight_num()] = std::move(aircraft);
}