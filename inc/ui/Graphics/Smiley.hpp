#pragma once

#include <functional>

#include "Tile.hpp"
#include "TextureManager.hpp"

namespace mines
{

class Smiley : public Tile
{
    using ClickCallback = std::function<void(void)>;
    enum Face { SMILE, PRESSED, SURPRISED, HAPPY, DEAD };
public:
    Smiley(TextureManager &textureStore, ClickCallback = []{});

    void draw(sf::RenderTarget&, sf::RenderStates) const override;
    
    void OnClick(ClickCallback);
    Smiley& Clicked(sf::Vector2i pos);
    Smiley& Released(sf::Vector2i pos);
    Smiley& React(); // react when cells are clicked
    Smiley& Happy();
    Smiley& Kill(); // X(
    Smiley& Revive();
    
private:
    TextureManager &manager;
    ClickCallback callback;
    Face face;
    bool pressed = false;

    void UpdateFace(Face f);
};

}