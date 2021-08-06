# Generic test that uses conan libs
include_directories(../include)

add_executable(lotrchess main.cpp lotrchess.cpp play_screen.cpp splash_screen.cpp battle_setup_screen.cpp splash_controller.cpp splash_view.cpp battle_setup_controller.cpp battle_setup_view.cpp string_manip.cpp play_controller.cpp play_view.cpp music_player.cpp toggle_button.cpp engine_interface.cpp board.cpp chess_move.cpp bitmask.cpp move_gen.cpp attack.cpp)
target_link_libraries(
        lotrchess
  PRIVATE project_options
          project_warnings
          CONAN_PKG::docopt.cpp
          CONAN_PKG::fmt
          CONAN_PKG::spdlog
          CONAN_PKG::imgui-sfml)
