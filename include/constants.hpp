// constants.hpp :: compile-time simulation parameters
#pragma once

namespace lbm {

    // simulation control
    constexpr int max_steps   = 20000;
    constexpr int output_freq = 1000;

    // grid dimensions (lattice units: dx = dt = 1)
    constexpr int nx = 128;
    constexpr int ny = 128;
    constexpr int ncells = nx*ny;

    // wall boundary scheme ---- this needs streamlining 
    constexpr bool use_halfway = false;

    // cavity length
    constexpr double L = use_halfway ? static_cast<double>(nx) : (nx - 1.0);

    // lid-driven cavity flow parameters
    constexpr double Re    = 1000.0;
    constexpr double ulid  = 0.1;
    constexpr double nu    = ulid * L/Re;
    constexpr double tau   = 3.0*nu + 0.5;
    constexpr double omega = 1.0/tau;

}