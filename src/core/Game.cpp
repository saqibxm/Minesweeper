#include "Game.hpp"

using namespace mines;

Game::Game(std::size_t w, std::size_t h, std::size_t m) : gameState(new ReadyState(*this))
{
    Initialize(w, h, m);
    // Notify();
    state = PLAYING;
}

void Game::Reveal(Index r, Index c)
{
    // TODO: implement state pattern
    gameState->Reveal(r, c);
}

void Game::Flag(Index r, Index c)
{
    gameState->Flag(r, c);
}

bool Game::CheckWin() const
{
    auto [rows, cols] = Dimensions();
    if(ClearCount() == (rows * cols) - MineCount())
    {
        return true;
    }
    return false;
}

void Game::Attach(IObserver *obs) // override
{
    Detach(obs); // remove if already exists
    observers.push_back(obs);
    Notify();
}

void Game::Detach(IObserver *obs) // override
{
    observers.remove(obs);
}

void Game::Notify() // override
{
    if(observers.empty()) return;

    auto [rows, cols] = Dimensions();
    BoardSnapshot snap = {rows, cols, MineCount(), Board()};

    for(auto pobs : observers)
        pobs->Update(snap);
}

void Game::BroadcastData(unsigned revealCount, unsigned flagCount)
{
    for(auto pobs : observers)
        pobs->DataReceived(revealCount, flagCount);
}

void Game::BroadcastCellChange(Index row, Index col, const Cell &cell) // override
{
    for(auto pobs : observers)
        pobs->CellUpdate(row, col, CellAt(row, col));
}

void Game::BroadcastGameOver(Index r, Index c) // override
{
    for(auto pobs : observers)
        pobs->Lost(r, c);
}

void Game::BroadcastVictory()
{
    for(auto pobs : observers)
        pobs->Ended();
}

void Game::TransitionTo(std::unique_ptr<State> next)
{
    gameState->Exit();
    gameState = std::move(next);
    gameState->Enter();
}