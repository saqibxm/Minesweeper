#include <iostream>
#include <iomanip>

#include "Console/ConsoleView.hpp"
#include "Controller.hpp"

using namespace mines;

ConsoleView::ConsoleView(Controller &ctrl)
    : console(std::cout), context(ctrl)
{
}

void ConsoleView::ShowMessage(const std::string &msg)
{

}
