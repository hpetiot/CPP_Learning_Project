#include "aircraft_manager.hpp"

#include <algorithm>
#include <cassert>
#include <numeric>
#include <string>
#include <utility>

AircraftManager::AircraftManager()
{
    crashes_count = 0;
    GL::move_queue.insert(this);
}

bool AircraftManager::move()
{
    // sorting the aircrafts:
    std::sort(aircrafts.begin(), aircrafts.end(),
              [](auto& aircraftLesser, auto& aircraftBigger)
              {
                  if (aircraftLesser->has_terminal())
                  {
                      if (aircraftBigger->has_terminal())
                      {

                          return aircraftLesser->get_fuel() < aircraftBigger->get_fuel();
                      }

                      return true;
                  }
                  if (aircraftBigger->has_terminal())
                  {

                      return false;
                  }

                  return aircraftLesser->get_fuel() < aircraftBigger->get_fuel();
              });
    // movin gthe aircrafts:
    auto it_end = std::remove_if(aircrafts.begin(), aircrafts.end(),
                                 [this](const auto& aircraft)
                                 {
                                     try
                                     {
                                         return !aircraft->move();
                                     } catch (const AircraftCrash& ac)
                                     {
                                         crashes_count++;
                                         std::cerr << ac.what() << ", total crashes : " << crashes_count
                                                   << std::endl;
                                         return true;
                                     }
                                 });
    aircrafts.erase(it_end, aircrafts.end());
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
    assert(airline >= 0 && "the index of a talbe cannot be negative");

    return std::count_if(
        aircrafts.begin(), aircrafts.end(),
        [this, airline](std::unique_ptr<Aircraft>& aircraft)
        { return aircraft->get_flight_num().substr(0, 2).compare(factory.get_airline_name(airline)) == 0; });
}

int AircraftManager::get_required_fuel() const
{
    // std::cout << "AircraftManager::get_required_fuel()" << std::endl;
    return std::accumulate(aircrafts.begin(), aircrafts.end(), 0,
                           [](int acc, const auto& aircraft)
                           {
                               if (aircraft->is_low_on_fuel() && aircraft->is_incoming())
                               {
                                   int added_fuel = 3000 - aircraft->get_fuel();
                                   std::cout << "aircraft was ow on fuel : needs " << added_fuel << std::endl;
                                   return acc + added_fuel;
                               }
                               return acc;
                           });
}

void AircraftManager::display_crashes()
{
    std::cout << "total crashes : " << crashes_count << std::endl;
}