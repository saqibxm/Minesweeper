#pragma once

#include <memory>
#include <list>
#include <type_traits>

#include "utility.h"
#include "State.hpp"
#include "Subject.hpp"
#include "Field.hpp"

namespace mines
{
    // Model of the MVC
struct BoardSnapshot
{
    struct DebugCellInfo {
        bool is_mine;
        bool is_flagged;
        short neighbour_mines;
    };
    std::size_t rows, cols;
    std::size_t mines;
    Field::Grid cells;
};

class Game : public ISubject, public Field // Field is not a dynamic class
{
    enum EState { READY, PLAYING, ENDED };
    friend class ReadyState;
    friend class PlayingState;
    friend class GameOverState;

public:
    Game(std::size_t w, std::size_t h, std::size_t m);
    Game(const DifficultyConfig&);

    void NewGame(const DifficultyConfig&);
    void NewGame(std::size_t w, std::size_t h, std::size_t m);

    void Reveal(Index r, Index c);
    void Flag(Index r, Index c);
    
    void Attach(IObserver*) override;
    void Detach(IObserver*) override;
    void Notify() override;

    template <typename T, typename = std::enable_if_t<std::is_base_of_v<State, T>>>
    void TransitionTo()
    {
        gameState->Exit();
        gameState.reset(new T(*this));
        gameState->Enter();
    }

    void TransitionTo(std::unique_ptr<State>);

    EState CurrentState() const { return state; }
    DifficultyConfig CurrentConfig() const { return config; }

private:
    EState state;
    std::unique_ptr<State> gameState;
    DifficultyConfig config;
    
    bool CheckWin() const;
    
    std::list<IObserver*> observers;
    void BroadcastCounterData(unsigned, unsigned) override;
    void BroadcastCellChange(Index, Index, const Cell&) override;
    void BroadcastGameOver(Index, Index) override;
    void BroadcastVictory() override;
};

}