#include <thread>
#include <memory>
#include <chrono>

#include "Graphics/GraphicalView.hpp"
#include "Graphics/DisplayConfig.hpp"
#include "Controller.hpp"

using namespace mines;

Graphics::Graphics(Controller &ctrl)
    : context(ctrl)
{
    using namespace DisplayConfig;

    auto [rows, cols] = context.ModelSize();
    /*
    // tiles.assign(rows, decltype(tiles)::value_type(cols));
    tiles.reserve(rows);
    
    decltype(tiles)::value_type vec;
    vec.reserve(cols);
    
    for(decltype(rows) i = 0; i < rows; ++i)
    {
        for(decltype(cols) j = 0; j < cols; ++j)
        {
            vec.emplace_back(texman.PlaceholderPtr())
            .UpdatePosition(TileWidth * j, (TileHeight * i) + HeaderHeight);
            // RefreshTexture(i, j); // segfault, for obvious reasons
        }
        tiles.push_back(std::move(vec));
    }
    */
    
    window.setFramerateLimit(30);
    
    std::ignore = font.openFromFile("assets/Monocraft.ttf"); // TODO: remove hardcodes

    message.setFillColor(sf::Color::White);
    message.setPosition(sf::Vector2f(10.0f, 10.0f));
    message.setString("Minesweeper");
    message.setCharacterSize(14);

    data.setFillColor(sf::Color::White);
    data.setPosition(sf::Vector2f(10.0f, 30.0f));
    data.setString("Data");
    data.setCharacterSize(12);

#ifndef NDEBUG
    debugInfo.setPosition(sf::Vector2f(10.f, TileHeight * rows + HeaderHeight));
    debugInfo.setFillColor(sf::Color::Cyan);
    debugInfo.setOutlineColor(sf::Color::Black);
    debugInfo.setOutlineThickness(0.5f);
    debugInfo.setString("Info");
    debugInfo.setCharacterSize(12);
#endif /* NDEBUG */
}

void Graphics::Reset(const BoardSnapshot &snap)
{
    using namespace DisplayConfig;
    window.create(DisplayConfig::ComputeVideoMode({snap.rows, snap.cols}), DisplayConfig::Title, sf::Style::Close);

    tiles.clear();
    tiles.reserve(snap.rows);
    
    decltype(tiles)::value_type vec;
    vec.reserve(snap.cols);

    for(decltype(snap.rows) i = 0; i < snap.rows; ++i)
    {
        for(decltype(snap.cols) j = 0; j < snap.cols; ++j)
        {
            vec.emplace_back(texman.PlaceholderPtr())
                .UpdatePosition(TileWidth * j, (TileHeight * i) + HeaderHeight);
            // RefreshTexture(i, j); // segfault, for obvious reasons
        }
        tiles.push_back(std::move(vec));
    }
}

DifficultyConfig Graphics::SelectDifficulty()
{
    return {/* NOT IMPLEMENTED */};
}

void Graphics::ShowMessage(const std::string &msg)
{
    using namespace std::chrono_literals;

    static std::thread worker;
    static auto timeout = 0.5s;

    if(worker.joinable()) worker.join();

    auto original = message.getString();
    worker = std::thread(
        [&] {
            message.setString(msg);
            std::this_thread::sleep_for(timeout);
            message.setString(original);
        }
    );
}

void Graphics::Display()
{
    if(ShouldClose()) return;
    window.clear();

    for(auto i = 0U; i < tiles.size(); ++i)
    {
        for(auto j = 0U; j < tiles[i].size(); ++j)
        {
            DrawCell(i, j);
        }
    }

    window.draw(message);
    window.draw(data);

    
#ifndef NDEBUG
    auto [x, y] = sf::Mouse::getPosition(window);
    
    auto [fx, fy] = tiles.front().front().RetrievePosition();
    auto [lx, ly] = tiles.back().back().RetrievePosition();
    auto [w, h] = std::make_pair(DisplayConfig::TileWidth, DisplayConfig::TileHeight);
    // info_debug.setPosition({x+20.0f, y+20.0f});
    debugInfo.setString("row: " + std::to_string(y/DisplayConfig::TileWidth) + "\ncol: " + std::to_string(x/DisplayConfig::TileHeight));
    
    if((x >= fx && x < lx + w) && (y >= fy && y < ly + h)) {
        int row = (y - fy) / h;
        int col = (x - fx) / w;
        const auto &cell = lastSnap.cells[row][col];
        debugInfo.setString(
            "row: " + std::to_string(row)
            + "\ncol: " + std::to_string(col)
            + '\n' + (cell.mine() ? "mine" : "clean")
            + '\n' + (cell.flagged() ? "flagged" : "unflagged")
            + "\nproximity: " + std::to_string(cell.proximity())
            + "\ntexture: " + tiles[row][col].tex
        );
    }
    else debugInfo.setString("out of range");

    window.draw(debugInfo);
#endif /* NDEBUG */

    /*window.handleEvents([this](const sf::Event::Closed &e) -> void {
        window.close();
    });*/

    // window.handleEvents(std::bind(&Graphics::HandleCellClicked, this, std::placeholders::_1));
    window.handleEvents([this](const auto &event) -> void {
        using Type = std::decay_t<decltype(event)>;
        if constexpr (std::is_same_v<Type, sf::Event::Closed>)
        {
            window.close();
        }
        else if constexpr (std::is_same_v<Type, sf::Event::MouseButtonReleased>)
        {
            HandleCellClicked(event);
        }
    });
    
    window.display();
}

bool Graphics::ShouldClose() const
{
    return !window.isOpen();
}

void Graphics::Update(const BoardSnapshot &snap)
{

#ifndef NDEBUG
    lastSnap = snap;
#endif // NDEBUG
    Reset(snap);
    // assert(ctx == std::addressof(this->context)); // assert that its coming from the same context as held by this view
    
    for(decltype(tiles)::size_type ic = 0, ie = tiles.size(); ic < ie; ++ic)
    {
        for(decltype(tiles)::value_type::size_type jc = 0, je = tiles.size(); jc < je; ++jc)
            RefreshTexture(ic, jc, snap.cells[ic][jc]);
    }
}

void Graphics::CellUpdate(Index row, Index col, const Cell& cell)
{
    RefreshTexture(row, col, cell);
}

void Graphics::DataReceived(unsigned revealCount, unsigned flagCount)
{
    data.setString(
        "Revealed : " + std::to_string(revealCount)
        + "\nFlagged: " + std::to_string(flagCount)
    );
}

void Graphics::Ended()
{
}

void Graphics::Lost(Index r, Index c) // the coordinates of the cell that exploded
{
    tiles[r][c].UpdateTexture(texman.FetchPtr("blast"));
    message.setString("Game Lost!");
}

void Graphics::DrawCell(Index row, Index col)
{
    auto &cell = tiles[row][col];
    window.draw(cell);
}

void Graphics::RefreshTexture(Index row, Index col, const Cell& cell)
{
    // const auto &cell = context.Board().CellAt(row, col); // get the underlying (model's) cell for interogation
    auto &tile = tiles[row][col];

    std::string texture;

    switch (cell.state())
    {
    case Cell::HIDDEN:
        if (debug && cell.mine())
            texture = "debug";
        else
            texture = "cellup";
        break;
    case Cell::FLAGGED:
        texture = "cellflag";
        break;
    case Cell::CLEARED:
    {
        if(cell.mine())
        {
            texture = "cellmine";
        }
        else
        {
            if(auto proximity = cell.proximity())
            texture = "cell" + std::to_string(proximity);
            else texture = "celldown";
            
            if(cell.flagged()) texture = "falsemine"; // if cell was flagged but it didnt have mine
        }
    } break;
    default:
        break;
    }

#ifndef NDEBUG
    tile.tex = texture;
#endif // NDEBUG

    tile.UpdateTexture(texman.FetchPtr(texture));
}

void Graphics::HandleCellClicked(const sf::Event::MouseButtonReleased &mouse)
{
    const auto [x, y] = mouse.position;
    auto coords = CalculateCellCoord(x, y);

    if(!coords) return;

    auto [row, col] = *coords;

    if(mouse.button == sf::Mouse::Button::Left)
        context.HandleCommand(std::make_unique<RevealCommand>(row, col));
    else if(mouse.button == sf::Mouse::Button::Right)
        context.HandleCommand(std::make_unique<FlagCommand>(row, col));
}

Tile* Graphics::TileAt(float x, float y)
{
    if(auto coords = CalculateCellCoord(x, y))
    {
        return &tiles[coords->first][coords->second];
    }
    return nullptr;
}

std::optional<UPair<Index>> Graphics::CalculateCellCoord(float x, float y) const
{
    auto [fx, fy] = tiles.front().front().RetrievePosition();
    auto [lx, ly] = tiles.back().back().RetrievePosition();
    auto [w, h] = std::make_pair(DisplayConfig::TileWidth, DisplayConfig::TileHeight);
    
    if((x >= fx && x < lx + w) && (y >= fy && y < ly + h))
    {
        Index row = (y - fy) / h;
        Index col = (x - fx) / w;
        return UPair{row, col};
    }
    return std::nullopt;
}
