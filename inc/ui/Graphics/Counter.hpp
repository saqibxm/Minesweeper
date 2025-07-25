#pragma once

#include <array>
#include <SFML/Graphics/Drawable.hpp>

#include "TextureManager.hpp"

namespace mines {

class Counter : public sf::Drawable
{
    enum Face : int { MINUS = -1, ZERO = 0, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, NONE };
public:
    Counter(TextureManager&, int val = 0);

    void draw(sf::RenderTarget&, sf::RenderStates) const override;

    void SetNumber(int val);
    int GetNumber() const;
    void ResetNumber();

    void UpdateSize(float w, float h);
    void UpdatePosition(float x, float y);
    const sf::Vector2f& RetrievePosition() const;
    sf::Vector2f RetrieveSize() const;

    Counter& operator++();
    Counter& operator--();

private:
    std::array<sf::RectangleShape, 3> digits;
    TextureManager &manager;
    int value;

    sf::Sprite left, frame, right;

    sf::Vector2f position, size;

    void UpdateAllFaces();
    void UpdateFace(std::size_t index, Face face);
};

}