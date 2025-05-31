#pragma once

#include "common.h"
#include "utility.h"

namespace mines
{
class Game;

interface ICommand
{
    virtual ~ICommand() = default;
    virtual void Execute(Game&) = ABSTRACT;
};

class RevealCommand final : public ICommand
{
public:
    RevealCommand(Index, Index);
    void Execute(Game&) override;

private:
    Index row, col;
};

class FlagCommand final : public ICommand
{
public:
    FlagCommand(Index, Index);
    void Execute(Game&) override;

private:
    Index row, col;
};

class NewGameCommand final : public ICommand
{
public:
    NewGameCommand() = default;
    void Execute(Game&) override;
};


class NewCustomGameCommand final : public ICommand
{
public:
    NewCustomGameCommand(const DifficultyConfig&);
    void Execute(Game&) override;

private:
    DifficultyConfig diff;
};

}