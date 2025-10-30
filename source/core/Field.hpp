#pragma once

#ifndef BOARD_HPP
#define BOARD_HPP

#include <utility>
#include <random>
#include <functional>

#include "Cell.hpp"
#include "common.h"
#include "utility.h"
#include "FlatVector.hpp"
#include "NumberGenerator.hpp"

namespace mines
{
    class Field
    {
        unsigned rows = 0, cols = 0;
    
    public:
        // using Grid = std::vector<std::vector<Cell>>;
        using Grid = FlatVector<Cell>;
        using RevealCallback = std::function<void(Index, Index)>;

        Field() = default;
        Field(Index n_rows, Index n_cols, Index n_mines);

        void Initialize(Index n_rows, Index n_cols, Index n_mines = 10);
        void PlaceMines(); // fill the board with mines at random places

        // void RevealSingle(Index row, Index col); // helper
        void RevealCell(Index row, Index col, const RevealCallback &cb = [](Index, Index) -> void { /* Do Nothing by default! */ });
        void FlagCell(Index row, Index col);
        void UpdateProximity(); // of all cells
        void UpdateProximity(Index, Index); // of a specific cell

        int FlagCount() const { return flagged; }
        unsigned ClearCount() const { return cleared; }
        unsigned MineCount() const { return total_mines; }
        UPair<unsigned> Dimensions() const { return {rows, cols}; }
        
        Cell& CellAt(Index r, Index c) { return board[r][c]; /* board(r, c); */ }
        const Cell& CellAt(Index r, Index c) const { return board[r][c]; /* board(r, c); */ }
        Grid& Board() { return board; }
        Cell& operator[](Index);
        const Cell& operator[](Index) const;

    private:
        Grid board; // the board itself
        // std::vector<UPair<Index>> mines; // the indices of the mines present on board

        // Information
        unsigned total_mines = 0;
        unsigned cleared = 0;
        int flagged = 0;

        static NumberGenerator<Index> generator;
        void ShowMinesAndFlags(const RevealCallback &cb); // in the case of lose/win
        /* static */ Index RandomNumber();
        short ComputeAdjacency(Index, Index) const;
        UPair<Index> TranslateFlatIndex(Index idx) const { return TranslateIndex(idx, cols); }
    };
}

#endif