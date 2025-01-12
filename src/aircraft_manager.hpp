#pragma once

#include "AircraftFactory.hpp"
#include "GL/dynamic_object.hpp"
#include "aircraft.hpp"

#include <memory>
#include <string_view>
#include <unordered_map>
class AircraftManager : public GL::DynamicObject
{
private:
    // std::unordered_map<std::string_view, std::unique_ptr<Aircraft>> aircrafts;
    std::vector<std::unique_ptr<Aircraft>> aircrafts;
    // void add_aricraft(std::unique_ptr<Aircraft> aircraft);
    AircraftFactory factory {};
    int crashes_count;

public:
    AircraftManager();
    bool move() override;
    void add_aircraft(std::unique_ptr<Aircraft>);
    void create_random_aircraft(Tower& tower);
    int count_airline_members(int airline);
    void display_crashes();
    void create_aircraft(Tower& tower, const AircraftType& type);
    int get_required_fuel() const;
};