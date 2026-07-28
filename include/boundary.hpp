// boundary.hpp :: halfway bounce-back — static walls + moving lid
#pragma once

#include "constants.hpp"
#include "lattice.hpp"
#include "grid.hpp"

namespace lbm {

// Runs AFTER streaming. Pull-streaming leaves the wall-incoming populations
// (whose source neighbor is outside the domain) unset. Those holes are filled
// here by reflecting the opposite population back into the domain.
//
// Unknown ("hole") directions per wall, from the velocity set:
//   bottom y=0     : cy>0  -> 2(N), 5(NE), 6(NW)
//   top    y=ny-1  : cy<0  -> 4(S), 7(SW), 8(SE)   [moving lid]
//   left   x=0     : cx>0  -> 1(E), 5(NE), 8(SE)
//   right  x=nx-1  : cx<0  -> 3(W), 6(NW), 7(SW)
//
// static wall, one direction: f_i = f_opp
// moving lid, one direction : f_i = f_opp - 2 w_i rho (c_i . u_wall)/cs2
//   with u_wall = (ulid, 0)

inline void reflect_static(grid& g, int x, int y, int i) {
    g.f[idx(i, x, y)] = g.f[idx(opp[i], x, y)];
}

inline void reflect_lid(grid& g, int x, int y, int i) {
    const double rho_wall = g.rho[cidx(x, y)];
    const double ciu  = cx[i]*ulid;
    g.f[idx(i, x, y)] = g.f[idx(opp[i], x, y)]+ 2.0*w[i]*rho_wall*ciu/cs2;
}

inline void apply_boundaries(grid& g) {
    // bottom wall (static): reflect N, NE, NW
    for (int x = 0; x < nx; ++x) {
        reflect_static(g, x, 0, 2);
        reflect_static(g, x, 0, 5);
        reflect_static(g, x, 0, 6);
    }

    // left wall (static): reflect E, NE, SE
    for (int y = 0; y < ny; ++y) {
        reflect_static(g, 0, y, 1);
        reflect_static(g, 0, y, 5);
        reflect_static(g, 0, y, 8);
    }

    // right wall (static): reflect W, NW, SW
    for (int y = 0; y < ny; ++y) {
        reflect_static(g, nx - 1, y, 3);
        reflect_static(g, nx - 1, y, 6);
        reflect_static(g, nx - 1, y, 7);
    }

    // top lid (moving): reflect S, SW, SE with momentum term
    for (int x = 0; x < nx; ++x) {
        reflect_lid(g, x, ny - 1, 4);
        reflect_lid(g, x, ny - 1, 7);
        reflect_lid(g, x, ny - 1, 8);
    }
}

} // namespace lbm