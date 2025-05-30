#pragma once

#include <SFML/Graphics.hpp>

#include "utility.h"
#include "Cell.hpp"
// #include "TextureManager.hpp"

namespace mines
{
    class Tile : public sf::Drawable
    {
    public:
        constexpr static unsigned width = 20, height = 20; // n pixels each

        // Tile(const sf::Texture &init);

        Tile(sf::Texture *init);
        
        void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

        // Tile& UpdateTexture(const sf::Texture &tex);

        Tile& UpdateTexture(sf::Texture *const);

        Tile& UpdatePosition(float x, float y);

        sf::Vector2f RetrievePosition() const;

        Tile& UpdateSize(float w, float h);

        sf::Vector2f RetrieveSize() const;

        sf::Rect<float> BoundingPos() const;

    private:
        sf::RectangleShape tile;
        // sf::Sprite tile;
        Cell *cell; // unused
        // bool hovered;
    #ifndef NDEBUG
        std::string tex;
        friend class Graphics;
    #endif /* NDEBUG */
    };
}