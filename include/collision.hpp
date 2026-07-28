// collision.hpp :: BGK single-relaxation-time collision
#pragma once

#include "constants.hpp"
#include "lattice.hpp"
#include "grid.hpp"
#include "equilibrium.hpp"

namespace lbm {

// BGK collision: relax populations toward local equilibrium.
// assumes g.rho, g.ux, g.uy are current (call compute_moments first).
inline void collide(grid& g) {
    for (int y = 0; y < ny; ++y)
        for (int x = 0; x < nx; ++x) {
            const int c = cidx(x, y);
            const double r  = g.rho[c];
            const double vx = g.ux[c];
            const double vy = g.uy[c];
            for (int i = 0; i < q; ++i) {
                const int k = idx(i, x, y);
                const double feq = compute_feq(i, r, vx, vy);
                g.f[k] -= omega*(g.f[k] - feq);
            }
        }
}

} // namespace lbm