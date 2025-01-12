#include "aircraft.hpp"

#include "GL/opengl_interface.hpp"

#include <cmath>
#include <iostream>

void Aircraft::turn_to_waypoint()
{
    if (!waypoints.empty())
    {
        Point3D target = waypoints[0];
        if (waypoints.size() > 1)
        {
            const float d   = (waypoints[0] - pos).length();
            const Point3D W = (waypoints[0] - waypoints[1]).normalize(d / 2.0f);
            target += W;
        }

        turn(target - pos - speed);
    }
}

void Aircraft::turn(Point3D direction)
{
    // assert(direction != nullptr);
    (speed += direction.cap_length(type.max_accel)).cap_length(max_speed());
}

unsigned int Aircraft::get_speed_octant() const
{
    const float speed_len = speed.length();
    if (speed_len > 0)
    {
        const Point3D norm_speed { speed * (1.0f / speed_len) };
        const float angle =
            (norm_speed.y() > 0) ? 2.0f * 3.141592f - std::acos(norm_speed.x()) : std::acos(norm_speed.x());
        // partition into NUM_AIRCRAFT_TILES equal pieces

        return (static_cast<int>(std::round((angle * NUM_AIRCRAFT_TILES) / (2.0f * 3.141592f))) + 1) %
               NUM_AIRCRAFT_TILES;
    }
    else
    {

        return 0;
    }
}

// when we arrive at a terminal, signal the tower
void Aircraft::arrive_at_terminal()
{
    // we arrived at a terminal, so start servicing
    control.arrived_at_terminal(*this);
    is_at_terminal = true;
}

// deploy and retract landing gear depending on next waypoints
void Aircraft::operate_landing_gear()
{
    if (waypoints.size() > 1u)
    {
        const auto it            = waypoints.begin();
        const bool ground_before = it->is_on_ground();
        const bool ground_after  = std::next(it)->is_on_ground();
        // deploy/retract landing gear when landing/lifting-off
        if (ground_before && !ground_after)
        {
            std::cout << flight_number << " lift off" << std::endl;
        }
        else if (!ground_before && ground_after)
        {
            std::cout << flight_number << " is now landing..." << std::endl;
            landing_gear_deployed = true;
        }
        else if (!ground_before && !ground_after)
        {
            landing_gear_deployed = false;
        }
    }
}

template <bool front> void Aircraft::add_waypoint(const Waypoint& wp)
{
    if constexpr (front == true)
    {
        waypoints.push_front(wp);
    }
    else
    {
        waypoints.push_back(wp);
    }
}

bool Aircraft::move()
{
    if (waypoints.empty())
    {
        if (!incoming)
        {
            return false;
        }
        // If i ask for instruction after leaving the terinal, i'm sufficiently hight to be removed!
        // waypoints = control.get_instructions(*this);
        const auto front = false;
        for (const auto& wp : control.get_instructions(*this))
        {
            add_waypoint<front>(wp);
        }
    }

    if (!is_at_terminal)
    {
        turn_to_waypoint();
        // move in the direction of the current speed
        pos += speed;

        if (fuel <= 0)
        {
            std::cout << flight_number << " has no more fuel" << std::endl;
            // return false;
            throw AircraftCrash { "aeiognpazoebazpobaeobiaeoibapoibpaoirbapoibpaobipaoiebhpaoiAegf eazfaze" };
        }
        if (is_circling())
        {
            // std::cout << "adding waypoints" << std::endl;
            auto tmp_waypoints = control.reserve_terminal(*this);
            if (!tmp_waypoints.empty())
            {
                std::cout << flight_number << "'s path update: " << std::endl;
                // std::move(tmp_waypoints.begin(), tmp_waypoints.end(), std::back_inserter(waypoints));
                waypoints = std::move(tmp_waypoints); // <- not only for U_P.
            }
        }

        // if we are close to our next waypoint, stike if off the list
        if (!waypoints.empty() && distance_to(waypoints.front()) < DISTANCE_THRESHOLD)
        {
            if (waypoints.front().is_at_terminal())
            {
                arrive_at_terminal();
            }
            else
            {
                operate_landing_gear();
            }
            waypoints.pop_front();
        }

        if (is_on_ground())
        {
            if (!landing_gear_deployed)
            {
                using namespace std::string_literals;
                throw AircraftCrash { flight_number + " crashed into the ground"s };
            }
        }
        else
        {
            // if we are in the air, but too slow, then we will sink!
            const float speed_len = speed.length();

            fuel--;
            if (is_low_on_fuel())
            {
                // speed.cap_length(std::max(speed_len - 0.1f, 0.f));
                speed.cap_length(0.f);
            }
            if (speed_len < SPEED_THRESHOLD)
            {
                pos.z() -= SINK_FACTOR * (SPEED_THRESHOLD - speed_len);
            }
        }

        // update the z-value of the displayable structure
        GL::Displayable::z = pos.x() + pos.y();
    }
    return true;
}

void Aircraft::display() const
{
    type.texture.draw(project_2D(pos), { PLANE_TEXTURE_DIM, PLANE_TEXTURE_DIM }, get_speed_octant());
}

bool Aircraft::has_terminal() const
{
    if (waypoints.empty())
    {
        return false;
    }
    return waypoints.back().type == WaypointType::wp_terminal;
}

bool Aircraft::is_circling() const
{
    return (waypoints.back().type == WaypointType::wp_air) && incoming;
}

bool Aircraft::is_low_on_fuel() const
{
    return low_fuel_threshold > fuel;
}

void Aircraft::refill(int& fuel_stock)
{
    assert(fuel_stock >= 0 && "fuel_stock should never be ABLE to drop beloow 0");
    int fuel_needed = 3000 - fuel;
    int fuel_used   = std::min(fuel_stock, fuel_needed);
    std::cout << flight_number << " " << fuel_used << "fuel from stock (fuel aircraft : " << fuel
              << "; stock : " << fuel_stock << "; fuel needed :" << fuel_needed << std::endl;
    fuel += fuel_used;
    fuel_stock -= fuel_used;
}

bool Aircraft::at_terminal()
{
    return is_at_terminal;
}

int Aircraft::get_fuel() const
{
    return fuel;
}

const Point3D Aircraft::get_position()
{
    return pos;
}