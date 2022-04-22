#pragma once

#include "GL/displayable.hpp"
#include "GL/texture.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"
#include "geometry.hpp"

#include <memory>
#include <vector>

class CrashedAircraft: public GL::Displayable
{
private:
    static inline std::vector<std::unique_ptr<CrashedAircraft>> instances;

    const GL::Texture2D texture = GL::Texture2D { new img::Image { MediaPath { "crashed_aircraft.png" }.get_full_path() }};
    const Point2D position;

public:
    static void add(Point3D aircraft_pos)
    {
        instances.emplace_back(std::make_unique<CrashedAircraft>(aircraft_pos));
    }

    explicit CrashedAircraft(Point3D aircraft_pos)
    : GL::Displayable { aircraft_pos.x() + aircraft_pos.y() }
    , position { project_2D(aircraft_pos) }
    {}

    void display() const override
    {
        texture.draw(position, { PLANE_TEXTURE_DIM, PLANE_TEXTURE_DIM });
    }
};
