#include "Game.hpp"

using namespace mines;

Game::Game() : config()
{
    TransitionTo<ReadyState>();
}

Game::Game(const DifficultyConfig &cfg)
{
    NewGame(config);
}

Game::Game(std::size_t rows, std::size_t cols, std::size_t mines)
    : gameState(nullptr), config(Difficulty::CUSTOM, rows, cols, mines)
{
    Initialize(rows, cols, mines);
    BroadcastConfigChange();
    Notify();
    TransitionTo<ReadyState>();
}

Game::~Game()
{
    if (gameState) gameState->Exit();
}

void Game::NewGame(const DifficultyConfig &dc)
{
    gameState->NewGame(dc);
    TransitionTo<ReadyState>();
    if (config != dc)
    {
        config = dc;
        BroadcastConfigChange();
    }
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
    if (state == EState::READY)
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
    if (state == EState::READY) TransitionTo<PlayingState>();
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
    BroadcastConfigChange();
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
    BoardSnapshot snap = {config, Board()};

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

void Game::BroadcastConfigChange()
{
    for (auto pobs : observers)
        pobs->ConfigUpdate(config);
}


void Game::BroadcastGameOver(Index r, Index c) // override
{
    for(auto pobs : observers)
        pobs->Lost(r, c);
}

void Game::BroadcastVictory()
{
    for(auto pobs : observers)
        pobs->Won();
}

void Game::BroadcastTimerUpdate(double seconds)
{
    for (auto pobs : observers)
        pobs->TimeReceived(seconds);
}


void Game::TransitionTo(std::unique_ptr<State> next)
{
    gameState->Exit();
    gameState = std::move(next);
    gameState->Enter();
}