cmake_minimum_required(VERSION 3.28)

# Set the project name based on the folder name
project(Minesweeper)

# Location of executable
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set include directories
include_directories(source/
    source/ui/
    source/core/
    source/base/
    ${CMAKE_SOURCE_DIR}
)

# Make SFML available
include(FetchContent)
# Turn on verbose
set(FETCHCONTENT_QUIET OFF)

# Find Git executable
find_package(Git QUIET)
if(NOT Git_FOUND)
    message(FATAL_ERROR "Git not found! Required for SFML download.")
endif()

# Set SFML source path
set(SFML_SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/sfml-source")

# Only clone if not already exists
if(NOT EXISTS "${SFML_SOURCE_DIR}/CMakeLists.txt")
    # Shallow clone main repository (depth=1)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} clone --depth 1 --branch 3.0.x https://github.com/SFML/SFML.git ${SFML_SOURCE_DIR}
            RESULT_VARIABLE clone_result
    )

    # Shallow clone submodules
    if(clone_result EQUAL "0")
        execute_process(
                COMMAND ${GIT_EXECUTABLE} submodule update --init --depth 1
                WORKING_DIRECTORY ${SFML_SOURCE_DIR}
                RESULT_VARIABLE submodule_result
        )
    endif()

    # Verify clone success
    if(NOT clone_result EQUAL "0" OR NOT submodule_result EQUAL "0")
        message(FATAL_ERROR "Failed to clone SFML: Main:${clone_result} Submodules:${submodule_result}")
    endif()
endif()

# Configure SFML build
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_INSTALL OFF CACHE BOOL "" FORCE)

# Add to project
add_subdirectory(
        ${SFML_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}/_deps/sfml-build
        EXCLUDE_FROM_ALL
        SYSTEM
)

include_directories(${SFML_INCLUDE})

# Add source files to a list
set(
    SOURCES
    
    ./main.cpp
        ./common.h
        ./utility.h
        ./stopwatch.h
        ./Minesweeper.cpp
        ./Minesweeper.hpp

        source/base/Subject.hpp
        source/base/Observer.hpp
        source/base/Command.hpp
        source/base/State.hpp
        source/base/NumberGenerator.hpp

        source/core/Cell.hpp
        source/core/Controller.hpp
        source/core/Field.hpp
        source/core/Game.hpp

        source/ui/View.hpp
        #source/ui/Console/ConsoleView.hpp

        source/ui/Graphics/GraphicalView.hpp
        source/ui/Graphics/LayoutConfig.hpp
        source/ui/Graphics/Border.hpp
        source/ui/PlaceholderTexture.h
        source/ui/Graphics/Tile.hpp
        source/ui/Graphics/TextureManager.hpp
        source/ui/Graphics/Smiley.hpp
        source/ui/Graphics/Counter.hpp
        #source/ui/Graphics/Block.hpp

        source/base/Command.cpp
        source/base/State.cpp
        source/base/Replay.cpp
        source/base/NumberGenerator.cpp

        source/core/Cell.cpp
        source/core/Game.cpp
        source/core/Field.cpp
        source/core/Controller.cpp
    
        source/ui/Graphics/GraphicalView.cpp
        source/ui/Graphics/Border.cpp
        source/ui/Graphics/TextureManager.cpp
        source/ui/Graphics/Tile.cpp
        source/ui/Graphics/Smiley.cpp
        source/ui/Graphics/Counter.cpp
        source/ui/Graphics/DifficultySelectorDelegate.cpp
        source/ui/Graphics/DifficultySelectorDelegate.hpp
        #source/ui/Graphics/Block.cpp

        #source/ui/Console/ConsoleView.cpp
)

message("Build Type ${CMAKE_BUILD_TYPE}")

# Add the executable
add_executable(Minesweeper ${SOURCES})

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    # target_compile_options(Minesweeper PRIVATE "-fno-access-control")
    add_compile_options(-fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
endif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")

# Link with SFML
target_link_libraries(Minesweeper PRIVATE SFML::Graphics)

# Copy assets to binary directory
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})