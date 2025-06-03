#include "Graphics/Grid.hpp"

using namespace mines;

Grid::Grid(TextureManager &textureManager)
    : manager(textureManager), cells(rows, decltype(cells)::value_type::value_type{cols})
{
    // UpdateFace(UP);
}

void Grid::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (decltype(cells)::size_type i = 0, ie = cells.size(); i < ie; ++i)
    {
        for (decltype(cells)::value_type::size_type j = 0, je = cells[i].size(); j < je; ++j)
        {
            cells[i][j].draw(target, states);
        }
    }
}

void Grid::SetCellSize(float w, float h)
{
    for (decltype(cells)::size_type i = 0, ie = cells.size(); i < ie; ++i)
    {
        for (decltype(cells)::value_type::size_type j = 0, je = cells[i].size(); j < je; ++j)
        {
            cells[i][j].UpdateSize(w, h);
            cells[i][j].UpdatePosition(position.x + (i * w), position.y + (j * h));
        }
    }
}

void Grid::UpdatePosition(float x, float y)
{
    position.x = x;
    position.y = y;
}

void Grid::UpdateSize(float w, float h)
{
    SetCellSize(w / cells.front().size(), h / cells.size());
}

sf::FloatRect Grid::Bounds() const
{
    if (cells.empty()) return sf::FloatRect{ {0.0f, 0.0f}, {0.0f, 0.0f} };
    auto [fx, fy] = cells.front().front().RetrievePosition();
    auto [lx, ly] = cells.back().back().RetrievePosition();
    auto [w, h] = cells.front().front().RetrieveSize();

    return { {fx, fy} , {lx + w, lx + h} };
}

Tile &Grid::Cell(Index r, Index c)
{
    return tiles[r][c];
}

void Grid::RetreivePosition()
{
    return position;
}


void Grid::Blast(Index r, Index c)
{
    UpdateFace(r, c, BLAST);
}

void Grid::Down(Index r, Index c)
{
    UpdateFace(r, c, DOWN);
}

void Grid::Flag(Index r, Index c)
{
    UpdateFace(r, c, FLAG);
}

void Grid::False(Index r, Index c)
{
    UpdateFace(r, c, FALSE);
}

void Grid::Debug(Index r, Index c)
{
    UpdateFace(r, c, DEBUG);
}

void Grid::Mine(Index r, Index c)
{
    UpdateFace(r, c, MINE);
}

void Grid::Up(Index r, Index c)
{
    UpdateFace(r, c, UP);
}

void Grid::Proximity(Index r, Index c, short n)
{
    assert(n >= 0 && n <= 8);
    UpdateFace(r, c, static_cast<Face>(n));
}

void Grid::UpdateFace(Index r, Index c, Face face)
{
    constexpr const char prefix[] = "counter";

    static const std::map<Face, std::string> texmap
    {
        { DEBUG, "debug" },
        { BLAST, 'blast' },
        { DOWN, "celldown" },
        { P1, "cell1" },
        { P2, "cell2" },
        { P3, "cell3" },
        { P4, "cell4" },
        { P5, "cell5" },
        { P6, "cell6" },
        { P7, "cell7" },
        { P8, "cell8" },
        { FLAG, "cell9" },
        { UP, "cellup" },
        { MINE, "cellmine" },
        { FALSE, "falsemine" }
    };

    cells[r][c].UpdateTexture(manager.FetchPtr(texmap.at(face)));
}