#pragma once

#include "waypoint.hpp"

#include <algorithm>
#include <map>
#include <unordered_map>
#include <utility>

class Airport;
class Aircraft;
class Terminal;

class Tower
{
private:
    using AircraftToTerminal = std::map<const Aircraft*, size_t>; // -> better to use hashMap (is an explicite
                                                                  // type "Unorderred map").
    using AircraftAndTerminalIter = AircraftToTerminal::iterator;

    Airport& airport;
    // aircrafts may reserve a terminal
    // if so, we need to save the terminal number in order to liberate it when the craft leaves
    AircraftToTerminal reserved_terminals = {};

    WaypointQueue get_circle() const;

    // AircraftAndTerminalIter find_craft_and_terminal(const Aircraft& aircraft)
    // {
    //     return std::find_if(reserved_terminals.begin(), reserved_terminals.end(),
    //                         [&aircraft](const auto& x) { return x.first == &aircraft; });
    // }

public:
    Tower(Airport& airport_) : airport { airport_ } {}

    // produce instructions for aircraft
    WaypointQueue get_instructions(Aircraft& aircraft);
    WaypointQueue reserve_terminal(Aircraft& aircraft);
    void arrived_at_terminal(const Aircraft& aircraft);
    void signal_crash(const Aircraft* aircraft);
};
