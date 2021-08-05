# Generic test that uses conan libs
include_directories(../include)

add_executable(lotrchess main.cpp lotrchess.cpp play_mode.cpp splash_screen.cpp battle_setup_screen.cpp splash_dialog.cpp splash_dialog_view.cpp battle_setup_dialog.cpp battle_setup_dialog_view.cpp string_manip.cpp board.cpp board_view.cpp music_player.cpp toggle_button.cpp)
target_link_libraries(
        lotrchess
  PRIVATE project_options
          project_warnings
          CONAN_PKG::docopt.cpp
          CONAN_PKG::fmt
          CONAN_PKG::spdlog
          CONAN_PKG::imgui-sfml)
