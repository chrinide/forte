if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()

cmake_policy(SET CMP0048 NEW)  # project_VERSION* variables populated from project(... VERSION x.x.x) string

project(forte
        VERSION 0.1
        LANGUAGES CXX C)
set(forte_AUTHORS      "Francesco A. Evangelista and Group")
set(forte_DESCRIPTION  "Quantum Chemistry Methods for Strongly Correlated Electrons plugin to Psi4")
set(forte_URL          "https://github.com/evangelistalab/forte")
set(forte_LICENSE      "GPL-3.0+")
set(CMAKE_CXX_STANDARD 14)
set(TargetOpenMP_FIND_COMPONENTS "CXX")

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(psi4OptionsTools)

option_with_print(ENABLE_CheMPS2 "Enable CheMPS2 solver" OFF)
option_with_print(ENABLE_OPENMP "Enable threadsafe linking to OpenMP parallelized programs" ON)
option_with_print(ENABLE_MPI "Enable MPI parallelization" OFF)
option_with_print(ENABLE_GA "Enable Global Arrays" OFF)
option_with_print(MAX_DET_ORB "Set the maximum number of orbitals in a determinant" OFF)
option_with_print(ENABLE_CODECOV "Enable compilation with code coverage flags" OFF)
#option_with_print(ENABLE_GENERIC "Enable mostly static linking in shared library" OFF)

include(autocmake_omp)
include(autocmake_mpi)  # MPI option A
#include(custom_static_library)

#find_package(psi4 1.1 REQUIRED COMPONENTS ambit chemps2)
find_package(psi4 1.1 REQUIRED)

find_package(TargetLAPACK REQUIRED)
find_package(ambit REQUIRED)
if(ENABLE_CheMPS2)
    find_package(CheMPS2 1.8.3 CONFIG REQUIRED)
endif()

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
#    target_link_libraries(forte ${Boost_LIBRARIES})
endif()

# Including custom cmake rules
include(cmake/clang-cxx-dev-tools.cmake)
include(cmake/git-version.cmake)

# Check if we can compile with SSE4.2
check_cxx_compiler_flag("-msse4.2" SSE42_FLAG)
if(SSE42_FLAG)
    message("-- Adding SSE4.2 Flag")
    set(CMAKE_CXX_FLAGS "-msse4.2")
endif()
add_compile_options(-Wall -Wextra -pedantic) # -Werror)

if(ENABLE_CODECOV)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage") # enabling coverage
endif()

# List of CC files
add_psi4_plugin(forte
src/base_classes/active_space_method.cc
src/base_classes/active_space_solver.cc
src/orbital-helpers/avas.cc
src/casscf/casscf.cc
src/ci_rdm/ci_rdms.cc
src/ci_rdm/ci_rdms_dynamic.cc
src/sparse_ci/ci_reference.cc
src/helpers/davidson.cc
src/sparse_ci/determinant_hashvector.cc
src/helpers/davidson.cc
src/dmrg/dmrgscf.cc
src/dmrg/dmrgsolver.cc
src/base_classes/dynamic_correlation_solver.cc
src/finite_temperature/finite_temperature.cc
src/forte_old_methods.cc
src/helpers/iterative_solvers.cc
src/helpers/helpers.cc
src/main.cc
src/sparse_ci/operator.cc
src/options.cc
src/orbital-helpers/orbitaloptimizer.cc
src/python_api.cc
src/base_classes/rdms.cc
src/base_classes/state_info.cc
src/base_classes/scf_info.cc
src/base_classes/forte_options.cc
src/base_classes/mo_space_info.cc
src/base_classes/orbital_transform.cc
src/sci/aci.cc
src/sci/aci_build_F.cc
src/sci/asci.cc
src/sci/fci_mo.cc
src/sci/mrpt2.cc
src/sci/sci.cc
src/ci_ex_states/excited_state_solver.cc
src/sparse_ci/sparse_ci_solver.cc
src/sparse_ci/sigma_vector.cc
src/sparse_ci/sigma_vector_dynamic.cc
src/sparse_ci/stl_bitset_determinant.cc
src/sparse_ci/stl_bitset_string.cc
src/sparse_ci/sorted_string_list.cc
src/sparse_ci/ui64_determinant.cc
src/sparse_ci/ui64_determinant_functions.cc
src/orbital-helpers/aosubspace.cc
src/orbital-helpers/ci-no/ci-no.cc
src/orbital-helpers/ci-no/mrci-no.cc
src/fci/binary_graph.cc
src/fci/fci_solver.cc
src/fci/fci_vector.cc
src/fci/fci_vector_h_diag.cc
src/fci/fci_vector_hamiltonian.cc
src/fci/fci_vector_rdm.cc
src/fci/string_hole_list.cc
src/fci/string_lists.cc
src/fci/string_oo_list.cc
src/fci/string_vo_list.cc
src/fci/string_vvoo_list.cc
src/helpers/blockedtensorfactory.cc
src/helpers/printing.cc
src/helpers/string_algorithms.cc
src/integrals/cholesky_integrals.cc
src/integrals/conventional_integrals.cc
src/integrals/custom_integrals.cc
src/integrals/df_integrals.cc
src/integrals/diskdf_integrals.cc
src/integrals/distribute_df_integrals.cc
src/integrals/integrals.cc
src/integrals/own_integrals.cc
src/integrals/parallel_ccvv_algorithms.cc
src/integrals/paralleldfmo.cc
src/integrals/make_integrals.cc
src/integrals/active_space_integrals.cc
src/mrdsrg-helper/dsrg_source.cc
src/mrdsrg-helper/dsrg_time.cc
src/mrdsrg-helper/dsrg_transformed.cc
src/mrdsrg-helper/run_dsrg.cc
src/mrdsrg-so/mrdsrg_so.cc
src/mrdsrg-so/so-mrdsrg.cc
src/mrdsrg-spin-adapted/dsrg_mrpt.cc
src/mrdsrg-spin-adapted/dsrg_mrpt_2nd.cc
src/mrdsrg-spin-adapted/dsrg_mrpt_amp.cc
src/mrdsrg-spin-adapted/dsrg_mrpt_comm.cc
src/mrdsrg-spin-integrated/master_mrdsrg.cc
src/mrdsrg-spin-integrated/dsrg_mrpt2.cc
src/mrdsrg-spin-integrated/dsrg_mrpt2_ms.cc
src/mrdsrg-spin-integrated/dsrg_mrpt3.cc
src/mrdsrg-spin-integrated/mcsrgpt2_mo.cc
src/mrdsrg-spin-integrated/mrdsrg.cc
src/mrdsrg-spin-integrated/mrdsrg_amplitude.cc
src/mrdsrg-spin-integrated/mrdsrg_commutator.cc
src/mrdsrg-spin-integrated/mrdsrg_nonpt.cc
src/mrdsrg-spin-integrated/mrdsrg_pt.cc
src/mrdsrg-spin-integrated/mrdsrg_smart_s.cc
src/mrdsrg-spin-integrated/mrdsrg_srg.cc
src/mrdsrg-spin-integrated/three_dsrg_mrpt2.cc
src/mrdsrg-spin-integrated/active_dsrgpt2.cc
src/mrdsrg-spin-integrated/dwms_mrpt2.cc
src/orbital-helpers/ao_helper.cc
src/orbital-helpers/es-nos.cc
src/orbital-helpers/localize.cc
src/orbital-helpers/iao_builder.cc
src/orbital-helpers/mp2_nos.cc
src/orbital-helpers/semi_canonicalize.cc
src/orbital-helpers/unpaired_density.cc
src/pci/pci.cc
src/pci/pci_sigma.cc
src/v2rdm/v2rdm.cc
)

target_link_libraries(forte PRIVATE ambit::ambit)
if(TARGET CheMPS2::chemps2)
    target_link_libraries(forte PRIVATE CheMPS2::chemps2)
    add_definitions(-DHAVE_CHEMPS2)
endif()

target_include_directories(forte PRIVATE src)

## 64bit implementation
if(MAX_DET_ORB)
    add_definitions(-DMAX_DET_ORB=${MAX_DET_ORB})
else()
    add_definitions(-DMAX_DET_ORB=64)
    add_definitions(-DSMALL_BITSET)
endif()

if(ENABLE_MPI)
    target_link_libraries(forte PRIVATE ${MPI_CXX_LIBRARIES})  # MPI option A
    #target_link_libraries(forte PRIVATE MPI::MPI_CXX)  # MPI option B
    #target_add_definitions(forte PRIVATE HAVE_MPI)  # MPI option B
endif()

if(ENABLE_GA)
    target_link_libraries(forte PRIVATE GlobalArrays::ga)
endif()

#
# Forte tests
#
option(ENABLE_ForteTests "Enable Forte tests." ON)


if (ENABLE_ForteTests)
  project (forte_tests)
  include_directories(${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}/catch2/src/catch2/single_include)
  add_executable(forte_tests
    src/sparse_ci/ui64_determinant.cc
    tests/code/test_determinant.cc)

  project (forte_benchmarks)
  include_directories(${CMAKE_BINARY_DIR})
  add_executable(forte_benchmarks
    src/sparse_ci/ui64_determinant.cc
    tests/benchmark/determinant_benchmark.cc)

endif (ENABLE_ForteTests)
