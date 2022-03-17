#pragma once

#include "GL/opengl_interface.hpp"
#include "aircraft_manager.hpp"
#include "img/media_path.hpp"

class Airport;
struct AircraftType;

struct ContextInitializer
{
    ContextInitializer(int argc, char** argv)
    {
        MediaPath::initialize(argv[0]);
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        GL::init_gl(argc, argv, "Airport Tower Simulation");
    }
};

class TowerSimulation
{
private:
    bool help = false;
    ContextInitializer context;
    Airport* airport = nullptr;
    AircraftManager aircraft_manager {};

    TowerSimulation(const TowerSimulation&) = delete;
    TowerSimulation& operator=(const TowerSimulation&) = delete;

    void create_aircraft(const AircraftType& type);
    // void create_aircraft(const AircraftType& type);

    void create_random_aircraft();
    // void create_random_aircraft();

    void create_keystrokes();
    void display_help() const;

    void init_airport();

public:
    TowerSimulation(int argc, char** argv);
    ~TowerSimulation();

    void launch();
};
