#pragma once

#include "common.h"

namespace mines
{
    struct BoardSnapshot;
    interface ISubject;

    class Cell;

    interface IObserver
    {
        virtual ~IObserver() = default;
        virtual void Update(const BoardSnapshot&) = ABSTRACT;
        virtual void CellUpdate(Index, Index, const Cell&) = ABSTRACT;
        virtual void CountersReceived(unsigned, unsigned) = ABSTRACT;
        virtual void ConfigUpdate(const DifficultyConfig&) = ABSTRACT;
        virtual void Won() = ABSTRACT;
        virtual void Lost(Index, Index) = ABSTRACT;
        virtual void TimeReceived(double) = ABSTRACT;
        // virtual void ShutdownReceived() = ABSTRACT;
    };
}