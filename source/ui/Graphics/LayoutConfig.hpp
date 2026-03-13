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

        // Border sprite dimensions (derived from asset sizes)
        constexpr inline unsigned BorderLeft   = 30; // topleft/left/middleleft/bottomleft width
        constexpr inline unsigned BorderRight  = 23; // topright/right/middleright/bottomright width
        constexpr inline unsigned BorderTop    = 30; // top edge height
        constexpr inline unsigned BorderMiddle = 38; // horizontal divider between header and grid
        constexpr inline unsigned BorderBottom = 23; // bottom edge height

        // Height of the content region between top border and middle divider
        constexpr inline unsigned HeaderContentHeight = 68; // matches counterleft/right border sprite height

        // Total header height = top border + header content + middle divider
        constexpr inline unsigned HeaderHeight = BorderTop + HeaderContentHeight + BorderMiddle; // 136 px

        constexpr inline unsigned FooterHeight = debug ? 80 : 0;

        constexpr const char Title[] = "Minesweeper";

#ifndef NDEBUG
        // In debug builds these are runtime-mutable so the user can resize tiles with +/-
        inline unsigned currentTileW = TileWidth;
        inline unsigned currentTileH = TileHeight;
        inline unsigned CurrentTileWidth()  noexcept { return currentTileW; }
        inline unsigned CurrentTileHeight() noexcept { return currentTileH; }
        inline void SetDebugTileSize(unsigned w, unsigned h) noexcept
        {
            currentTileW = w > 0 ? w : 1;
            currentTileH = h > 0 ? h : 1;
        }
#else
        constexpr inline unsigned CurrentTileWidth()  noexcept { return TileWidth;  }
        constexpr inline unsigned CurrentTileHeight() noexcept { return TileHeight; }
#endif

        inline sf::VideoMode ComputeVideoMode(UPair<Index> size)
        {
            return sf::VideoMode(sf::Vector2u(
                BorderLeft + size.second * CurrentTileWidth() + BorderRight,
                HeaderHeight + size.first * CurrentTileHeight() + BorderBottom + FooterHeight
            ));
        }
    }
}