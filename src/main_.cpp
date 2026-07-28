// main.cpp :: D2Q9 lid-driven cavity solver

#include <iostream>
#include <iomanip>
#include <chrono>

#include "constants.hpp"
#include "lattice.hpp"
#include "grid.hpp"
#include "macroscopic.hpp"
#include "equilibrium.hpp"
#include "collision.hpp"
#include "streaming.hpp"
#include "boundary.hpp"
#include "io.hpp"

int main()
{
    std::cout << "D2Q9 lid-driven cavity\n";
    std::cout << "grid " << lbm::nx << " x " << lbm::ny
              << "   Re = " << lbm::Re
              << "   tau = " << lbm::tau << "\n\n";

    lbm::grid g;
    lbm::init_rest(g);

    using clock = std::chrono::steady_clock;
    auto t_start = clock::now();
    auto t_prev  = t_start;

    for (int step = 0; step <= lbm::max_steps; ++step) {
        lbm::compute_moments(g);   // rho, u from f
        lbm::collide(g);           // relax f toward equilibrium
        lbm::stream(g);            // propagate into fnew, swap
        lbm::apply_boundaries(g);  // fill wall holes: bounce-back + lid

        if (step % lbm::output_freq == 0) {
            lbm::write_h5(g, step);

            auto now = clock::now();
            // time for this block of steps, and MLUPS (million lattice
            // updates per second) — the standard LBM throughput metric
            double dt = std::chrono::duration<double>(now - t_prev).count();
            double mlups = (lbm::output_freq * double(lbm::ncells))/(dt * 1e6);
            double elapsed = std::chrono::duration<double>(now - t_start).count();

            std::cout << "step " << std::setw(6) << step
                      << "   " << std::fixed << std::setprecision(1)
                      << std::setw(6) << mlups << " MLUPS"
                      << "   elapsed " << std::setprecision(1) << elapsed << " s\n";

            t_prev = now;
        }
    }

    std::cout << "done\n";
    return 0;
}