#pragma once

#include "common.h"

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

}