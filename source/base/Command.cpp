#include "Command.hpp"
#include "Game.hpp"

using namespace mines;

RevealCommand::RevealCommand(Index r, Index c)
    : row(r), col(c)
{
}

bool RevealCommand::Execute(Game &model)
{
    if (model.CellAt(row, col).revealed()) return false;
    model.Reveal(row, col);
    return true;
}


FlagCommand::FlagCommand(Index r, Index c)
    : row(r), col(c)
{
}

bool FlagCommand::Execute(Game &model)
{
    model.Flag(row, col);
    return true;
}

bool NewGameCommand::Execute(Game &model)
{
    model.NewGame(model.CurrentConfig());
    return true;
}

NewCustomGameCommand::NewCustomGameCommand(const DifficultyConfig &dc)
    : diff(dc)
{
}

bool NewCustomGameCommand::Execute(Game &model)
{
    model.NewGame(diff);
    return true;
}

TimerUpdateCommand::TimerUpdateCommand(double secs)
    : seconds(secs)
{
}

bool TimerUpdateCommand::Execute(Game &model)
{
    model.BroadcastTimerUpdate(seconds);
    return true;
}
