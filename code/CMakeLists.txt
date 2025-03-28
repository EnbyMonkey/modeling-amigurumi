cmake_minimum_required(VERSION 3.15)
project(amigurumi_designer VERSION 0.3.2 LANGUAGES C CXX) # the name of the project, project version and languages used

# specify c++ standard minimum
set(CMAKE_CXX_STANDARD 20) # 20 needed for graaf, 17 needed for pretty much everything else
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# get information about the compiler used, which is needed to setting compiler flags later on
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>") # the syntax '$<...>' is called a generator expression
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>") # I'm guessing this one is for compiling on Windows

# create a config file to pass variables to the project build
configure_file(include/config.h.in config.h)


include(FetchContent)

# GSL -- Guidelines Support Library
FetchContent_Declare(GSL
    GIT_REPOSITORY "https://github.com/microsoft/GSL"
    GIT_TAG "v4.1.0"
    GIT_SHALLOW ON
)

# graaflib -- graphing library
FetchContent_Declare(
        graaf
        GIT_REPOSITORY "https://github.com/bobluppes/graaf.git"
        GIT_TAG main
)

# set some flags for graaflib
set(SKIP_TESTS ON)
set(SKIP_BENCHMARKS ON)
set(SKIP_EXAMPLES ON)

# ceres solver
FetchContent_Declare(
    ceres
    GIT_REPOSITORY "https://github.com/ceres-solver/ceres-solver"
    GIT_TAG         46b4b3b002994ddb9d6fc72268c3e271243cd1df # commit from feb 19, 2025
)

# set some flags for ceres
set(CMAKE_BUILD_TYPE "Release") # default is debug which has very poor performance

# polyscope
FetchContent_Declare(
    polyscope
    GIT_REPOSITORY "https://github.com/nmwsharp/polyscope"
    GIT_TAG         d76bf6552f8db0956ce071dc1eb9a402c45a90ae # commit from feb 4, 2025
)

FetchContent_MakeAvailable(GSL graaf ceres polyscope)

set(CMAKE_BUILD_TYPE "Debug") # reset to debug so that we can launch and run debugger in vscode

# our own source files
add_executable(amigurumi-designer
    run/test_polyscope.cpp
    graph/graph.cpp
    graph/graphcontrol.cpp
    graph/ceressolver.cpp
    crochet/Project.cpp
    crochet/Piece.cpp
    crochet/Pattern.cpp
    crochet/Row.cpp
    crochet/Action.cpp
    crochet/Stitch.cpp
    rendering/model/Model.cpp
    rendering/model/IcoSphere.cpp
)

target_include_directories(amigurumi-designer PRIVATE graph/ crochet/ rendering/ include/ /usr/include/eigen3 ${graaflib_SOURCE_DIR}/include/ ${PROJECT_BINARY_DIR})
target_link_libraries(amigurumi-designer PRIVATE
    Microsoft.GSL::GSL
    ceres
    Graaf::Graaf
    polyscope
)

# idk what this is for
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
