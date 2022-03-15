#include "aircraft_manager.hpp"

#include <utility>

AircraftManager::AircraftManager()
{
    GL::move_queue.insert(this);
}

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

    return true;
}

void AircraftManager::add_aircraft(std::unique_ptr<Aircraft> aircraft)
{
    aircrafts[aircraft->get_flight_num()] = std::move(aircraft);
}