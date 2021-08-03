#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <array>
#include <spdlog/spdlog.h>
#include <iostream>
#include "board.hpp"
#include "board_view.hpp"

int main() {
    /// initialize variables
    constexpr auto width = 1280;
    constexpr auto height = 1080;
    constexpr auto title = "Lord of the Rings: Chess";
    constexpr auto fps_limit = 30;
    constexpr auto scale_factor = 2.0;
    constexpr auto font_scale_factor = 1.0;
    const sf::Vector2f board_offset(50., 75.);
    const std::string board_texture_path("../../res/img/ChessBoard.png");
    const std::unordered_map<char, std::string> texture_map_paths({
        {'P', "../../res/img/Light_Pawn.png"},
        {'R', "../../res/img/Light_Rook.png"},
        {'N', "../../res/img/Light_Knight.png"},
        {'B', "../../res/img/Light_Bishop.png"},
        {'K', "../../res/img/Light_King.png"},
        {'Q', "../../res/img/Light_Queen.png"},
        {'p', "../../res/img/Dark_Pawn.png"},
        {'r', "../../res/img/Dark_Rook.png"},
        {'n', "../../res/img/Dark_Knight.png"},
        {'b', "../../res/img/Dark_Bishop.png"},
        {'k', "../../res/img/Dark_King.png"},
        {'q', "../../res/img/Dark_Queen.png"}
    });

    Board board;
    std::cout << board << std::endl;
    BoardView board_view(board, board_offset, board_texture_path, texture_map_paths);

    sf::RenderWindow window(sf::VideoMode(width, height), title);
    window.setFramerateLimit(fps_limit);
    ImGui::SFML::Init(window);

    ImGui::GetStyle().ScaleAllSizes(scale_factor);
    ImGui::GetIO().FontGlobalScale = font_scale_factor;

    constexpr std::array steps {
        "Friendship",
        "Is",
        "For",
        "Losers"
    };
    std::array<bool, steps.size()> states{};

    sf::Clock deltaClock;

    /// this is the main game loop
    while (window.isOpen()) {
        // while the window is still open
        sf::Event event;
        while (window.pollEvent(event)) {
            // poll every event that has occurred
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                // if the closed event was set, then close the window
                window.close();
            }
        }


        ImGui::SFML::Update(window, deltaClock.restart()); // update window

        ImGui::Begin("The Plan!");
        for (std::size_t index{0}; const auto& step : steps) {
            ImGui::Checkbox(fmt::format("{} : {}", index, step).c_str(), &states.at(index));
            ++index;
        }
        ImGui::End();

        window.clear(); // clear the window

        ImGui::SFML::Render(window); // render the window
        board_view.draw(window);
        window.display(); // display the window
    }

    ImGui::SFML::Shutdown(); // shut everything down

    return 0;
}
