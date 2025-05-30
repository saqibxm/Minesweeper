#include "State.hpp"
#include "Game.hpp"

using namespace mines;

ReadyState::ReadyState(Game &ctx) : State(ctx)
{
}

void ReadyState::Enter()
{
    context.Notify();
}

void ReadyState::Exit()
{
}

void ReadyState::Reveal(Index r, Index c)
{
    auto playstate = std::make_unique<PlayingState>(context);
    playstate->Reveal(r, c);
    context.TransitionTo(std::move(playstate));
}

void ReadyState::Flag(Index r, Index c)
{
    auto playstate = std::make_unique<PlayingState>(context);
    playstate->Flag(r, c);
    context.TransitionTo(std::move(playstate));
}

PlayingState::PlayingState(Game &ctx) : State(ctx)
{
}

void PlayingState::Enter()
{
    // context.Notify();
    // notify the views that we are in playing state so they update
}

void PlayingState::Exit()
{
}

void PlayingState::Reveal(Index row, Index col)
{
    context.RevealCell(row, col, [this](Index r, Index c) {
        context.BroadcastCellChange(r, c, context.CellAt(r, c));
    });

    context.BroadcastData(context.ClearCount(), context.FlagCount());

    if(context.CellAt(row, col).mine())
    {
        context.BroadcastGameOver(row, col);
        context.TransitionTo<GameOverState>();
    }
}

void PlayingState::Flag(Index row, Index col)
{
    context.FlagCell(row, col);
    context.BroadcastCellChange(row, col, context.CellAt(row, col));

    context.BroadcastData(context.ClearCount(), context.FlagCount());
}

GameOverState::GameOverState(Game &ctx) : State(ctx)
{
}

void GameOverState::Enter()
{
}

void GameOverState::Exit()
{
}