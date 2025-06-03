#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "Tile.hpp"
#include "TextureManager.hpp"

namespace mines {

class Grid : public sf::Drawable
{
public:
    enum Face { DEBUG, BLAST, DOWN = 0, P1 = 1, P2, P3, P4, P5, P6, P7, P8, FLAG, MINE, UP, FALSE };

    Grid(TextureManager& textureManager, std::size_t rows, std::size_t cols);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void SetCellSize(float w, float h);
    void UpdatePosition(float x, float y);
    void RetreivePosition();
    void UpdateSize(float w, float h);

    void Blast(Index r, Index c);
    void Down(Index r, Index c);
    void Up(Index r, Index c);
    void Flag(Index r, Index c);
    void False(Index r, Index c);
    void Mine(Index r, Index c);
    void Debug(Index r, Index c); // only active during debugging
    void Proximity(Index r, Index c, short n);
    Tile& Cell(Index r, Index c);
    sf::FloatRect Bounds() const;

private:
    TextureManager& manager;
    std::vector<std::vector<Tile>> cells;
    sf::Vector2f position;

    void UpdateFace(Index r, Index c, Face face);
};

}