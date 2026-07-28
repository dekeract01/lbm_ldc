// streaming.hpp :: pull-scheme propagation
#pragma once

#include "constants.hpp"
#include "lattice.hpp"
#include "grid.hpp"

namespace lbm {

// Pull streaming: each node gathers the population that flows INTO it along
// each direction, from the neighbor upstream of that direction.
//
// For direction i at node (x,y), the population arriving now started one
// step ago at the source neighbor (x - cx[i], y - cy[i]) and moved along c_i.
// We only pull when that source is inside the domain; edge directions whose
// source is outside are left unset in fnew and filled afterward by bounce-back.
//
// Writes into fnew, then swaps so the streamed data becomes current f.
inline void stream(grid& g) {
    for (int y = 0; y < ny; ++y)
        for (int x = 0; x < nx; ++x)
            for (int i = 0; i < q; ++i) {
                const int xs = x - cx[i];   // source node for direction i
                const int ys = y - cy[i];

                if (xs >= 0 && xs < nx && ys >= 0 && ys < ny) {
                    g.fnew[idx(i, x, y)] = g.f[idx(i, xs, ys)];
                }
                // else: source outside domain -> leave as hole for bounce-back
            }

    g.f.swap(g.fnew);   // streamed data in fnew becomes current f
}

} // namespace lbm