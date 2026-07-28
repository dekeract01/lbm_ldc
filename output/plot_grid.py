#!/usr/bin/env python3
# plot_grid.py :: plot rho, ux, uy from an LBM HDF5 dump

import os
import h5py
import numpy as np
import matplotlib.pyplot as plt

# --- settings ---
file = 'fields_040000.h5'
show = False          # True: display windows; False: save PNGs instead

here = os.path.dirname(os.path.abspath(__file__))
path = os.path.join(here, file)
stem = os.path.splitext(file)[0]   # e.g. 'fields_040000', used for PNG names

def finish(fig, name):
    # show on screen, or save a PNG named <dumpstem>_<name>.png next to the script
    if show:
        plt.show()
    else:
        out = os.path.join(here, f"{stem}_{name}.png")
        fig.savefig(out, dpi=150)
        print("saved", out)
        plt.close(fig)

print("reading", path)
with h5py.File(path, "r") as f:
    rho = f["rho"][:]
    ux  = f["ux"][:]
    uy  = f["uy"][:]

print("rho min/max:", rho.min(), rho.max())
print("ux  min/max:", ux.min(), ux.max())
print("uy  min/max:", uy.min(), uy.max())

# --- field maps ---
fig, ax = plt.subplots(1, 3, figsize=(14, 4))
for a, (name, data) in zip(ax, [("rho", rho), ("ux", ux), ("uy", uy)]):
    im = a.imshow(data, origin="lower")   # origin lower: y points up
    a.set_title(name)
    fig.colorbar(im, ax=a)
plt.tight_layout()
finish(fig, "fields")

# --- centerline profiles (normalize by u_lid = 0.1) ---
ulid = 0.1
ny, nx = ux.shape
ux_vert = ux[:, nx//2] / ulid
uy_horiz = uy[ny//2, :] / ulid

fig2, (a1, a2) = plt.subplots(1, 2, figsize=(11, 4))
a1.plot(ux_vert, np.arange(ny)/(ny-1))
a1.set_xlabel("ux / u_lid"); a1.set_ylabel("y/L"); a1.set_title("vertical centerline")
a2.plot(np.arange(nx)/(nx-1), uy_horiz)
a2.set_xlabel("x/L"); a2.set_ylabel("uy / u_lid"); a2.set_title("horizontal centerline")

# Ghia et al. (1982), Re=1000
ghia_y = [0.0000, 0.0547, 0.0625, 0.0703, 0.1016, 0.1719, 0.2813, 0.4531,
          0.5000, 0.6172, 0.7344, 0.8516, 0.9531, 0.9609, 0.9688, 0.9766, 1.0000]
ghia_u = [0.0000, -0.1811, -0.2020, -0.2222, -0.2973, -0.3829, -0.2781, -0.1065,
          -0.0608, 0.0570, 0.1872, 0.3330, 0.4661, 0.5122, 0.5722, 0.6558, 1.0000]

ghia_x = [0.0000, 0.0625, 0.0703, 0.0781, 0.0938, 0.1563, 0.2266, 0.2344,
          0.5000, 0.8047, 0.8594, 0.9063, 0.9453, 0.9531, 0.9609, 0.9688, 1.0000]
ghia_v = [0.0000, 0.2749, 0.2901, 0.3035, 0.3263, 0.3710, 0.3308, 0.3224,
          0.0253, -0.3197, -0.4267, -0.5150, -0.3919, -0.3371, -0.2767, -0.2245, 0.0000]

a1.scatter(ghia_u, ghia_y, c='r', s=18, zorder=5, label='Ghia 1000')
a2.scatter(ghia_x, ghia_v, c='r', s=18, zorder=5, label='Ghia 1000')
a1.legend(); a2.legend()
plt.tight_layout()
finish(fig2, "centerlines")

# --- streamlines ---
x = np.arange(nx)
y = np.arange(ny)
speed = np.sqrt(ux**2 + uy**2)

fig3, ax3 = plt.subplots(figsize=(7, 7))
strm = ax3.streamplot(x, y, ux, uy, density=1.5, color=speed,
                      cmap="viridis", linewidth=1.0)
fig3.colorbar(strm.lines, ax=ax3, label="|u|")
ax3.set_title("streamlines")
ax3.set_xlabel("x"); ax3.set_ylabel("y")
ax3.set_aspect("equal")
ax3.set_xlim(0, nx-1); ax3.set_ylim(0, ny-1)
plt.tight_layout()
finish(fig3, "streamlines")