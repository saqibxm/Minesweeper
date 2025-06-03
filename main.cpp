#include "Game.hpp"
#include "Controller.hpp"
#include "Graphics/GraphicalView.hpp"

int main()
{
    mines::Game game(12, 12, 15);
    mines::Controller ctrl(game);
    mines::Graphics view(ctrl);

    game.Attach(&view);

    while(!view.ShouldClose())
    {
        view.Display();
        ctrl.Update();
    }
}