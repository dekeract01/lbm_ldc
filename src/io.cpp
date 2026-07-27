// io.cpp :: HDF5 field output for ParaView
#include "io.hpp"
#include "constants.hpp"
#include "lattice.hpp"

#include <hdf5.h>
#include <cstdio>
#include <fstream>

namespace lbm {

static void write_dataset(hid_t file, const char* name,
                          const double* data, hid_t space) {
    hid_t dset = H5Dcreate2(file, name, H5T_NATIVE_DOUBLE, space,
                            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    H5Dwrite(dset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
    H5Dclose(dset);
}

void write_h5(const grid& g, int step) {
    char fname[256];
    std::snprintf(fname, sizeof(fname), "output/fields_%06d.h5", step);

    hid_t file = H5Fcreate(fname, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    // scalar fields are stored y*nx + x  ==  row-major [ny][nx], write directly
    hsize_t dims[2] = { (hsize_t)ny, (hsize_t)nx };
    hid_t space = H5Screate_simple(2, dims, nullptr);

    write_dataset(file, "rho", g.rho.data(), space);
    write_dataset(file, "ux",  g.ux.data(),  space);
    write_dataset(file, "uy",  g.uy.data(),  space);

    H5Sclose(space);
    H5Fclose(file);
}

void write_xdmf(int step) {
    char fname[256], h5name[256];
    std::snprintf(fname,  sizeof(fname),  "output/fields_%06d.xdmf", step);
    std::snprintf(h5name, sizeof(h5name), "fields_%06d.h5", step);  // relative to xdmf

    std::ofstream o(fname);
    o << "<?xml version=\"1.0\" ?>\n<Xdmf Version=\"2.0\">\n  <Domain>\n";
    o << "    <Grid Name=\"cavity\" GridType=\"Uniform\">\n";
    o << "      <Topology TopologyType=\"2DCoRectMesh\" Dimensions=\""
      << ny << " " << nx << "\"/>\n";
    o << "      <Geometry GeometryType=\"ORIGIN_DXDY\">\n";
    o << "        <DataItem Dimensions=\"2\" Format=\"XML\">0 0</DataItem>\n";
    o << "        <DataItem Dimensions=\"2\" Format=\"XML\">1 1</DataItem>\n";
    o << "      </Geometry>\n";
    for (const char* name : {"rho", "ux", "uy"}) {
        o << "      <Attribute Name=\"" << name
          << "\" AttributeType=\"Scalar\" Center=\"Node\">\n";
        o << "        <DataItem Dimensions=\"" << ny << " " << nx
          << "\" NumberType=\"Float\" Precision=\"8\" Format=\"HDF\">"
          << h5name << ":/" << name << "</DataItem>\n";
        o << "      </Attribute>\n";
    }
    o << "    </Grid>\n  </Domain>\n</Xdmf>\n";
}

} // namespace lbm