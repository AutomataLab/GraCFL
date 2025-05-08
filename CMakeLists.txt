# Minimum CMake version
cmake_minimum_required(VERSION 3.26)

# Set the project name
project(GraCFL)
set(EXE_NAME gracfl)

# Set C++ standard (optional)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set Release build type by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add -O3 optimization flag for Release builds
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -flto")

# Set the source and include directories
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

# Include the directory for header files
include_directories(${INCLUDE_DIR})

# Find OpenMP
find_package(OpenMP)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Create the bin directory if it doesn't exist
file(MAKE_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# Use ExternalProject to download and build jemalloc if it isn't found
include(ExternalProject)

# Try to find jemalloc first on the system
find_library(JEMALLOC_LIB jemalloc)

# If jemalloc is not found, download and build it using ExternalProject
if(NOT JEMALLOC_LIB)
    message(STATUS "jemalloc not found, downloading and building it...")

    ExternalProject_Add(
        jemalloc
        GIT_REPOSITORY https://github.com/jemalloc/jemalloc.git
        GIT_TAG 5.3.0  # Specify the version you want to use
        CONFIGURE_COMMAND ./autogen.sh --prefix=${CMAKE_BINARY_DIR}/jemalloc_install --libdir=${CMAKE_BINARY_DIR}/jemalloc_install/lib --with-private-libpthread
        BUILD_COMMAND make
        INSTALL_COMMAND make install
        BUILD_IN_SOURCE 1
        PREFIX ${CMAKE_BINARY_DIR}/_deps/jemalloc
    )

    # Set the path to jemalloc built library and include directories
    ExternalProject_Get_Property(jemalloc install_dir)
    set(JEMALLOC_LIB "${CMAKE_BINARY_DIR}/jemalloc_install/lib/libjemalloc.a")
    include_directories("${CMAKE_BINARY_DIR}/jemalloc_install/include")

    # Create a target for jemalloc
    add_custom_target(libjemalloc DEPENDS ${JEMALLOC_LIB})

    # Set jemalloc library path and link pthread, dl
    set(JEMALLOC_LIBS ${JEMALLOC_LIB} pthread dl)
else()
    set(JEMALLOC_LIBS ${JEMALLOC_LIB})
endif()

# Try to find TBB
find_package(TBB)

# If TBB is not found, download and build it using ExternalProject
if(NOT TBB_FOUND)
    message(STATUS "TBB not found, downloading and building it...")

    ExternalProject_Add(
        tbb
        GIT_REPOSITORY https://github.com/oneapi-src/oneTBB.git
        GIT_TAG v2021.13.0  # Specify the version you want to use
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/tbb_install -DCMAKE_INSTALL_LIBDIR=lib -DTBB_TEST=OFF  # Disable tests
        BUILD_COMMAND ${CMAKE_COMMAND} --build . --target install
        INSTALL_COMMAND ""
        BUILD_IN_SOURCE 1
        PREFIX ${CMAKE_BINARY_DIR}/_deps/tbb
    )

    # Set the path to the built TBB library and include directories (using lib64)
    set(TBB_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/tbb_install/include")
    set(TBB_LIB "${CMAKE_BINARY_DIR}/tbb_install/lib/libtbb${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(TBB_MALLOC_LIB "${CMAKE_BINARY_DIR}/tbb_install/lib/libtbbmalloc${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(TBB_MALLOC_PROXY_LIB "${CMAKE_BINARY_DIR}/tbb_install/lib/libtbbmalloc_proxy${CMAKE_SHARED_LIBRARY_SUFFIX}")

    include_directories(${TBB_INCLUDE_DIRS})

    # Create a target for TBB
    add_custom_target(libTBB DEPENDS ${TBB_LIB} ${TBB_MALLOC_LIB} ${TBB_MALLOC_PROXY_LIB})
else()
    set(TBB_LIB TBB::tbb)
    set(TBB_MALLOC_LIB TBB::tbbmalloc)
    set(TBB_MALLOC_PROXY_LIB TBB::tbbmalloc_proxy)
endif()

# Find all .cpp files in the src directory
file(GLOB GRACFL_SOURCES 
    "${SRC_DIR}/main.cpp"
    "${SRC_DIR}/*/*.cpp"
    "${SRC_DIR}/*/*/*.cpp")

# ––– Define the executable
add_executable(gracfl
  ${GRACFL_SOURCES}
)

# —————————————————————————————————————————————————————————
# Link TBB
# —————————————————————————————————————————————————————————
if(TBB_FOUND)
    target_link_libraries(${EXE_NAME} PRIVATE
        TBB::tbb
        TBB::tbbmalloc
        TBB::tbbmalloc_proxy
    )
else()
    # assume you have libTBB target or variables pointing to your system libs
    add_dependencies(${EXE_NAME} libTBB)
    target_link_libraries(${EXE_NAME} PRIVATE
        ${TBB_LIB}
        ${TBB_MALLOC_LIB}
        ${TBB_MALLOC_PROXY_LIB}
    )
endif()

# —————————————————————————————————————————————————————————
# Link OpenMP
# —————————————————————————————————————————————————————————
if(OpenMP_CXX_FOUND)
    target_link_libraries(${EXE_NAME} PRIVATE
        OpenMP::OpenMP_CXX
    )
endif()
