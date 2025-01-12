#include "tower_sim.hpp"

#include "GL/opengl_interface.hpp"
#include "aircraft.hpp"
#include "airport.hpp"
#include "config.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>

using namespace std::string_literals;

// const std::string airlines[8] = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };

TowerSimulation::TowerSimulation(int argc, char** argv) :
    help { (argc > 1) && (std::string { argv[1] } == "--help"s || std::string { argv[1] } == "-h"s) },
    context { argc, argv }
{
    // MediaPath::initialize(argv[0]);
    // std::srand(static_cast<unsigned int>(std::time(nullptr)));
    // GL::init_gl(argc, argv, "Airport Tower Simulation");

    create_keystrokes();
}

TowerSimulation::~TowerSimulation()
{
    delete airport;
}

// void TowerSimulation::create_aircraft(const AircraftType& type)
// // void TowerSimulation::create_aircraft(const AircraftType& type)
// {
//     assert(airport); // make sure the airport is initialized before creating aircraft

//     // const std::string flight_number = airlines[std::rand() % 8] + std::to_string(1000 + (rand() %
//     9000));
//     // const float angle       = (rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and
//     2pi
//     // const Point3D start     = Point3D { std::sin(angle), std::cos(angle), 0 } * 3 + Point3D { 0, 0, 2 };
//     // const Point3D direction = (-start).normalize();

//     aircraft_manager.add_aircraft(
//         std::make_unique<Aircraft>(type, flight_number, start, direction, airport->get_tower()));
//     // Aircraft* aircraft = new Aircraft { type, flight_number, start, direction, airport->get_tower() };
//     //  GL::display_queue.emplace_back(aircraft);
//     //  GL::move_queue.emplace(aircraft);
// }

// void TowerSimulation::create_random_aircraft()
// {

//     create_aircraft(*(aircraft_types[rand() % 3]));
// }

void TowerSimulation::create_keystrokes()
{
    GL::keystrokes.emplace('0',
                           [this]() { std::cout << aircraft_manager.count_airline_members(0) << std::endl; });
    GL::keystrokes.emplace('1',
                           [this]() { std::cout << aircraft_manager.count_airline_members(1) << std::endl; });
    GL::keystrokes.emplace('2',
                           [this]() { std::cout << aircraft_manager.count_airline_members(2) << std::endl; });
    GL::keystrokes.emplace('3',
                           [this]() { std::cout << aircraft_manager.count_airline_members(3) << std::endl; });
    GL::keystrokes.emplace('4',
                           [this]() { std::cout << aircraft_manager.count_airline_members(4) << std::endl; });
    GL::keystrokes.emplace('5',
                           [this]() { std::cout << aircraft_manager.count_airline_members(5) << std::endl; });
    GL::keystrokes.emplace('6',
                           [this]() { std::cout << aircraft_manager.count_airline_members(6) << std::endl; });
    GL::keystrokes.emplace('7',
                           [this]() { std::cout << aircraft_manager.count_airline_members(7) << std::endl; });

    GL::keystrokes.emplace('x', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('q', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('c', [this]() { aircraft_manager.create_random_aircraft(airport->get_tower()); });
    GL::keystrokes.emplace('m', [this]() { aircraft_manager.display_crashes(); });
    GL::keystrokes.emplace('+', []() { GL::change_zoom(0.95f); });
    GL::keystrokes.emplace('-', []() { GL::change_zoom(1.05f); });
    GL::keystrokes.emplace('f', []() { GL::toggle_fullscreen(); });
    GL::keystrokes.emplace('*', []() { GL::ticks_per_sec += DEFAULT_TPS_INCREMENT; }); // @MINE
    GL::keystrokes.emplace('/', []() { GL::ticks_per_sec -= DEFAULT_TPS_INCREMENT; }); // @MINE
    GL::keystrokes.emplace('p', []() { GL::PAUSED = !GL::PAUSED; });                   // @MINE
    // GL::keystrokes.emplace(' ', [](){GL::ticks_per_sec}); // @MINE
}

void TowerSimulation::display_help() const
{
    std::cout << "This is an airport tower simulator" << std::endl
              << "the following keysstrokes have meaning:" << std::endl;

    for (const auto& [characterKey, keyStroke] : GL::keystrokes)
    {
        std::cout << characterKey << ' ';
    }

    std::cout << std::endl;
}

void TowerSimulation::init_airport()
{
    airport =
        new Airport { one_lane_airport, Point3D { 0, 0, 0 },
                      new img::Image { one_lane_airport_sprite_path.get_full_path() }, aircraft_manager };

    // GL::display_queue.emplace_back(airport);
    GL::move_queue.emplace(airport);
}

void TowerSimulation::launch()
{
    if (help)
    {
        display_help();
        return;
    }

    init_airport();
    // init_aircraft_types();

    GL::loop();
}
