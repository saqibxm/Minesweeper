#pragma once

#include "common.h"
#include "utility.h"

namespace mines
{
class Game;

interface ICommand
{
    virtual ~ICommand() = default;
    virtual bool Execute(Game&) = ABSTRACT; // return type now signals whether it wasnt no-op
};

class RevealCommand final : public ICommand
{
public:
    RevealCommand(Index, Index);
    bool Execute(Game&) override;

private:
    Index row, col;
};

class FlagCommand final : public ICommand
{
public:
    FlagCommand(Index, Index);
    bool Execute(Game&) override;

private:
    Index row, col;
};

class NewGameCommand final : public ICommand
{
public:
    NewGameCommand() = default;
    bool Execute(Game&) override;
};


class NewCustomGameCommand final : public ICommand
{
public:
    NewCustomGameCommand(const DifficultyConfig&);
    bool Execute(Game&) override;

private:
    DifficultyConfig diff;
};

}