// macroscopic.hpp :: compute rho, ux, uy from populations
#pragma once

#include "constants.hpp"
#include "lattice.hpp"
#include "grid.hpp"

namespace lbm {

inline void compute_moments(grid& g) {

    for (int y = 0; y < ny; ++y)
        for (int x = 0; x < nx; ++x) {

            double r = 0.0, mx = 0.0, my = 0.0;
            for (int i = 0; i < q; ++i) {
                const double fi = g.f[idx(i, x, y)];
                r  += fi;
                mx += fi*cx[i];
                my += fi*cy[i];
            }
            const int c = cidx(x, y);
            g.rho[c] = r;
            g.ux[c]  = mx/r;
            g.uy[c]  = my/r;
        }
}

} // namespace lbm