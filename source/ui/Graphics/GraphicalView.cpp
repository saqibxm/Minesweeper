#include <thread>
#include <memory>
#include <chrono>

#include "Graphics/GraphicalView.hpp"
#include "Graphics/LayoutConfig.hpp"
#include "Controller.hpp"
#include "Graphics/DifficultySelectorDelegate.hpp"

using namespace mines;
using namespace std::string_literals;
using namespace LayoutConfig;

Graphics::Graphics(Controller &ctrl)
    : context(ctrl), border(textures), smiley(textures), flagCounter(textures), timeCounter(textures)
{
    using namespace LayoutConfig;

    smiley.OnClick([this] { context.NewGameRequested(); });
    
    window.setFramerateLimit(30);
    
    std::ignore = font.openFromFile("assets/Monocraft.ttf"); // TODO: remove hardcodes

    smiley.UpdateSize(SmileyWidth, SmileyHeight);

    message.setFillColor(sf::Color::White);
    message.setCharacterSize(14);

    data.setFillColor(sf::Color::White);
    data.setCharacterSize(12);

    difficultyLabel.setFillColor(sf::Color{200, 200, 200});
    difficultyLabel.setCharacterSize(11);

    flagCounter.UpdateSize(CounterWidth, CounterHeight);
    timeCounter.UpdateSize(CounterWidth, CounterHeight);

#ifndef NDEBUG
    debugInfo.setFillColor(sf::Color::Cyan);
    debugInfo.setOutlineColor(sf::Color::Black);
    debugInfo.setOutlineThickness(0.5f);
    debugInfo.setString("Info");
    debugInfo.setCharacterSize(12);
#endif /* NDEBUG */
}

void Graphics::Reset(const DifficultyConfig &cfg)
{
    auto [rows, cols] = context.ModelSize();
    auto [windowWidth, windowHeight] = window.getSize();
    auto [smileyWidth, smileyHeight] = smiley.RetrieveSize();

    // Header content area centre Y (between top border and middle divider)
    const float headerCenterY = static_cast<float>(BorderTop) + static_cast<float>(HeaderContentHeight) / 2.f;

#ifndef NDEBUG
    debugInfo.setPosition(sf::Vector2f(static_cast<float>(BorderLeft), static_cast<float>(TileHeight * rows + HeaderHeight)));
    data.setPosition(sf::Vector2f(15.0f + (windowWidth / 2.F), 5.0f + (TileHeight * rows + HeaderHeight)));
#else
    data.setPosition(sf::Vector2f(10.0f, 30.0F));
#endif

    message.setString("Minesweeper");
    data.setString("Data");

    // Centre the smiley horizontally; vertically centre it in the header content band
    smiley.UpdatePosition(
        (windowWidth / 2.f) - smileyWidth / 2.f,
        headerCenterY - smileyHeight / 2.f
    );

    // Flag counter: left side of header content, small margin from left border
    const float counterY = headerCenterY - flagCounter.RetrieveSize().y / 2.f;
    flagCounter.UpdatePosition(static_cast<float>(BorderLeft) + 6.f, counterY);

    // Time counter: symmetric on the right side
    const float contentRight = static_cast<float>(windowWidth - BorderRight);
    timeCounter.UpdatePosition(contentRight - timeCounter.RetrieveSize().x - 6.f, counterY);

    message.setPosition(sf::Vector2f(static_cast<float>(BorderLeft), static_cast<float>(BorderTop) + 4.f));

    // Difficulty label — centred below the smiley
    difficultyLabel.setString(DifficultyConfig::DiffToString(cfg.level).data());
    {
        auto lb = difficultyLabel.getLocalBounds();
        difficultyLabel.setPosition(sf::Vector2f{
            (windowWidth / 2.f) - lb.size.x / 2.f,
            headerCenterY + static_cast<float>(smileyHeight) / 2.f + 2.f
        });
    }

    mines = cfg.mines;
    flagCounter.SetNumber(mines);
}

DifficultyConfig Graphics::SelectDifficulty()
{
    return selector.PromptSelection();
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

    // Draw border chrome first (behind everything)
    window.draw(border);

    for(auto i = 0U; i < tiles.size(); ++i)
    {
        for(auto j = 0U; j < tiles[i].size(); ++j)
        {
            DrawCell(i, j);
        }
    }

    // window.draw(tiles);
    window.draw(timeCounter);
    window.draw(flagCounter);
    window.draw(message);
    window.draw(difficultyLabel);
    window.draw(data);
    window.draw(smiley);

#ifndef NDEBUG
    auto [x, y] = sf::Mouse::getPosition(window);

    // info_debug.setPosition({x+20.0f, y+20.0f});
    debugInfo.setString("row: " + std::to_string(y/LayoutConfig::TileWidth) + '\n' + "col: " + std::to_string(x/LayoutConfig::TileHeight));

    auto coords = CalculateCellCoord(x, y);

    if(coords) {
        int row = coords->first;
        int col = coords->second;

        const auto &cell = lastSnap.cells[row][col];
        debugInfo.setString(
            "row: " + std::to_string(row)
            + "\ncol: " + std::to_string(col)
            + '\n' + (cell.mine() ? "mine" : "clean") + '\t' + (cell.state() == Cell::HIDDEN ? "hidden" : "revealed")
            + '\n' + (cell.flagged() ? "flagged" : "unflagged")
            + "\nproximity: " + std::to_string(cell.proximity())
            + "\ntexture: " + tiles[row][col].tex
        );
    }
    else debugInfo.setString("out of range"s
        + "\nX: " + std::to_string(x)
        + "\nY: " + std::to_string(y));

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
            HandleClickReleased(event);
        }
        else if constexpr (std::is_same_v<Type, sf::Event::MouseButtonPressed>)
        {
            HandleClicked(event);
        }
        else if constexpr (std::is_same_v<Type, sf::Event::KeyPressed>)
        {
            HandleKeyPressed(event);
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
    Reset(snap.lvl);
    // assert(ctx == std::addressof(this->context)); // assert that its coming from the same context as held by this view

    for(decltype(tiles)::size_type ic = 0, ie = tiles.size(); ic < ie; ++ic)
    {
        for(decltype(tiles)::value_type::size_type jc = 0, je = tiles[ic].size(); jc < je; ++jc)
            RefreshTexture(ic, jc, snap.cells[ic][jc]);
            // RefreshTexture(ic, jc, snap.cells->operator()(ic, jc));
    }

    timeCounter.ResetNumber();
}

void Graphics::CellUpdate(Index row, Index col, const Cell& cell)
{
    RefreshTexture(row, col, cell);

#ifndef NDEBUG
    lastSnap.cells[row][col] = cell;
#endif // NDEBUG
}

void Graphics::CountersReceived(unsigned revealCount, unsigned flagCount)
{
    data.setString(
        "Revealed : " + std::to_string(revealCount)
        + '\n'
        + "Flagged: " + std::to_string(flagCount)
    );

    flagCounter.SetNumber(mines - flagCount);
}

void Graphics::TimeReceived(double seconds)
{
    timeCounter.SetNumber(seconds + 1);
}

void Graphics::ConfigUpdate(const DifficultyConfig &config)
{
    currentConfig = config;
    window.create(LayoutConfig::ComputeVideoMode({config.rows, config.cols}), LayoutConfig::Title, sf::Style::Close);

    border.Configure(config.rows, config.cols);

    const auto &cfg = config;
    tiles.clear();
    tiles.reserve(cfg.rows);

    decltype(tiles)::value_type vec;
    vec.reserve(cfg.cols);

    for(decltype(cfg.rows) i = 0; i < cfg.rows; ++i)
    {
        for(decltype(cfg.cols) j = 0; j < cfg.cols; ++j)
        {
            // Tiles are offset by the left border and the full header height
            vec.emplace_back(textures.PlaceholderPtr())
                .UpdatePosition(
                    static_cast<float>(BorderLeft  + CurrentTileWidth()  * j),
                    static_cast<float>(HeaderHeight + CurrentTileHeight() * i))
            .UpdateSize(CurrentTileWidth(), CurrentTileHeight());
        }
        tiles.push_back(std::move(vec));
    }

    Reset(config);
}

void Graphics::Won()
{
    message.setString("Won");
    smiley.Happy();
}

void Graphics::Lost(Index r, Index c) // the coordinates of the cell that exploded
{
    tiles[r][c].UpdateTexture(textures.FetchPtr("blast"));
    message.setString("Lost");

    smiley.Kill();
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
    // auto &tile = tiles.Cell(row, col);

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

    tile.UpdateTexture(textures.FetchPtr(texture));
}

void Graphics::HandleKeyPressed(const sf::Event::KeyPressed &key)
{
    using K = sf::Keyboard::Key;

#ifndef NDEBUG
    // Debug-only: + / - to increase / decrease tile size
    if (key.code == K::Equal || key.code == K::Add)
    {
        LayoutConfig::SetDebugTileSize(CurrentTileWidth() + 4, CurrentTileHeight() + 4);
        RelayoutGrid();
        return;
    }
    if (key.code == K::Hyphen || key.code == K::Subtract)
    {
        unsigned nw = CurrentTileWidth()  > 8 ? CurrentTileWidth()  - 4 : 8;
        unsigned nh = CurrentTileHeight() > 8 ? CurrentTileHeight() - 4 : 8;
        LayoutConfig::SetDebugTileSize(nw, nh);
        RelayoutGrid();
        return;
    }
#endif // NDEBUG

    // F2 or N: new game with same difficulty
    if (key.code == K::F2 || key.code == K::N)
    {
        context.NewGameRequested();
        smiley.Revive();
        return;
    }

    // 1 / 2 / 3: quick-switch to a preset difficulty (starts a new game)
    if (key.code == K::Num1) { context.NewGameRequested(DifficultyConfig::From(Difficulty::BEGINNER));     smiley.Revive(); return; }
    if (key.code == K::Num2) { context.NewGameRequested(DifficultyConfig::From(Difficulty::INTERMEDIATE)); smiley.Revive(); return; }
    if (key.code == K::Num3) { context.NewGameRequested(DifficultyConfig::From(Difficulty::EXPERT));       smiley.Revive(); return; }

    // 4 or D: open the full difficulty selector mid-game
    if (key.code == K::Num4 || key.code == K::D)
    {
        try
        {
            auto cfg = selector.PromptSelection();
            context.NewGameRequested(cfg);
            smiley.Revive();
        }
        catch (...) {} // user closed the dialog without choosing
    }
}

#ifndef NDEBUG
void Graphics::RelayoutGrid()
{
    // Resize the window to fit the new tile dimensions
    auto newMode = LayoutConfig::ComputeVideoMode({currentConfig.rows, currentConfig.cols});
    window.setSize(newMode.size);

    // Reposition and resize all tiles
    for (std::size_t i = 0; i < tiles.size(); ++i)
        for (std::size_t j = 0; j < tiles[i].size(); ++j)
            tiles[i][j]
                .UpdatePosition(
                    static_cast<float>(BorderLeft  + CurrentTileWidth()  * j),
                    static_cast<float>(HeaderHeight + CurrentTileHeight() * i))
                .UpdateSize(CurrentTileWidth(), CurrentTileHeight());

    Reset(currentConfig);
}
#endif // NDEBUG

void Graphics::HandleClickReleased(const sf::Event::MouseButtonReleased &mouse)
{
    const auto [x, y] = mouse.position;
    auto coords = CalculateCellCoord(x, y);

    smiley.Released(mouse.position);
    if (!coords) return;

    auto [row, col] = *coords;

    if(mouse.button == sf::Mouse::Button::Left)
        context.RevealRequested(row, col);
    else if(mouse.button == sf::Mouse::Button::Right)
        context.FlagRequested(row, col);
}

void Graphics::HandleClicked(const sf::Event::MouseButtonPressed &mouse)
{
    if(!smiley.BoundingPos().contains(sf::Vector2f(mouse.position)))
        smiley.Clicked(mouse.position);
    else
        smiley.Clicked(mouse.position);
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
    auto [w, h] = std::make_pair(LayoutConfig::TileWidth, LayoutConfig::TileHeight);
    
    if((x >= fx && x < lx + w) && (y >= fy && y < ly + h))
    {
        Index row = (y - fy) / h;
        Index col = (x - fx) / w;
        return UPair<Index>{row, col};
    }
    return std::nullopt;
}
