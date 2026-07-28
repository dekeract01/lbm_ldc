// grid.hpp :: field storage and initialization
#pragma once

#include <vector>
#include "constants.hpp"
#include "lattice.hpp"

namespace lbm {

    // all field data for the simulation
    struct grid {
        std::vector<double> f;      // populations      (q * ncells)
        std::vector<double> fnew;   // streaming buffer (q * ncells)
        std::vector<double> rho;    // density          (ncells)
        std::vector<double> ux;     // x-velocity       (ncells)
        std::vector<double> uy;     // y-velocity       (ncells)

        grid() : f(q*ncells, 0.0), fnew(q*ncells, 0.0), rho(ncells, 1.0),
                ux(ncells, 0.0), uy(ncells, 0.0) {}
    };

    // set populations to equilibrium at rest (rho = 1, u = 0);
    // feq at zero velocity reduces to w[i] * rho
    inline void init_rest(grid& g) {
        for (int y = 0; y < ny; ++y)
            for (int x = 0; x < nx; ++x) {
                const double r = g.rho[cidx(x, y)];
                for (int i = 0; i < q; ++i) g.f[idx(i, x, y)] = w[i]*r;
            }
    }

} // namespace lbm