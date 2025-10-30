#include <cassert>

#include "Controller.hpp"
#include "Graphics/GraphicalView.hpp"

using namespace mines;

/*
Controller::Controller(Game *p_model, bool graphical)
: model(p_model)
{
    view.reset(new Graphics(*model));
    assert(model && view);
    
    model->Attach(view.get());
}
*/

Controller::Controller(Game &gameModel) : model(gameModel)
{
    // commands.reserve(100);
    timer.reset();
}

Controller::~Controller()
{
}

void Controller::Update()
{
    if (model.CurrentState() != EState::PLAYING) return;
    HandleCommand(std::make_unique<TimerUpdateCommand>(timer.elapsed()));
}


void Controller::RevealRequested(Index r, Index c)
{
    if (model.CurrentState() == EState::READY) timer.start();

    auto cmd = std::make_unique<RevealCommand>(r, c);
    bool success = HandleCommand(std::move(cmd));

    if (success && !timer.is_running())
        timer.start();
}

void Controller::FlagRequested(Index r, Index c)
{
    if (model.CurrentState() == EState::READY) timer.start();

    auto cmd = std::make_unique<FlagCommand>(r, c);
    auto success = HandleCommand(std::move(cmd));

    if (success && !timer.is_running())
        timer.start();
}

void Controller::NewGameRequested(const DifficultyConfig &cfg)
{
    auto cmd = std::make_unique<NewCustomGameCommand>(cfg);
    HandleCommand(std::move(cmd));

    timer.reset();
}

void Controller::NewGameRequested()
{
    auto cmd = std::make_unique<NewGameCommand>();
    HandleCommand(std::move(cmd));

    timer.reset();
}


bool Controller::HandleCommand(std::unique_ptr<ICommand> cmd)
{
    bool success = cmd->Execute(model);

    if (success)
        commands.push(std::move(cmd));

    // commands.top()->Execute(model);
    return success;
}