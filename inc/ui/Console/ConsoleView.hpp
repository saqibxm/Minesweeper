#pragma once

#include <iosfwd>
#include <vector>

#include "View.hpp"

namespace mines {

class ConsoleView final : public IView
{
    using Block = char;
public:
    ConsoleView(Controller&);

    DifficultyConfig SelectDifficulty() override;
    void ShowMessage(const std::string &) override;
    void Display() override;
    bool ShouldClose() const override;

    void Update(const BoardSnapshot &) override;
    void CellUpdate(Index, Index, const Cell &) override;
    void CountersReceived(unsigned, unsigned) override;
    void Ended() override;
    void Lost(Index, Index) override;

private:
    std::ostream &console;
    Controller &context;
    std::vector<std::vector<Block>> blocks;
    std::size_t cleared = 0, flagged = 0;
};

}