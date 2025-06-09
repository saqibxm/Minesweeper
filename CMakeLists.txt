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
        ./common.h
        ./utility.h
        ./stopwatch.h

        inc/base/Subject.hpp
        inc/base/Observer.hpp
        inc/base/Command.hpp
        inc/base/State.hpp
        inc/base/NumberGenerator.hpp

        inc/core/Cell.hpp
        inc/core/Controller.hpp
        inc/core/Field.hpp
        inc/core/Game.hpp

        inc/ui/View.hpp
        #inc/ui/Console/ConsoleView.hpp

        inc/ui/Graphics/GraphicalView.hpp
        inc/ui/Graphics/LayoutConfig.hpp
        inc/ui/PlaceholderTexture.h
        inc/ui/Graphics/Tile.hpp
        inc/ui/Graphics/TextureManager.hpp
        inc/ui/Graphics/Smiley.hpp
        inc/ui/Graphics/Counter.hpp
        #inc/ui/Graphics/Block.hpp

        src/base/Command.cpp
        src/base/State.cpp
        src/base/NumberGenerator.cpp

        src/core/Cell.cpp
        src/core/Game.cpp
        src/core/Field.cpp
        src/core/Controller.cpp
    
        src/ui/Graphics/GraphicalView.cpp
        src/ui/Graphics/TextureManager.cpp
        src/ui/Graphics/Tile.cpp
        src/ui/Graphics/Smiley.cpp
        src/ui/Graphics/Counter.cpp
        src/ui/Graphics/DifficultySelector.cpp
        inc/ui/Graphics/DifficultySelector.hpp
        #src/ui/Graphics/Block.cpp

        #src/ui/Console/ConsoleView.cpp
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