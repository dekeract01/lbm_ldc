// equilibrium.hpp :: D2Q9 equilibrium distribution
#pragma once

#include "constants.hpp"
#include "lattice.hpp"

namespace lbm {

// equilibrium population for direction i at given macroscopic state
inline double compute_feq(int i, double rho, double ux, double uy) {
    const double cu = cx[i]*ux + cy[i]*uy;    // c_i . u
    const double uu = ux*ux + uy*uy;          // u . u
    return w[i]*rho*(1.0 + cu/cs2 + (cu*cu)/(2.0*cs2*cs2) - uu/(2.0*cs2));
}

} // namespace lbm