#pragma once

#include <SFML/Graphics.hpp>

#include "TextureManager.hpp"
#include "common.h"
#include "utility.h"

namespace mines
{
    /**
     * Renders the classic Minesweeper border chrome around the game window.
     *
     * Layout (Y positions from top of window):
     *   [0  .. BorderTop)             → top border row  (topleft, top×N, topright)
     *   [BorderTop .. BorderTop+HeaderContentHeight) → header sides (left, right)
     *   [BorderTop+HeaderContentHeight .. HeaderHeight) → middle divider (middleleft, top×N, middleright)
     *   [HeaderHeight .. HeaderHeight+rows*TileHeight) → grid sides (left×M, right×M)
     *   [HeaderHeight+rows*TileHeight .. window bottom) → bottom border (bottomleft, bottom×N, bottomright)
     *
     * X positions:
     *   [0  .. BorderLeft)                       → left column
     *   [BorderLeft .. BorderLeft+cols*TileWidth) → content column
     *   [BorderLeft+cols*TileWidth .. window width) → right column
     */
    class Border : public sf::Drawable
    {
    public:
        explicit Border(TextureManager &mgr);

        /// Call whenever the grid size changes.
        void Configure(unsigned rows, unsigned cols);

        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        TextureManager &manager;
        unsigned gridRows = 0;
        unsigned gridCols = 0;

        // Helper: build a RectangleShape that tiles a given texture over a given rect.
        static sf::RectangleShape MakeTiled(sf::Texture &tex,
                                            float x, float y,
                                            float w, float h);

        // Helper: build a scaled Sprite (no tiling needed for corner / end pieces).
        static sf::Sprite MakeSprite(const sf::Texture &tex, float x, float y);
    };
}
