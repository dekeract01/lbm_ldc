#!/usr/bin/env python3
# plot_grid.py :: plot rho, ux, uy from an LBM HDF5 dump

import glob, os, sys
import h5py
import numpy as np
import matplotlib.pyplot as plt

here = os.path.dirname(os.path.abspath(__file__))

# file from command line, else newest fields_*.h5 next to this script
if len(sys.argv) > 1:
    path = sys.argv[1]
else:
    files = sorted(glob.glob(os.path.join(here, "fields_*.h5")))
    if not files:
        sys.exit("no fields_*.h5 found")
    path = files[-1]

print("reading", path)
with h5py.File(path, "r") as f:
    rho = f["rho"][:]
    ux  = f["ux"][:]
    uy  = f["uy"][:]

print("rho min/max:", rho.min(), rho.max())
print("ux  min/max:", ux.min(), ux.max())
print("uy  min/max:", uy.min(), uy.max())

fig, ax = plt.subplots(1, 3, figsize=(14, 4))
for a, (name, data) in zip(ax, [("rho", rho), ("ux", ux), ("uy", uy)]):
    im = a.imshow(data, origin="lower")   # origin lower: y points up
    a.set_title(name)
    fig.colorbar(im, ax=a)
plt.tight_layout()
plt.show()