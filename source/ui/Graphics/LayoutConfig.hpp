#pragma once

#include <SFML/Graphics.hpp>


#include "common.h"
#include "utility"

namespace mines
{
    namespace LayoutConfig
    {
        constexpr inline unsigned TileWidth = 32; // pixels
        constexpr inline unsigned TileHeight = 32; // pixels
        [[maybe_unused]] constexpr inline float TileOutline = 1.5f;

        constexpr inline unsigned SmileyWidth = 52, SmileyHeight = 52;

        constexpr inline unsigned CounterWidth = 26, CounterHeight = 50; // subject to change

        constexpr inline unsigned HeaderHeight = 100;
        constexpr inline unsigned FooterHeight = debug ? 80 : 0;

        constexpr const char Title[] = "Minesweeper";

        inline sf::VideoMode ComputeVideoMode(UPair<Index> size)
        {
            return sf::VideoMode(sf::Vector2u(size.second * TileWidth, (size.first * TileHeight) + HeaderHeight + FooterHeight));
        }
    }
}