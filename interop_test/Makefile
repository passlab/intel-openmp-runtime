LIBIOMP_BUILD=../../intel-openmp-runtime-build

LIBIOMP_PATH = ${LIBIOMP_BUILD}/src
LIBIOMP_INCLUDE = ${LIBIOMP_BUILD}/src

EXE = omp_set_wait_policy_test_1 omp_quiesce_test_1

# Do the following export before run the program
# export LD_LIBRARY_PATH=${LIBIOMP_PATH}:$LD_LIBRARY_PATH

# For Clang/LLVM using interop implementation on Intel OpenMP runtime
CC = clang
CFLAGS = -fopenmp -I${LIBIOMP_INCLUDE} -L${LIBIOMP_PATH} -liomp5

# For icc using interop implementation on Intel OpenMP runtime
# CC = icc
# CFLAGS = -fopenmp -I${LIBIOMP_INCLUDE} -L${LIBIOMP_PATH} -liomp5

# For GCC using interop implementation on GOMP runtime
# CC = gcc
# CFLAGS = -fopenmp ...
 
all: ${EXE}

.c:
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@

clean:
	rm -rf ${EXE} a.out *.o
