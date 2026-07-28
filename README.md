# D2Q9 Lattice Boltzmann, Lid-Driven Cavity

A from-scratch 2D lattice Boltzmann solver (D2Q9, BGK collision) for the
lid-driven cavity flow, written in C++. Validated against the Ghia et al.
(1982) benchmark at Re = 1000.

## Method

- **D2Q9** lattice, nine discrete velocities
- **BGK** single-relaxation-time collision
- **Pull-scheme** streaming with ping-pong buffers
- **Halfway bounce-back** on the three static walls (second-order accurate)
- **Moving-lid** bounce-back with a momentum-injection term on the top wall

The time loop is: compute moments, collide, stream, apply boundaries.

## Results

At Re = 1000 on a 128x128 grid, run to 40k steps, the centreline velocity
profiles fall on the Ghia et al. benchmark points, and the flow shows the
expected primary vortex (core near x/L ~ 0.53, y/L ~ 0.57) together with both
bottom-corner secondary vortices. Plots are written to the `output/` folder as
PNG files.

## Roadmap

- **Corner treatment.** The corner nodes show minor artifacts and the contours
  there look slightly off. The interior matches Ghia, so the issue is localised,
  but the corner scheme needs revisiting.

- **CUDA port.** The main planned direction. The pull scheme, ping-pong
  buffers, and header-inline per-node routines were chosen up front so the
  compute maps cleanly onto GPU kernels. The target is a fused collide-stream
  kernel with coalesced, direction-major memory access.

- **Higher resolution and Reynolds number.** Testing on 256x256 and finer
  grids, and at Reynolds numbers beyond 1000, where the secondary corner
  vortices grow and further vortices appear.

- **Alternative collision models.** Exploring collision operators beyond BGK,
  such as regularised LBM or a multiple-relaxation-time (MRT) scheme, to extend
  the stable range at higher Reynolds number and coarser grids.