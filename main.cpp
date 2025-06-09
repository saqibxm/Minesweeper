#include "Game.hpp"
#include "Controller.hpp"
#include "Graphics/GraphicalView.hpp"

int main()
{
    mines::Game game;
    mines::Controller ctrl(game);
    mines::Graphics view(ctrl);

    auto diff = view.SelectDifficulty();
    ctrl.NewGameRequested(diff);

    game.Attach(&view); // order matters because attaching unconditionally broadcasts config

    while(!view.ShouldClose())
    {
        view.Display();
        ctrl.Update();
    }
}