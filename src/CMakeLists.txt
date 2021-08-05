# Generic test that uses conan libs
include_directories(../include)

add_executable(lotrchess main.cpp string_manip.cpp board.cpp board_view.cpp music_player.cpp)
target_link_libraries(
        lotrchess
  PRIVATE project_options
          project_warnings
          CONAN_PKG::docopt.cpp
          CONAN_PKG::fmt
          CONAN_PKG::spdlog
          CONAN_PKG::imgui-sfml)
