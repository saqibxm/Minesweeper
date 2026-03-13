#pragma once

#include <vector>
#include <optional>

#include <SFML/Graphics.hpp>

#include "TextureManager.hpp"
#include "Border.hpp"
#include "View.hpp"
#include "Game.hpp"
#include "Tile.hpp"
#include "Smiley.hpp"
#include "Counter.hpp"
#include "Graphics/DifficultySelectorDelegate.hpp"

namespace mines
{
    class Graphics : public IView
    {
    public:
        Graphics() = default;
        Graphics(Controller &ctrl);

        void Reset(const DifficultyConfig&);
        DifficultyConfig SelectDifficulty() override;
        void ShowMessage(const std::string &msg) override;
        void Display() override;
        bool ShouldClose() const override;

        void Update(const BoardSnapshot &snap) override;
        void CellUpdate(Index r, Index c, const Cell&) override;
        void CountersReceived(unsigned rc, unsigned fc) override;
        void TimeReceived(double) override;
        void ConfigUpdate(const DifficultyConfig &) override;
        void Won() override;
        void Lost(Index r, Index c) override;
    
    private:
        sf::RenderWindow window;
        std::vector<std::vector<Tile>> tiles;

        Controller &context;
        TextureManager textures;

        Border border;
        Smiley smiley;
        Counter flagCounter;
        Counter timeCounter;

        sf::Font font;
        sf::Text message{font};
        sf::Text data{font};
        sf::Text difficultyLabel{font};  // shows current difficulty in the header

        unsigned mines = 0;
        DifficultyConfig currentConfig;  // last applied config (for mid-game changes)

        impl::DifficultySelectorDelegate selector{font};  // reusable selector

        void HandleClicked(const sf::Event::MouseButtonPressed&);
        void HandleClickReleased(const sf::Event::MouseButtonReleased&);
        void HandleKeyPressed(const sf::Event::KeyPressed&);
        void DrawCell(Index r, Index c);
        void RefreshTexture(Index r, Index c, const Cell&);
        Tile* TileAt(float x, float y);
        std::optional<UPair<Index>> CalculateCellCoord(float x, float y) const;
    
#ifndef NDEBUG
    sf::Text debugInfo{font};
    BoardSnapshot lastSnap;
#endif /* NDEBUG */
    };
}