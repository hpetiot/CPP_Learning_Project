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
    std::cout << "AircraftManager::move() IN" << std::endl;
    // sorting the aircrafts:
    std::sort(aircrafts.begin(), aircrafts.end(),
              [](auto& aircraftLesser, auto& aircraftBigger)
              {
                  std::cout << "predicate sort aircraft IN" << std::endl;
                  if (aircraftLesser->has_terminal())
                  {
                      std::cout << "pased 1st if" << std::endl;
                      if (aircraftBigger->has_terminal())
                      {
                          std::cout << "predicate sort aircraft OUT" << std::endl;

                          return aircraftLesser->get_fuel() < aircraftBigger->get_fuel();
                      }
                      std::cout << "predicate sort aircraft OUT" << std::endl;

                      return true;
                  }
                  if (aircraftBigger->has_terminal())
                  {
                      std::cout << "predicate sort aircraft OUT" << std::endl;

                      return false;
                  }
                  std::cout << "predicate sort aircraft OUT" << std::endl;

                  return aircraftLesser->get_fuel() < aircraftBigger->get_fuel();
              });
    std::cout << "done sorting list of aicrafts" << std::endl;
    // movin gthe aircrafts:
    auto it_end =
        std::remove_if(aircrafts.begin(), aircrafts.end(), [](auto& aircraft) { return !aircraft->move(); });
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
        { return aircraft->get_flight_num().substr(0, 2).compare(factory.get_airline_name(airline)) == 0; });
}

int AircraftManager::get_required_fuel() const
{
    std::cout << "AircraftManager::get_required_fuel()" << std::endl;
    return std::accumulate(aircrafts.begin(), aircrafts.end(), 0,
                           [](int acc, auto& aircraft)
                           {
                               std::cout << "acc passage" << std::endl;
                               if (aircraft->is_low_on_fuel() && aircraft->at_terminal())
                               {
                                   return acc + (3000 - aircraft->get_fuel());
                               }
                               return acc;
                           });
}