#pragma once

#include <vector>
#include <optional>

#include <SFML/Graphics.hpp>

#include "View.hpp"
#include "Game.hpp"
#include "Tile.hpp"
#include "TextureManager.hpp"
#include "Smiley.hpp"

namespace mines
{
    class Graphics : public IView
    {
    public:
        Graphics() = default;
        Graphics(Controller &ctrl);
        // ~Graphics();

        void Reset(const BoardSnapshot &snap);
        DifficultyConfig SelectDifficulty() override;
        void ShowMessage(const std::string &msg) override;
        void Display() override;
        bool ShouldClose() const override; // if the view should close

        void Update(const BoardSnapshot &snap) override;
        void CellUpdate(Index r, Index c, const Cell&) override;
        void CountersReceived(unsigned rc, unsigned fc) override;
        void Ended() override;
        void Lost(Index r, Index c) override;
    
    private:
        sf::RenderWindow window;
        std::vector<std::vector<Tile>> tiles;

        Controller &context;
        TextureManager texman;

        Smiley smiley; // The smiley face
        sf::Font font;
        sf::Text message{font};
        sf::Text data{font};

        void HandleClicked(const sf::Event::MouseButtonPressed&);
        void HandleClickReleased(const sf::Event::MouseButtonReleased&);
        void DrawCell(Index r, Index c);
        void RefreshTexture(Index r, Index c, const Cell&);
        Tile* TileAt(float x, float y);
        std::optional<UPair<Index>> CalculateCellCoord(float x, float y) const;
        // void PublishCommand(std::unique_ptr<ICommand>); // wrapper to do additional work depending on the command
    
#ifndef NDEBUG
    sf::Text debugInfo{font};
    BoardSnapshot lastSnap;
#endif /* NDEBUG */
    };
}