#include "aircraft_manager.hpp"

#include <algorithm>
#include <string>
#include <utility>

AircraftManager::AircraftManager()
{
    GL::move_queue.insert(this);
}

bool AircraftManager::move()
{

    // for (auto it = aircrafts.begin(); it != aircrafts.end();)
    // {
    //     if (!it->second->move())
    //     {
    //         it = aircrafts.erase(it);
    //     }
    //     else
    //     {
    //         it++;
    //     }
    // }
    std::cout << "AircraftManager::move() IN" << std::endl;
    auto it_end = std::remove_if(aircrafts.begin(), aircrafts.end(),
                                 [](auto& aircraft)
                                 {
                                     std::cout << "predicate in remove_if from AircraftManager::move"
                                               << std::endl;
                                     return !aircraft->move();
                                 });
    aircrafts.erase(it_end, aircrafts.cend());
    std::cout << "AircraftManager::move() OUT" << std::endl;
    return true;
}

void AircraftManager::add_aircraft(std::unique_ptr<Aircraft> aircraft)
{
    aircrafts.emplace_back(std::move(aircraft));
}

void AircraftManager::create_random_aircraft(Tower& tower)
{
    auto aircraft = factory.create_random_aircraft(tower);
    aircrafts.emplace_back(std::move(aircraft));
}

void AircraftManager::create_aircraft(Tower& tower, const AircraftType& type)
{
    auto aircraft = factory.create_aircraft(tower, type);
    aircrafts.emplace_back(std::move(aircraft));
}

int AircraftManager::count_airline_members(int airline)
{
    return std::count_if(
        aircrafts.begin(), aircrafts.end(),
        [this, airline](std::unique_ptr<Aircraft>& aircraft)
        {
            std::cout << "predicate in count_if from AAircraftManager::count_airline_members" << std::endl;
            return aircraft->get_flight_num().substr(0, 2).compare(factory.get_airline_name(airline)) == 0;
        });
}