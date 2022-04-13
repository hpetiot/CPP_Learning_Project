#include "aircraft_manager.hpp"

#include <algorithm>
#include <numeric>
#include <string>
#include <utility>

AircraftManager::AircraftManager()
{
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
                      if (aircraftBigger->has_teminal())
                      {
                          return aircraftLesser.get_fuel() < aircraftBigger.get_fuel();
                      }
                      return true;
                  }
                  if (aircraftBigger->has_teminal())
                  {
                      return false;
                  }
                  return aircraftLesser.get_fuel() < aircraftBigger.get_fuel();
              });
    // movin gthe aircrafts:
    auto it_end =
        std::remove_if(aircrafts.begin(), aircrafts.end(), [](auto& aircraft) { return !aircraft->move(); });
    aircrafts.erase(it_end, aircrafts.cend());
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
        { return aircraft->get_flight_num().substr(0, 2).compare(factory.get_airline_name(airline)) == 0; });
}

int AircraftManager::get_required_fuel() const
{
    return std::accumulate(aircrafts.begin(), aircrafts.end(), 0,
                           [](int acc, auto& aircraft)
                           {
                               if (aircraft.is_low_on_fuel() && aircraft.incoming())
                               {
                                   return 3000 - aircraft.get_fuel();
                               }
                               return 0;
                           });
}