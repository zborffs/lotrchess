/// third party includes
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <spdlog/spdlog.h>
#include <SFML/Audio/Music.hpp>

/// stl includes
#include <array>

/// project includes
#include "board.hpp"
#include "board_view.hpp"
#include "music_player.hpp"

int main() {

    /// initialize constant variables
    constexpr auto width = 1280;
    constexpr auto height = 1080;
    constexpr auto title = "Lord of the Rings: Chess";
    constexpr auto fps_limit = 30;
    constexpr auto scale_factor = 2.0;
    constexpr auto font_scale_factor = 1.0;
    const sf::Vector2f board_offset(50., 75.);
    const std::string board_texture_path("../../res/img/ChessBoard.png");
    const std::string one_ring_texture_path("../../res/img/one_ring.png");
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
    const std::array<std::string, 4> music_paths({
        "../../res/music/white_startup_music.wav",
        "../../res/music/white_victory_music.wav",
        "../../res/music/black_startup_music.wav",
        "../../res/music/black_victory_music.wav",
    });

    /// create the game objects
    Board board; // board model
    BoardView board_view(board, board_offset, board_texture_path, one_ring_texture_path, texture_map_paths); // board view
    MusicPlayer music_player(music_paths);

    /// create the window object and set the max frame rate
    sf::RenderWindow window(sf::VideoMode(width, height), title);
    window.setFramerateLimit(fps_limit);
    ImGui::SFML::Init(window);
    sf::Clock deltaClock; // change in time between frames

    /// scale the fonts and the objects
    ImGui::GetStyle().ScaleAllSizes(scale_factor);
    ImGui::GetIO().FontGlobalScale = font_scale_factor;


    /// main game-loop
    while (window.isOpen()) {
        // while the window is still open, process the events and draw the objects


        sf::Event event;
        while (window.pollEvent(event)) {
            // poll every event that has occurred
            ImGui::SFML::ProcessEvent(event);

            switch (event.type) {
            case sf::Event::Closed: {
                // if it was a close window event, then close the window
                window.close();
                break;
            }
            case sf::Event::KeyPressed: {
                if (event.key.code == sf::Keyboard::R) {
                    // if the user pressed 'R' then reverse the board
                    spdlog::info("R was pressed");
                    board.switch_sides(); // update the model
                    board_view.update_pieces(board); // update the view's understanding of the model
                } else if (event.key.code == sf::Keyboard::A) {
                    spdlog::info("A was pressed...");
                    music_player.play_startup_music(WHITE);
                } else if (event.key.code == sf::Keyboard::S) {
                    spdlog::info("S was pressed...");
                    music_player.play_startup_music(BLACK);
                } else if (event.key.code == sf::Keyboard::D) {
                    spdlog::info("D was pressed...");
                    music_player.play_victory_music(WHITE);
                } else if (event.key.code == sf::Keyboard::F) {
                    spdlog::info("F was pressed...");
                    music_player.play_victory_music(BLACK);
                }
            }
            case sf::Event::MouseButtonPressed: {
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;

                if (board_view.in_board(x, y)) {
                    // if the user pressed on the mouse inside the board's bounding box, then figure out what square was clicked
                    sf::Vector2f board_view_offset = board_view.board_offset();
                    int col = static_cast<int>((static_cast<float>(x) - board_view_offset.x) / 100);
                    int row = static_cast<int>((static_cast<float>(y) - board_view_offset.y) / 100);

                    // to determine what the square was, we need to check if the board is reversed or not on the screen
                    Square_t sq {Square::EP_DEFAULT};
                    if (board.as_white()) {
                        sq = static_cast<Square_t>(col + (7 - row) * 8);
                    } else {
                        sq = static_cast<Square_t>((7 - col) + row * 8);
                    }

                    if (board.valid_highlight(sq)) {
                        board.add_highlight(sq); // add a highlight on the square that was clicked
                        board_view.update_pieces(board); // update the pieces
                    } else {
                        board.clear_highlights(); // add a highlight on the square that was clicked
                        board_view.update_pieces(board); // update the pieces
                    }
                }
            }
            default:
                spdlog::trace("Unhanded event type in game-loop");
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart()); // let SFML update the window

//        ImGui::Begin("The Plan!");
//        for (std::size_t index{0}; const auto& step : steps) {
//            ImGui::Checkbox(fmt::format("{} : {}", index, step).c_str(), &states.at(index));
//            ++index;
//        }
//        ImGui::End();

        window.clear(); // clear the window
        ImGui::SFML::Render(window); // render the window
        board_view.draw(window); // draw the window
        window.display(); // display the window
    }

    ImGui::SFML::Shutdown(); // shut everything down

    return 0;
}
