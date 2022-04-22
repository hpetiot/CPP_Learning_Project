#pragma once

#include "aircraft.hpp"
#include "aircraft_types.hpp"

#include <string>
class AircraftFactory
{
private:
    static constexpr size_t NUM_AIRCRAFT_TYPES = 3;
    // std::normal_distribution<double> random_fuel { 150, 3000 };
    const int MIN_FUEL = 150;
    const int MAX_FUEL = 500; // normaly 3K, cahnged for test during soutenance ex

    const AircraftType* aircraft_types[NUM_AIRCRAFT_TYPES];
    const std::string airlines[8] = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };
    std::unordered_set<std::string> list_flight_number;

public:
    AircraftFactory(); // with init_aircraft_typesinside;
    const std::string_view get_airline_name(int airline_number);
    std::unique_ptr<Aircraft> create_aircraft(Tower& tower, const AircraftType& type);
    std::unique_ptr<Aircraft> create_random_aircraft(Tower& tower);
};