#pragma once

#include "GL/dynamic_object.hpp"
#include "aircraft.hpp"

#include <memory>
#include <string_view>
#include <unordered_map>
class AircraftManager : public GL::DynamicObject
{
private:
    std::unordered_map<std::string_view, std::unique_ptr<Aircraft>> aircrafts;
    void add_aricraft(std::unique_ptr<Aircraft> aircraft);

public:
    AircraftManager() {};
    bool move() override;
    void add_aircraft(std::unique_ptr<Aircraft>);
};