cmake_minimum_required(VERSION 3.26)
project(GraCFL)

set(EXE_NAME gracfl)
set(LIB_NAME graCFLlib)

# C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Default to Release build
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Source and include directories
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories(${INCLUDE_DIR})

# Output binary directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
file(MAKE_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# Find OpenMP
find_package(OpenMP)

# External jemalloc setup
include(ExternalProject)
find_library(JEMALLOC_LIB jemalloc)

if(NOT JEMALLOC_LIB)
    message(STATUS "jemalloc not found, downloading and building it...")

    ExternalProject_Add(
        jemalloc
        GIT_REPOSITORY https://github.com/jemalloc/jemalloc.git
        GIT_TAG 5.3.0
        CONFIGURE_COMMAND ./autogen.sh --prefix=${CMAKE_BINARY_DIR}/jemalloc_install --libdir=${CMAKE_BINARY_DIR}/jemalloc_install/lib --with-private-libpthread
        BUILD_COMMAND make
        INSTALL_COMMAND make install
        BUILD_IN_SOURCE 1
        PREFIX ${CMAKE_BINARY_DIR}/_deps/jemalloc
    )

    ExternalProject_Get_Property(jemalloc install_dir)
    set(JEMALLOC_LIB "${CMAKE_BINARY_DIR}/jemalloc_install/lib/libjemalloc.a")
    include_directories("${CMAKE_BINARY_DIR}/jemalloc_install/include")
    add_custom_target(libjemalloc DEPENDS ${JEMALLOC_LIB})
    set(JEMALLOC_LIBS ${JEMALLOC_LIB} pthread dl)
else()
    set(JEMALLOC_LIBS ${JEMALLOC_LIB})
endif()

# External TBB setup
find_package(TBB)

if(NOT TBB_FOUND)
    message(STATUS "TBB not found, downloading and building it...")

    ExternalProject_Add(
        tbb
        GIT_REPOSITORY https://github.com/oneapi-src/oneTBB.git
        GIT_TAG v2021.13.0
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/tbb_install -DCMAKE_INSTALL_LIBDIR=lib -DTBB_TEST=OFF
        BUILD_COMMAND ${CMAKE_COMMAND} --build . --target install
        INSTALL_COMMAND ""
        BUILD_IN_SOURCE 1
        PREFIX ${CMAKE_BINARY_DIR}/_deps/tbb
    )

    set(TBB_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/tbb_install/include")
    set(TBB_LIB "${CMAKE_BINARY_DIR}/tbb_install/lib/libtbb${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(TBB_MALLOC_LIB "${CMAKE_BINARY_DIR}/tbb_install/lib/libtbbmalloc${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(TBB_MALLOC_PROXY_LIB "${CMAKE_BINARY_DIR}/tbb_install/lib/libtbbmalloc_proxy${CMAKE_SHARED_LIBRARY_SUFFIX}")
    include_directories(${TBB_INCLUDE_DIRS})
    add_custom_target(libTBB DEPENDS ${TBB_LIB} ${TBB_MALLOC_LIB} ${TBB_MALLOC_PROXY_LIB})
else()
    set(TBB_LIB TBB::tbb)
    set(TBB_MALLOC_LIB TBB::tbbmalloc)
    set(TBB_MALLOC_PROXY_LIB TBB::tbbmalloc_proxy)
endif()

# Find all .cpp files in the src directory
# file(GLOB MAIN_SOURCE
#     "${SRC_DIR}/main.cpp"
#     "${SRC_DIR}/*/*.cpp"
#     "${SRC_DIR}/*/*/*.cpp")

# # Find all sources
file(GLOB_RECURSE ALL_SOURCES "${SRC_DIR}/*.cpp")

# Split sources into library and main
set(LIB_SOURCES "")
set(MAIN_SOURCE "")

# file(GLOB LIB_SOURCES
#     "${SRC_DIR}/*/*.cpp"
#     "${SRC_DIR}/*/*/*.cpp")

# file(GLOB MAIN_SOURCE
#     "${SRC_DIR}/main.cpp")

foreach(source_file IN LISTS ALL_SOURCES)
    if(source_file MATCHES "main\\.cpp$")
        set(MAIN_SOURCE ${source_file})
    else()
        list(APPEND LIB_SOURCES ${source_file})
    endif()
endforeach()

# Build the static library
add_library(${LIB_NAME} STATIC ${LIB_SOURCES})
target_include_directories(${LIB_NAME} PUBLIC ${INCLUDE_DIR})

message(STATUS "→ graCFLlib will compile these sources:\n   ${LIB_SOURCES}")


# Link dependencies to the library
target_link_libraries(${LIB_NAME} PRIVATE ${JEMALLOC_LIBS})
if(TBB_FOUND)
    target_link_libraries(${LIB_NAME} PRIVATE TBB::tbb TBB::tbbmalloc TBB::tbbmalloc_proxy)
else()
    add_dependencies(${LIB_NAME} libTBB)
    target_link_libraries(${LIB_NAME} PRIVATE ${TBB_LIB} ${TBB_MALLOC_LIB} ${TBB_MALLOC_PROXY_LIB})
endif()

if(OpenMP_CXX_FOUND)
    target_link_libraries(${LIB_NAME} PRIVATE OpenMP::OpenMP_CXX)
endif()

# Build the executable
add_executable(${EXE_NAME} ${MAIN_SOURCE})
target_link_libraries(${EXE_NAME} PRIVATE ${LIB_NAME})
