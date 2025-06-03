#pragma once

#include <memory>
#include <stack>

#include "common.h"
#include "Game.hpp" // convert to an interface?
#include "View.hpp"
#include "Command.hpp"
#include "stopwatch.h"

namespace mines
{

class Controller
{
public:
    Controller(Game &gameModel);
    ~Controller();

    void Update();

    void RevealRequested(Index, Index);
    void FlagRequested(Index, Index);
    void NewGameRequested();
    void NewGameRequested(const DifficultyConfig &);
    void ShutdownRequested();
    // Cell& FetchCell(Index, Index);
    UPair<unsigned> ModelSize() const { return model.Dimensions(); }

private:
    Game &model;
    Stopwatch timer;

    // bool running; // when the main view requests shut down set to false

    std::stack <
        std::unique_ptr<ICommand>,
        std::vector<std::unique_ptr<ICommand>>
    > commands;

    bool HandleCommand(std::unique_ptr<ICommand>);
};

}