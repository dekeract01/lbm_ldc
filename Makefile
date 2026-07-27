# -----------------------------------------------------
# D2Q9 Lattice Boltzmann Makefile (HDF5-MPI build)
# -----------------------------------------------------

# Compiler — mpicxx is a wrapper over the real compiler that adds MPI
# include/lib paths. The code itself is serial; this only satisfies the
# <mpi.h> include that parallel HDF5 headers pull in.
CXX := mpicxx

# Compiler flags
CXXFLAGS := -std=c++20 -O3 -march=native -Wall -Wextra
CPPFLAGS := -Iinclude -MMD -MP

# HDF5 (parallel/MPI build via homebrew)
HDF5_PREFIX := $(shell brew --prefix hdf5-mpi)
CPPFLAGS += -I$(HDF5_PREFIX)/include
LDFLAGS  := -L$(HDF5_PREFIX)/lib -lhdf5

# Directories
SRC_DIR := src
OBJ_DIR := build

# Executable
TARGET := d2q9

# Source, object and dependency files
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

# Output files
OUTPUT := output/d2q9_solution_cpp.dat
LOG := d2q9.out.txt

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
	@./$(TARGET) > $(LOG); \
	status=$$?; \
	cat $(LOG); \
	exit $$status

test: all run

bench: $(TARGET)
	@echo "Running 5 benchmark iterations..."
	@for i in 1 2 3 4 5; do \
		echo "Run $$i"; \
		./$(TARGET); \
		echo ""; \
	done

# -----------------------------------------------------
# Compiler selection
# -----------------------------------------------------

gcc:
	$(MAKE) CXX=g++

clang:
	$(MAKE) CXX=clang++

# -----------------------------------------------------
# Optimisation reports
# -----------------------------------------------------

profile-gcc:
	$(MAKE) CXXFLAGS="$(CXXFLAGS) -fopt-info-vec-optimized"

profile-clang:
	$(MAKE) CXX=clang++ CXXFLAGS="$(CXXFLAGS) -Rpass=loop-vectorize"

# -----------------------------------------------------
# Clean
# -----------------------------------------------------

clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(LOG) *.png *.pdf

# -----------------------------------------------------
# Help
# -----------------------------------------------------

help:
	@echo "Available targets:"
	@echo "  make               Build the executable"
	@echo "  make run           Build and run"
	@echo "  make test          Same as run"
	@echo "  make bench         Run benchmark (5 iterations)"
	@echo "  make clean         Remove build files"
	@echo "  make gcc           Build with GCC"
	@echo "  make clang         Build with Clang"
	@echo "  make profile-gcc   GCC vectorisation report"
	@echo "  make profile-clang Clang vectorisation report"

.PHONY: all run test bench clean gcc clang profile-gcc profile-clang help