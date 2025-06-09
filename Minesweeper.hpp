#pragma once

#include <memory>

#include "core/Game.hpp"
#include "core/Controller.hpp"
#include "ui/View.hpp"

class Minesweeper {
public:
    Minesweeper(/* bool graphics */);
    void Run();

private:
    mines::Game game;
    mines::Controller controller;
    std::unique_ptr<mines::IView> view;
};

