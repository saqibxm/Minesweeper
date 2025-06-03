#include "Graphics/Tile.hpp"
#include "Graphics/LayoutConfig.hpp"

using namespace mines;
using namespace mines::DisplayConfig;

Tile::Tile(sf::Texture *init)
    : tile()
{
    UpdateTexture(init);
    UpdateSize(TileWidth, TileHeight);
}
        
void Tile::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    target.draw(tile, state);
}

/*
Tile& Tile::UpdateTexture(const sf::Texture &tex)
{
    tile.setTexture(tex);
    return *this;
}
*/

Tile& Tile::UpdateTexture(sf::Texture *const tex)
{
    tile.setTexture(tex, true); // wtf is the second parameter
    return *this;
}

Tile& Tile::UpdatePosition(float x, float y)
{
    tile.setPosition({x, y});
    return *this;
}

sf::Vector2f Tile::RetrievePosition() const {
    return tile.getPosition();
}

Tile& Tile::UpdateSize(float w, float h)
{
    /*
    sf::Vector2u textureSize = tile.getTexture().getSize();
    
    // Calculate scale factors
    float scaleX = DisplayConfig::TileWidth / static_cast<float>(textureSize.x);
    float scaleY = DisplayConfig::TileWidth / static_cast<float>(textureSize.y);
    
    // Apply scale
    tile.setScale({scaleX, scaleY});
    */

    tile.setSize({w, h});

    return *this;
}
sf::Vector2f Tile::RetrieveSize() const {
    return {DisplayConfig::TileWidth, DisplayConfig::TileHeight};
}
sf::Rect<float> Tile::BoundingPos() const {
    return tile.getGlobalBounds();

    auto [x, y] = tile.getPosition();
    auto [w, h] = RetrieveSize();
    return sf::Rect<float>{{x, y}, {x + w, y + h}};
}