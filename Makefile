# -----------------------------------------------------
# D2Q9 Lattice Boltzmann Makefile (HDF5-MPI build)
# -----------------------------------------------------
#
# NOTE: this code is NOT MPI-parallel — it's a serial solver. mpicxx is used
# only because the parallel HDF5 headers hard-include <mpi.h>; the wrapper
# supplies those paths. The binary is serial: run it with ./d2q9, not mpirun.
# -----------------------------------------------------

CXX := mpicxx

CXXFLAGS := -std=c++20 -O3 -march=native -Wall -Wextra
CPPFLAGS := -Iinclude -MMD -MP

# HDF5 (parallel/MPI build via homebrew)
HDF5_PREFIX := $(shell brew --prefix hdf5-mpi)
CPPFLAGS += -I$(HDF5_PREFIX)/include
LDFLAGS  := -L$(HDF5_PREFIX)/lib -lhdf5

# Directories
SRC_DIR := src
OBJ_DIR := build
OUT_DIR := output

# Executable
TARGET := d2q9

# Source, object and dependency files
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

# -----------------------------------------------------
# Build
# -----------------------------------------------------

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Include automatically generated header dependencies
-include $(DEP)

# -----------------------------------------------------
# Run
# -----------------------------------------------------

run: $(TARGET)
	./$(TARGET)

# -----------------------------------------------------
# Clean
# -----------------------------------------------------

# clean: remove build artifacts only
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# clean-output: remove simulation dumps (separate, so a normal clean
# never nukes your results by accident)
clean-output:
	rm -f $(OUT_DIR)/*.h5 $(OUT_DIR)/*.xdmf

# clean-all: everything
clean-all: clean clean-output

# -----------------------------------------------------
# Help
# -----------------------------------------------------

help:
	@echo "Available targets:"
	@echo "  make               Build the executable"
	@echo "  make run           Build and run"
	@echo "  make clean         Remove build files (build/, d2q9)"
	@echo "  make clean-output  Remove output dumps (*.h5, *.xdmf)"
	@echo "  make clean-all     Remove everything"

.PHONY: all run clean clean-output clean-all help