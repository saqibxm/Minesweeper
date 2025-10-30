#pragma once

#include "common.h"
#include "utility.h"

namespace mines
{
    enum class EState { READY, PLAYING, ENDED, INDETERMINATE };

    /**
     * Game Model Finite State Machine
    **/

    class Game; // Model
    class State
    {
    public:
        explicit State(Game &ctx) : context(ctx) {}
        virtual ~State() = default;

        virtual void Enter() = ABSTRACT;
        virtual void Exit() = ABSTRACT;

        virtual void Reveal(Index, Index) {}
        virtual void Flag(Index, Index) {}
        virtual void NewGame(DifficultyConfig);

        Game& Context() { return context; }

    protected:
        Game &context;
    };

    struct ReadyState final : public State
    {
        ReadyState(Game &ctx);

        void Enter() override;
        void Exit() override;

        void Reveal(Index r, Index c) override;
        void Flag(Index r, Index c) override;
    };

    struct PlayingState final : public State
    {
        PlayingState(Game &ctx);

        void Enter() override;
        void Exit() override;

        virtual void Reveal(Index, Index) override;
        virtual void Flag(Index, Index) override;
        // virtual void NewGame(DifficultyConfig) override;
    };

    struct GameOverState final : public State
    {
        GameOverState(Game &ctx);

        void Enter() override;
        void Exit() override;
        // virtual void NewGame(DifficultyConfig) override;
    };
}


/*
template <GameStates S>
class TState
    {
    public:
        State(Game &ctx) : context(ctx) {}
        virtual ~State() = default;

        virtual void Enter() = ABSTRACT;
        virtual void Exit() = ABSTRACT;

        virtual void Reveal(Index, Index) {}
        virtual void Flag(Index, Index) {}
        Game& Context() { return context; }

    protected:
        Game &context;
    };


    template <GameStates S>
    void TState<S>::Reveal(Index row, Index col)
    {
        this->context.Board().RevealCell(row, col, [this](Index r, Index c) {
        this->context.BroadcastCellChange(r, c);
    });
       if(this->context.Board().CellAt(row, col).mine())
        this->context.BroadcastGameOver(row, col);
    }
*/