#include "Game.hpp"

using namespace mines;

Game::Game(std::size_t rows, std::size_t cols, std::size_t mines)
    : gameState(nullptr), config(Difficulty::CUSTOM, rows, cols, mines)
{
    Initialize(rows, cols, mines);
    TransitionTo<ReadyState>();
}

Game::~Game()
{
    if (gameState) gameState->Exit();
}

void Game::NewGame(const DifficultyConfig &dc)
{
    config = dc;
    gameState->NewGame(dc);
    TransitionTo<ReadyState>();
    Notify();
}

void Game::NewGame(std::size_t rows, std::size_t cols, std::size_t mines)
{
    config.level = Difficulty::CUSTOM;
    config.rows = rows;
    config.cols = cols;
    config.mines = mines;

    NewGame(config);
}

void Game::Reveal(Index r, Index c)
{
    // TODO: implement state pattern
    if (state == READY)
        TransitionTo<PlayingState>();

    gameState->Reveal(r, c);

    if (CheckWin())
    {
        TransitionTo<GameOverState>();
        BroadcastVictory();
    }
}

void Game::Flag(Index r, Index c)
{
    if (state == READY) TransitionTo<PlayingState>();
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

void Game::BroadcastCounterData(unsigned revealCount, unsigned flagCount)
{
    for(auto pobs : observers)
        pobs->CountersReceived(revealCount, flagCount);
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