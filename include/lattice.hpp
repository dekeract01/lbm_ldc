// lattice.hpp :: d2q9 lattice definition
#pragma once

#include "constants.hpp"

namespace lbm {

    constexpr int q = 9;

    // lattice sound speed squared
    constexpr double cs2 = 1.0/3.0;

    // discrete velocity vectors
    // 0: rest
    // 1: east, 2: north, 3: west, 4: south
    // 5: northeast, 6: northwest, 7: southwest, 8: southeast
    constexpr int cx[q] = {0,  1,  0, -1,  0,  1, -1, -1,  1};
    constexpr int cy[q] = {0,  0,  1,  0, -1,  1,  1, -1, -1};

    // d2q9 lattice weights from 2d gauss-hermite quadrature
    //
    // the d2q9 lattice is constructed from the tensor product of two
    // one-dimensional three-point gauss-hermite quadratures:
    //
    // 1d weights:
    //   w(0)  = 2/3
    //   w(+1) = 1/6
    //   w(-1) = 1/6
    //
    // combining the x and y directions gives:
    //
    //   rest node:
    //       (2/3)*(2/3) = 4/9
    //
    //   axial nodes:
    //       (1/6)*(2/3) = 1/9
    //
    //   diagonal nodes:
    //       (1/6)*(1/6) = 1/36
    //
    // these weights ensure recovery of the correct low-order moments:
    // mass conservation and the isotropic pressure tensor required by
    // the navier-stokes equations.
    constexpr double w[q] = {
                4.0/9.0,
                1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0,
                1.0/36.0, 1.0/36.0, 1.0/36.0, 1.0/36.0
            };

    // opposite directions for bounce-back
    constexpr int opp[q] = {0, 3, 4, 1, 2, 7, 8, 5, 6};

    // 1d soa flat index helper
    inline constexpr int idx(int i, int x, int y){
        return i*ncells + y*nx + x;
    }

    // node index helper (one value per cell: rho, ux, uy)
    inline constexpr int cidx(int x, int y){
        return y*nx + x;
    }


} // namespace lbm