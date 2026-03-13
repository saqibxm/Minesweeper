#include <cassert>
#include <filesystem>

#include "Controller.hpp"
#include "Graphics/GraphicalView.hpp"

using namespace mines;

Controller::Controller(Game &gameModel) : model(gameModel)
{
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

    if (success)
        currentRecording.Record(ReplayEntry::Action::REVEAL, r, c, timer.elapsed());
}

void Controller::FlagRequested(Index r, Index c)
{
    if (model.CurrentState() == EState::READY) timer.start();

    auto cmd = std::make_unique<FlagCommand>(r, c);
    auto success = HandleCommand(std::move(cmd));

    if (success && !timer.is_running())
        timer.start();

    if (success)
        currentRecording.Record(ReplayEntry::Action::FLAG, r, c, timer.elapsed());
}

void Controller::SaveAndResetRecording()
{
    if (!currentRecording.Empty())
    {
        lastReplay = currentRecording;
        // Auto-save to disk
        std::filesystem::path dir = Replay::DefaultPath().parent_path();
        std::filesystem::create_directories(dir);
        lastReplay.Save(Replay::DefaultPath());
    }
    currentRecording.Clear();
}

void Controller::NewGameRequested(const DifficultyConfig &cfg)
{
    SaveAndResetRecording();

    auto cmd = std::make_unique<NewCustomGameCommand>(cfg);
    HandleCommand(std::move(cmd));

    timer.reset();
}

void Controller::NewGameRequested()
{
    SaveAndResetRecording();

    auto cmd = std::make_unique<NewGameCommand>();
    HandleCommand(std::move(cmd));

    timer.reset();
}


bool Controller::HandleCommand(std::unique_ptr<ICommand> cmd)
{
    bool success = cmd->Execute(model);

    if (success)
        commands.push(std::move(cmd));

    return success;
}