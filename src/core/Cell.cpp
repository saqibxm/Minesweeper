#include <stdexcept>
#include <exception>
#include <string>

#include "../../inc/core/Cell.hpp"

using namespace mines;

Cell::Cell(bool is_mine)
    : mine_(is_mine), flag_{}, proximity_{}
{
    // mine(is_mine); // call the setter if additional work
}

bool Cell::mine() const
{
    return mine_;
}

void Cell::mine(bool set)
{
    mine_ = set;
    // proximity(-1); // is itself the mine
}

bool Cell::revealed() const
{
    return state_ == CLEARED;
}

bool Cell::flagged() const
{
    return flag_;
}

short Cell::proximity() const
{
    return proximity_;
}

bool Cell::proximal() const
{
    return proximity_ != 0;
}

void Cell::open()
{
    state_ = CLEARED;
}

Cell::State Cell::state() const
{
    return state_;
}

void Cell::toggle_flag()
{
    flag_ = !flag_;
    state_ = state() == FLAGGED ? HIDDEN : FLAGGED;
}

void Cell::reveal()
{
    state(CLEARED);
    flag_ = false;
}

void Cell::state(State s)
{
    state_ = s;
}

void Cell::proximity(short n)
{
    constexpr static int max_neighbouring_mines = 8;

    if((n < 0 || n > max_neighbouring_mines) && !mine_)
        throw std::logic_error("proximity is out of valid range of [0 - " + std::to_string(max_neighbouring_mines) + "]\n");

    proximity_ = n;
}