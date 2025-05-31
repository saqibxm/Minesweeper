#include <random> // prng
#include <cassert> // assert
#include <algorithm> // std::shuffle
#include <numeric> // std::iota
#include <stdexcept> // std::out_of_range
#include <queue>
#include <vector>

#include "Field.hpp"

using namespace mines;

std::default_random_engine Field::randeng{ std::random_device{}() };
// watch out for static order initilization fiasco

Field::Field(Index n_rows, Index n_cols, unsigned n_mines)
    : total_mines(n_mines), rows(n_rows), cols(n_cols)
    , board(rows, Grid::value_type(cols))
{
    PlaceMines();
}

void Field::Initialize(Index n_rows, Index n_cols, unsigned n_mines)
{
    rows = n_rows;
    cols = n_cols;
    total_mines = n_mines;

    board.assign(rows, Grid::value_type(cols));
    PlaceMines();

    if constexpr(debug)
        UpdateProximity(); // if debugging update proximity of all cells beforehand

    cleared = 0;
    flagged = 0;
}

void Field::PlaceMines()
{
    std::ptrdiff_t count;
    for(const auto &row : board)
    {
        count = std::count_if(row.begin(), row.end(), [](const Cell &c) {
            return c.mine();
        });
    }

    assert(count == 0); // board must be clean

    // Fisher-Yates shuffle for O(N) placement
    std::vector<Index> indices(rows * cols);

    std::iota(indices.begin(), indices.end(), decltype(indices)::value_type{0}); // fill the indices with consecutive integers starting from 0
    std::shuffle(indices.begin(), indices.end(), randeng); // shuffle the indices

    for (int i = 0; i < total_mines; ++i)
    {
        auto [row, col] = translate_index(indices[i]);
        // mines.emplace_back(row, col); // put the index into mine* list
        board[row][col].mine(true); // set the cell to be a mine
    }
}

void Field::RevealCell(Index row, Index col, const RevealCallback &callback)
{
    if (row >= rows || col >= cols) return;

    std::queue<UPair<Index>> candidates;
    candidates.emplace(row, col); // add the current cell initially to avoid specific handling

    while (!candidates.empty())
    {
        auto [r, c] = candidates.front();
        candidates.pop();
        auto& current = board[r][c];

        if (current.revealed()) continue; // if already revealed, skip!

        if(current.flagged()) // default winmine behaviour is to clear the cell that's flagged
        {
            current.toggle_flag();
            --flagged;
        }

        current.reveal(); // finally clear the cell
        
        UpdateProximity(r, c); // after revealing the cell, update its proximity
        
        ++cleared; // modify to not increment when the cell was a mine
        
        callback(r, c); // run the callback on this index, subject of removal
        
        if(current.mine())
            RevealMinesAndFlags(callback);

        // Only continue expansion for cells with zero proximity
        if (current.proximity() != 0 || current.mine()) continue;

        for (int dr = -1; dr <= 1; ++dr)
        {
            for (int dc = -1; dc <= 1; ++dc)
            {
                if (dr == 0 && dc == 0) continue;

                Index nr = r + dr;
                Index nc = c + dc;
                if (nr >= rows || nc >= cols) continue;

                auto& neighbor = board[nr][nc];
                if (!neighbor.revealed())
                    candidates.emplace(nr, nc);
            }
        }
    }
}

void Field::FlagCell(Index row, Index col)
{
    if (row >= board.size() || col >= board[row].size()) return;

    auto &cell = board[row][col];

    if(cell.revealed()) return;

    if(cell.flagged())
        --flagged;
    else
        ++flagged;

    cell.toggle_flag();
}

void Field::UpdateProximity()
{
    // assert(!mines.empty() || total_mines != 0); // there should be mines on the board

    // redundant due to a check already in parameterized variant

    for (std::size_t i = 0; i < board.size(); ++i)
    {
        for (std::size_t j = 0; j < board[i].size(); ++j)
        {
            if (board[i][j].mine())
                continue;
            board[i][j].proximity(compute_adjacency(i, j));
        }
    }
}

void Field::UpdateProximity(Index row, Index col)
{
    if(board[row][col].mine()) return;
    board[row][col].proximity(compute_adjacency(row, col));
}

/*
Cell& Field::CellAt(Index row, Index col)
{
    if(row >= board.size() || col >= board[row].size()) throw std::out_of_range("Grid range is out of bounds");
    return board[row][col];
}
*/

void Field::RevealMinesAndFlags(const RevealCallback &callback)
{
    // TODO: Optimise
    for(std::size_t i = 0; i < board.size(); ++i)
    {
        for(std::size_t j = 0; j < board[i].size(); ++j)
        {
            auto &cell = board[i][j];
            if(!cell.mine() && !cell.flagged())
                continue;
            cell.open(); // different from cell.reveal();
            callback(i, j);
        }
    }
}

Cell& Field::operator[](Index idx)
{
    return const_cast<Cell&>(static_cast<const Field*>(this)->operator[](idx));
}

const Cell& Field::operator[](Index idx) const
{
    auto [row, col] = translate_index(idx);
    return board[row][col];
}

Index Field::get_randnum()
{
    auto range = std::make_pair(0, (rows * cols) - 1);
    static std::uniform_int_distribution<Index> dist(range.first, range.second);

    return dist(Field::randeng);
}

short Field::compute_adjacency(Index row, Index col) const
{
    short count = 0;

    for (int i = -1; i <= 1; ++i)
    {
        if (row + i >= board.size())
            continue;
        for (int j = -1; j <= 1; ++j)
        {
            if (col + j >= board[row].size())
                continue;
            if (board[row + i][col + j].mine())
                ++count;
        }
    }
    return count;
}
