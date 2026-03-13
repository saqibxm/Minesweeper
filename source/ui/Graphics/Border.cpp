#include "Graphics/Border.hpp"
#include "Graphics/LayoutConfig.hpp"

using namespace mines;
using namespace LayoutConfig;

Border::Border(TextureManager &mgr) : manager(mgr)
{
    // Mark border textures as repeatable so SFML can tile them.
    manager.Fetch("top").setRepeated(true);
    manager.Fetch("bottom").setRepeated(true);
    manager.Fetch("left").setRepeated(true);
    manager.Fetch("right").setRepeated(true);
}

void Border::Configure(unsigned rows, unsigned cols)
{
    gridRows = rows;
    gridCols = cols;
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
    const float gW  = static_cast<float>(gridCols * TileWidth);
    const float gH  = static_cast<float>(gridRows * TileHeight);

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
    {
        auto &tex = const_cast<Border*>(this)->manager.Fetch("top");
        target.draw(MakeTiled(tex, lW, 0.f, gW, tH), states);
    }

    // ─── Middle horizontal strip (divider between header and grid) ───────────
    {
        auto &tex = const_cast<Border*>(this)->manager.Fetch("top");
        // Scale vertically so it fills the middle divider height
        sf::RectangleShape shape({gW, mH});
        shape.setPosition({lW, midY});
        shape.setTexture(&tex);
        // Tile horizontally, stretch vertically by repeating original top texture rect
        shape.setTextureRect({{0, 0}, {static_cast<int>(gW), static_cast<int>(tH)}});
        target.draw(shape, states);
    }

    // ─── Bottom horizontal strip ──────────────────────────────────────────────
    {
        auto &tex = const_cast<Border*>(this)->manager.Fetch("bottom");
        target.draw(MakeTiled(tex, lW, botY, gW, bH), states);
    }

    // ─── Left vertical strip (header + grid combined) ────────────────────────
    {
        auto &tex = const_cast<Border*>(this)->manager.Fetch("left");
        // Header content section
        target.draw(MakeTiled(tex, 0.f, tH, lW, hCH), states);
        // Grid section
        target.draw(MakeTiled(tex, 0.f, gridY, lW, gH), states);
    }

    // ─── Right vertical strip ─────────────────────────────────────────────────
    {
        auto &tex = const_cast<Border*>(this)->manager.Fetch("right");
        // Header content section
        target.draw(MakeTiled(tex, lW + gW, tH, rW, hCH), states);
        // Grid section
        target.draw(MakeTiled(tex, lW + gW, gridY, rW, gH), states);
    }
}
