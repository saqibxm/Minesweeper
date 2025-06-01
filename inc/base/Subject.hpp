#pragma once

#include "common.h"
#include "./Observer.hpp"

namespace mines
{
    // interface IObserver;
    struct Cell;

    interface ISubject
    {
        virtual ~ISubject() = default;
        virtual void Attach(IObserver*) = ABSTRACT; // Subscribe / Register
        virtual void Detach(IObserver*) = ABSTRACT; // Unsubscribe / Remove
        virtual void Notify() = ABSTRACT; // Broadcast generic change
        virtual void BroadcastCounterData(unsigned, unsigned) = ABSTRACT;
        virtual void BroadcastCellChange(Index, Index, const Cell&) = ABSTRACT;
        virtual void BroadcastGameOver(Index, Index) = ABSTRACT; // notify observers on game end, flag indicates victory
        virtual void BroadcastConfigChange() = ABSTRACT;
        virtual void BroadcastVictory() = ABSTRACT;
    };
}