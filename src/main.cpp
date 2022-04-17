#include "tower_sim.hpp"

template <typename... T> void printall(const T&... params)
{
    ((std::cout << params << std::endl), ...);
}

void test_generic_points()
{
    Point<float, 3> p1 { 0.3f, 0.3f, 0.3f };
    Point<float, 3> p2 { 0.3, 0.3f, 0.3f };
    // Point<std::string, 2> p6 { "je me suis", "planté" };

    auto p3 = p1 + p2;
    auto p4 = p1 * 2;
    auto p5 = p1 - p2;
    auto p6 = p1;

    printall(p1, p2, p3, p4, p5, p6);
}

int main(int argc, char** argv)
{
    test_generic_points();
    TowerSimulation simulation { argc, argv };
    simulation.launch();

    return 0;
}