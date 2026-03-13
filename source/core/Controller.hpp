#pragma once

#include <memory>
#include <stack>

#include "common.h"
#include "Game.hpp"
#include "View.hpp"
#include "Command.hpp"
#include "stopwatch.h"
#include "Replay.hpp"

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

    UPair<unsigned> ModelSize() const { return model.Dimensions(); }

    // Replay access — returns the recording for the last completed game.
    const Replay& LastReplay() const noexcept { return lastReplay; }

private:
    Game &model;
    Stopwatch timer;

    std::stack <
        std::unique_ptr<ICommand>,
        std::vector<std::unique_ptr<ICommand>>
    > commands;

    Replay currentRecording; // records the game in progress
    Replay lastReplay;       // saved when a new game starts

    bool HandleCommand(std::unique_ptr<ICommand>);
    void SaveAndResetRecording();
};

}