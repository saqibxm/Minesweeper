#include "Graphics/Border.hpp"
#include "Graphics/LayoutConfig.hpp"

using namespace mines;
using namespace LayoutConfig;

Border::Border(TextureManager &mgr) : manager(mgr)
{
    // Textures are marked as repeatable when Configure() first fetches them.
}

void Border::Configure(unsigned rows, unsigned cols)
{
    gridRows = rows;
    gridCols = cols;

    // Cache mutable texture pointers and enable tiling
    texTop    = &manager.Fetch("top");    texTop->setRepeated(true);
    texBottom = &manager.Fetch("bottom"); texBottom->setRepeated(true);
    texLeft   = &manager.Fetch("left");   texLeft->setRepeated(true);
    texRight  = &manager.Fetch("right");  texRight->setRepeated(true);
}

sf::RectangleShape Border::MakeTiled(sf::Texture &tex,
                                     float x, float y,
                                     float w, float h)
{
    sf::RectangleShape shape({w, h});
    shape.setPosition({x, y});
    shape.setTexture(&tex);
    shape.setTextureRect({{0, 0}, {static_cast<int>(w), static_cast<int>(h)}});
    return shape;
}

sf::Sprite Border::MakeSprite(const sf::Texture &tex, float x, float y)
{
    sf::Sprite s(tex);
    s.setPosition({x, y});
    return s;
}

void Border::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    const float lW  = static_cast<float>(BorderLeft);
    const float rW  = static_cast<float>(BorderRight);
    const float tH  = static_cast<float>(BorderTop);
    const float mH  = static_cast<float>(BorderMiddle);
    const float bH  = static_cast<float>(BorderBottom);
    const float hCH = static_cast<float>(HeaderContentHeight);
    const float gW  = static_cast<float>(gridCols * CurrentTileWidth());
    const float gH  = static_cast<float>(gridRows * CurrentTileHeight());

    // Derived positions
    const float midY    = tH + hCH;           // y of middle divider
    const float gridY   = static_cast<float>(HeaderHeight);
    const float botY    = gridY + gH;

    // ─── Corners ─────────────────────────────────────────────────────────────
    target.draw(MakeSprite(manager.Fetch("topleft"),     0.f,       0.f),     states);
    target.draw(MakeSprite(manager.Fetch("topright"),    lW + gW,   0.f),     states);
    target.draw(MakeSprite(manager.Fetch("middleleft"),  0.f,       midY),    states);
    target.draw(MakeSprite(manager.Fetch("middleright"), lW + gW,   midY),    states);
    target.draw(MakeSprite(manager.Fetch("bottomleft"),  0.f,       botY),    states);
    target.draw(MakeSprite(manager.Fetch("bottomright"), lW + gW,   botY),    states);

    // ─── Top horizontal strip (top border row) ────────────────────────────────
    if (texTop)
        target.draw(MakeTiled(*texTop, lW, 0.f, gW, tH), states);

    // ─── Middle horizontal strip (divider between header and grid) ───────────
    if (texTop)
    {
        sf::RectangleShape shape({gW, mH});
        shape.setPosition({lW, midY});
        shape.setTexture(texTop);
        shape.setTextureRect({{0, 0}, {static_cast<int>(gW), static_cast<int>(tH)}});
        target.draw(shape, states);
    }

    // ─── Bottom horizontal strip ──────────────────────────────────────────────
    if (texBottom)
        target.draw(MakeTiled(*texBottom, lW, botY, gW, bH), states);

    // ─── Left vertical strip (header + grid combined) ────────────────────────
    if (texLeft)
    {
        target.draw(MakeTiled(*texLeft, 0.f, tH, lW, hCH), states);
        target.draw(MakeTiled(*texLeft, 0.f, gridY, lW, gH), states);
    }

    // ─── Right vertical strip ─────────────────────────────────────────────────
    if (texRight)
    {
        target.draw(MakeTiled(*texRight, lW + gW, tH, rW, hCH), states);
        target.draw(MakeTiled(*texRight, lW + gW, gridY, rW, gH), states);
    }
}
