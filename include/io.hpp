// io.hpp :: field output
#pragma once
#include "grid.hpp"

namespace lbm {

    void write_h5(const grid& g, int step);   // dumps rho, ux, uy to output/fields_NNNNNN.h5
    void write_xdmf(int step);                // XML wrapper so ParaView sees proper geometry

} // namespace lbm