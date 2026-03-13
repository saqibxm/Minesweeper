#include <array>
#include <string>
#include <cctype>
#include <optional>

#include "Graphics/DifficultySelectorDelegate.hpp"

using namespace mines;
using namespace mines::impl;

// ─── Colour palette ──────────────────────────────────────────────────────────
static constexpr sf::Color BG      {45,  45,  45};   // window background
static constexpr sf::Color BTN_IDLE{68,  68,  68};   // button normal
static constexpr sf::Color BTN_HOV {100, 100, 100};  // button hovered
static constexpr sf::Color BTN_CUST{50,  80,  110};  // custom button idle
static constexpr sf::Color BTN_CHOV{70,  110, 150};  // custom button hovered
static constexpr sf::Color TXT_MAIN{240, 240, 240};  // main text
static constexpr sf::Color TXT_SUB {170, 170, 170};  // subtitle / hint text

// Helper — build a subtitle string with grid info for a difficulty.
static std::string DiffSubtitle(Difficulty diff)
{
    auto cfg = DifficultyConfig::From(diff);
    if (diff == Difficulty::CUSTOM) return "Enter your own settings";
    return std::to_string(cfg.rows) + " \xc3\x97 " + std::to_string(cfg.cols)   // ×
           + "  \xe2\x80\xa2  " + std::to_string(cfg.mines) + " mines";          // •
}

DifficultySelectorDelegate::DifficultySelectorDelegate(sf::Font &font_) : font(font_)
{
}


DifficultyConfig DifficultySelectorDelegate::PromptSelection()
{
    constexpr std::size_t sz = 4;

    if (prompt.isOpen()) prompt.close();

    constexpr unsigned W = 280, H = 320;
    prompt.create(sf::VideoMode({W, H}), "Select Difficulty", sf::Style::Close);
    prompt.setFramerateLimit(30);

    constexpr Difficulty levels[sz] = {
        Difficulty::BEGINNER,
        Difficulty::INTERMEDIATE,
        Difficulty::EXPERT,
        Difficulty::CUSTOM
    };

    // ── Title ─────────────────────────────────────────────────────────────────
    sf::Text title(font, "Minesweeper", 22);
    title.setFillColor(TXT_MAIN);
    title.setPosition({W / 2.f - title.getLocalBounds().size.x / 2.f, 10.f});

    sf::Text hint(font, "1 Beginner  2 Intermediate  3 Expert", 9);
    hint.setFillColor(TXT_SUB);
    hint.setPosition({W / 2.f - hint.getLocalBounds().size.x / 2.f, 38.f});

    // ── Buttons ───────────────────────────────────────────────────────────────
    constexpr float BTN_X = 15.f, BTN_W = W - 30.f, BTN_H = 52.f, BTN_GAP = 8.f;
    constexpr float FIRST_Y = 60.f;

    sf::RectangleShape buttons[sz];
    sf::Text mainLabels[sz] = {sf::Text{font}, sf::Text{font}, sf::Text{font}, sf::Text{font}};
    sf::Text subLabels[sz]  = {sf::Text{font}, sf::Text{font}, sf::Text{font}, sf::Text{font}};

    for (std::size_t i = 0; i < sz; ++i)
    {
        float y = FIRST_Y + i * (BTN_H + BTN_GAP);
        buttons[i].setSize({BTN_W, BTN_H});
        buttons[i].setPosition({BTN_X, y});
        buttons[i].setFillColor(i < sz - 1 ? BTN_IDLE : BTN_CUST);

        mainLabels[i].setFont(font);
        mainLabels[i].setString(DifficultyConfig::DiffToString(levels[i]).data());
        mainLabels[i].setCharacterSize(16);
        mainLabels[i].setFillColor(TXT_MAIN);
        mainLabels[i].setPosition({BTN_X + 10.f, y + 8.f});

        subLabels[i].setFont(font);
        subLabels[i].setString(DiffSubtitle(levels[i]));
        subLabels[i].setCharacterSize(10);
        subLabels[i].setFillColor(TXT_SUB);
        subLabels[i].setPosition({BTN_X + 10.f, y + 30.f});
    }

    std::optional<DifficultyConfig> chosen;

    // ── Event loop ─────────────────────────────────────────────────────────────
    while (prompt.isOpen())
    {
        sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(prompt));

        while (const auto event = prompt.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                prompt.close();
                break;
            }

            if (const auto *mbr = event->getIf<sf::Event::MouseButtonReleased>())
            {
                const auto mpos = sf::Vector2f(mbr->position);
                for (std::size_t i = 0; i < sz; ++i)
                {
                    if (!buttons[i].getGlobalBounds().contains(mpos)) continue;
                    if (i < sz - 1)
                    {
                        chosen = DifficultyConfig::From(levels[i]);
                        prompt.close();
                    }
                    else
                    {
                        prompt.close();
                        auto [rows, cols, mines_] = PromptCustomDifficulty();
                        chosen.emplace(Difficulty::CUSTOM, rows, cols, mines_);
                        if (!chosen.value()) chosen = DifficultyConfig::From(Difficulty::BEGINNER);
                    }
                    break;
                }
            }

            // Keyboard shortcuts 1–3
            if (const auto *kp = event->getIf<sf::Event::KeyPressed>())
            {
                auto key = kp->code;
                if (key == sf::Keyboard::Key::Num1) { chosen = DifficultyConfig::From(Difficulty::BEGINNER);     prompt.close(); }
                if (key == sf::Keyboard::Key::Num2) { chosen = DifficultyConfig::From(Difficulty::INTERMEDIATE); prompt.close(); }
                if (key == sf::Keyboard::Key::Num3) { chosen = DifficultyConfig::From(Difficulty::EXPERT);       prompt.close(); }
                if (key == sf::Keyboard::Key::Num4) { prompt.close(); auto [r,c,m] = PromptCustomDifficulty(); chosen.emplace(Difficulty::CUSTOM,r,c,m); if(!chosen.value()) chosen=DifficultyConfig::From(Difficulty::BEGINNER); }
            }
        }

        // Hover colours
        for (std::size_t i = 0; i < sz; ++i)
        {
            bool hov = buttons[i].getGlobalBounds().contains(mousePos);
            if (i < sz - 1)
                buttons[i].setFillColor(hov ? BTN_HOV : BTN_IDLE);
            else
                buttons[i].setFillColor(hov ? BTN_CHOV : BTN_CUST);
        }

        prompt.clear(BG);
        prompt.draw(title);
        prompt.draw(hint);
        for (std::size_t i = 0; i < sz; ++i)
        {
            prompt.draw(buttons[i]);
            prompt.draw(mainLabels[i]);
            prompt.draw(subLabels[i]);
        }
        prompt.display();
    }

    if (!chosen.has_value())
        throw std::runtime_error("No difficulty selected.");

    return chosen.value();
}


std::array<unsigned, 3> DifficultySelectorDelegate::PromptCustomDifficulty()
{
    constexpr unsigned nfields = 3;
    sf::RenderWindow &inputWindow = prompt;
    prompt.create(sf::VideoMode({300, 300}), "Custom Difficulty", sf::Style::Close);
    prompt.setFramerateLimit(30);

    sf::Text title(font, "Custom Game", 18);
    title.setFillColor(TXT_MAIN);
    title.setPosition({10.f, 10.f});

    sf::Text subtitle(font, "Tab / Enter to advance fields", 10);
    subtitle.setFillColor(TXT_SUB);
    subtitle.setPosition({10.f, 36.f});

    std::array<std::string, nfields> labels = {"Rows:", "Cols:", "Mines:"};
    std::array<std::string, nfields> inputs = {};
    std::vector<sf::Text> inputTexts;
    std::array<sf::RectangleShape, nfields> inputBoxes;

    for (int i = 0; i < nfields; ++i)
    {
        inputTexts.emplace_back(font);
        inputTexts[i].setCharacterSize(16);
        inputTexts[i].setFillColor(sf::Color::White);
        inputTexts[i].setPosition({110.f, 60.f + i * 60.f + 6.f});

        inputBoxes[i].setSize({150.f, 32.f});
        inputBoxes[i].setPosition({100.f, 60.f + i * 60.f});
        inputBoxes[i].setFillColor(BTN_IDLE);
        inputBoxes[i].setOutlineThickness(2.f);
        inputBoxes[i].setOutlineColor(sf::Color::Transparent);
    }

    int currentBox = 0;
    sf::Text labelText(font, "", 16);
    labelText.setFillColor(TXT_SUB);

    sf::RectangleShape okButton({100.f, 38.f});
    okButton.setPosition({100.f, 248.f});
    okButton.setFillColor(BTN_CUST);
    sf::Text okText(font, "OK", 16);
    okText.setFillColor(TXT_MAIN);
    okText.setPosition({148.f - okText.getLocalBounds().size.x / 2.f, 255.f});

    while (inputWindow.isOpen())
    {
        sf::Vector2f mpos = sf::Vector2f(sf::Mouse::getPosition(inputWindow));
        okButton.setFillColor(okButton.getGlobalBounds().contains(mpos) ? BTN_CHOV : BTN_CUST);

        while (const auto event = inputWindow.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                inputWindow.close();
            else if (event->is<sf::Event::TextEntered>())
            {
                auto e = *event->getIf<sf::Event::TextEntered>();
                if (std::isdigit(e.unicode) && inputs[currentBox].size() < 3)
                    inputs[currentBox] += static_cast<char>(e.unicode);
                else if (e.unicode == '\b' && !inputs[currentBox].empty())
                    inputs[currentBox].pop_back();
                inputTexts[currentBox].setString(inputs[currentBox]);
            }
            else if (const auto *kp = event->getIf<sf::Event::KeyPressed>())
            {
                if (kp->code == sf::Keyboard::Key::Enter)
                {
                    if (currentBox < 2) ++currentBox;
                    else                inputWindow.close();
                }
                else if (kp->code == sf::Keyboard::Key::Tab)
                {
                    currentBox = (currentBox + 1) % nfields;
                }
            }
            else if (const auto *mbr = event->getIf<sf::Event::MouseButtonReleased>())
            {
                for (int i = 0; i < nfields; ++i)
                    if (inputBoxes[i].getGlobalBounds().contains(sf::Vector2f(mbr->position))) currentBox = i;
                if (okButton.getGlobalBounds().contains(sf::Vector2f(mbr->position)))
                    inputWindow.close();
            }

            for (int i = 0; i < nfields; ++i)
                inputBoxes[i].setOutlineColor(sf::Color::Transparent);
            inputBoxes[currentBox].setOutlineColor(sf::Color{100, 160, 220});
        }

        inputWindow.clear(BG);
        inputWindow.draw(title);
        inputWindow.draw(subtitle);
        for (int i = 0; i < nfields; ++i)
        {
            labelText.setString(labels[i]);
            labelText.setPosition({10.f, 60.f + i * 60.f + 6.f});
            inputWindow.draw(labelText);
            inputWindow.draw(inputBoxes[i]);
            inputWindow.draw(inputTexts[i]);
        }
        inputWindow.draw(okButton);
        inputWindow.draw(okText);
        inputWindow.display();
    }

    std::array<unsigned, 3> ret;
    for (std::size_t i = 0; i < std::size(ret); ++i)
    {
        try   { ret[i] = std::stoi(inputs[i]); }
        catch (...) { ret[i] = 0; }
    }
    return ret;
}