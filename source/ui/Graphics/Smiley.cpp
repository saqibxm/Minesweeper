#include "Graphics/Smiley.hpp"

using namespace mines;

Smiley::Smiley(TextureManager &texman, ClickCallback cb)
    : Tile(nullptr), manager(texman), callback(std::move(cb))
{
    // Tile::UpdateTexture(manager.PlaceholderPtr());
    UpdateFace(SMILE);
}

void Smiley::draw(sf::RenderTarget &target, sf::RenderStates state) const
{
    Tile::draw(target, state);
}

void Smiley::OnClick(ClickCallback cb)
{
    callback = std::move(cb);
}

Smiley& Smiley::Clicked(sf::Vector2i pos)
{
    if(BoundingPos().contains(sf::Vector2f(pos)))
    {
        UpdateFace(PRESSED);
        pressed = true;
    }
    else
        React();

    return *this;
}

Smiley& Smiley::Released(sf::Vector2i pos)
{
    if (face == DEAD || face == HAPPY) return *this;

    UpdateFace(SMILE);
    if (pressed)
    {
        pressed = false;
        callback();
    }

    return *this;
}

Smiley& Smiley::React()
{
    if(face != DEAD && face != HAPPY)
    {
        UpdateFace(SURPRISED);
    }

    return *this;
}

Smiley& Smiley::Happy()
{
    // if(face != DEAD) return *this;

    UpdateFace(HAPPY);

    return *this;
}

Smiley& Smiley::Kill()
{
    UpdateFace(DEAD);

    return *this;
}

Smiley& Smiley::Revive()
{
    UpdateFace(SMILE);

    return *this;
}

void Smiley::UpdateFace(Face f)
{
    static const std::map<Face, std::string> texmap
    {
        { SMILE, "smileface" },
        { PRESSED, "smilefacedown" },
        { SURPRISED, "clickface" },
        { HAPPY, "winface" },
        { DEAD, "lostface" }
    };

    face = f;
    UpdateTexture(manager.FetchPtr(texmap.at(face)));
}
