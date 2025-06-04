#include "State.hpp"
#include "Game.hpp"

using namespace mines;

void State::NewGame(DifficultyConfig dc)
{
    context.Initialize(dc.rows, dc.cols, dc.mines);
}

ReadyState::ReadyState(Game &ctx) : State(ctx)
{
}

void ReadyState::Enter()
{
    context.state = EState::READY;
    // context.UpdateProximity();
    // context.Notify();
}

void ReadyState::Exit()
{
    context.state = EState::INDETERMINATE;
}

void ReadyState::Reveal(Index r, Index c)
{
    // context.TransitionTo<PlayingState>();
    // context.Reveal(r, c);
}

void ReadyState::Flag(Index r, Index c)
{
    // PURE GARBAGE
    /*
    auto s = std::make_unique<PlayingState>(context);
    s->Flag(r, c);
    context.TransitionTo(std::move(s));
    */
}

PlayingState::PlayingState(Game &ctx) : State(ctx)
{
}

void PlayingState::Enter()
{
    // context.Notify();
    // notify the views that we are in playing state so they update
    context.state = EState::PLAYING;
}

void PlayingState::Exit()
{
    context.state = EState::INDETERMINATE;
}

void PlayingState::Reveal(Index row, Index col)
{
    context.RevealCell(row, col, [this](Index r, Index c) {
        context.BroadcastCellChange(r, c, context.CellAt(r, c));
    });

    context.BroadcastCounterData(context.ClearCount(), context.FlagCount());

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

    context.BroadcastCounterData(context.ClearCount(), context.FlagCount());
}

GameOverState::GameOverState(Game &ctx) : State(ctx)
{
}

void GameOverState::Enter()
{
    context.state = EState::ENDED;
}

void GameOverState::Exit()
{
    context.state = EState::INDETERMINATE;
}