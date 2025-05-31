#include "Command.hpp"
#include "Game.hpp"

using namespace mines;

RevealCommand::RevealCommand(Index r, Index c)
    : row(r), col(c)
{
}

void RevealCommand::Execute(Game &model)
{
    model.Reveal(row, col);
}


FlagCommand::FlagCommand(Index r, Index c)
    : row(r), col(c)
{
}

void FlagCommand::Execute(Game &model)
{
    model.Flag(row, col);
}

void NewGameCommand::Execute(Game &model)
{
    model.NewGame(model.CurrentConfig());
}

NewCustomGameCommand::NewCustomGameCommand(const DifficultyConfig &dc)
    : diff(dc)
{
}

void NewCustomGameCommand::Execute(Game &model)
{
    model.NewGame(diff);
}