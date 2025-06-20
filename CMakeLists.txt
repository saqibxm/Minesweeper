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
# Turn on verbose
set(FETCHCONTENT_QUIET OFF)

# Find Git executable
find_package(Git QUIET)
if(NOT Git_FOUND)
    message(FATAL_ERROR "Git not found! Required for SFML download.")
endif()

# Set SFML source path
set(SFML_SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/sfml-src")

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
        src/ui/Graphics/DifficultySelectorDelegate.cpp
        inc/ui/Graphics/DifficultySelectorDelegate.hpp
        #src/ui/Graphics/Block.cpp

        #src/ui/Console/ConsoleView.cpp
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