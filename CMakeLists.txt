cmake_minimum_required(VERSION 3.28)

# Set the project name based on the folder name
project(Minesweeper)

# Location of executable
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set include directories
include_directories(inc/
    inc/ui/
    inc/core/
    inc/base/
    ${CMAKE_SOURCE_DIR}
)

# Make SFML available
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.x
    GIT_PROGRESS TRUE
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

include_directories(${SFML_INCLUDE})

# Add source files to a list
set(
    SOURCES
    
    ./main.cpp
    src/core/Cell.cpp
    src/core/Game.cpp
    src/core/Field.cpp
    src/core/Controller.cpp
    src/base/State.cpp
    src/base/Command.cpp
    #src/ui/Console/Console.cpp
    #src/ui/Console/Display.cpp
    
    src/ui/Graphics/GraphicalView.cpp
    src/ui/Graphics/TextureManager.cpp
    src/ui/Graphics/Tile.cpp
)


# Add the executable
add_executable(Minesweeper ${SOURCES})

if(${CMAKE_BUILD_TYPE} EQUAL "DEBUG")
    target_compile_options(Minesweeper PRIVATE "-fno-access-control")
endif(${CMAKE_BUILD_TYPE} EQUAL "DEBUG")

# Link with SFML
target_link_libraries(Minesweeper PRIVATE SFML::Graphics)

# Copy assets to binary directory
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})