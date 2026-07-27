// main_.cpp :: basic LBM component test

#include <iostream>

#include "constants.hpp"
#include "lattice.hpp"
#include "grid.hpp"
#include "io.hpp"

int main()
{
    std::cout << "D2Q9 LBM test\n";
    std::cout << "Grid: " << lbm::nx << " x " << lbm::ny << "\n";
    std::cout << "Cells: " << lbm::ncells << "\n";
    std::cout << "Re = " << lbm::Re << "\n";
    std::cout << "tau = " << lbm::tau << "\n";
    std::cout << "omega = " << lbm::omega << "\n";

    std::cout << "\nD2Q9 lattice:\n";
    for (int i = 0; i < lbm::q; i++) {
        std::cout << i << ": c=(" << lbm::cx[i] << "," << lbm::cy[i] << ") "
                  << "w=" << lbm::w[i] << " opp=" << lbm::opp[i] << "\n";
    }

    std::cout << "\nIndex test:\n";
    std::cout << "idx(0,10,10) = " << lbm::idx(0, 10, 10) << "\n";

    // --- build a grid and write it out ---
    lbm::grid g;
    lbm::init_rest(g);

    // scribble a known pattern so ParaView shows something non-flat:
    // ux ramps left-to-right, uy ramps bottom-to-top
    for (int y = 0; y < lbm::ny; ++y)
        for (int x = 0; x < lbm::nx; ++x) {
            g.ux[lbm::cidx(x, y)] = static_cast<double>(x);
            g.uy[lbm::cidx(x, y)] = static_cast<double>(y);
        }

    lbm::write_h5(g, 0);
    lbm::write_xdmf(0);
    std::cout << "\nwrote output/fields_000000.h5\n";

    return 0;
}