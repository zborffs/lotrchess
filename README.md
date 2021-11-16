# Lord of the Rings: Chess
- this is a Lord of the Rings themed chess game in C++.
- Check out the [demo](https://zborffs.github.io/site/posts/lotr-chess.html).

## Art Contributions
- Huge thanks to Erica Hanania for making the board and piece sprites

## Download and Install
- the only way of installing the app at the moment is to build it from source:

```shell
git clone https://github.com/zborffs/lotrchess
mkdir -p lotrchess/build
pushd lotrchess/build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
popd
```

### User Prerequisites
Running the preceding commands will download and build the software. Make sure you have the following dependencies 
before attempting to install:
1. CMake (3.15 or later)
2. conan

### Source Dependencies
The project uses conan to handle all the source dependencies. The code relies on the following libraries:
  1. boost.process ~ talking to the uci engine
  2. SFML+ImGui ~ Graphics + UI components
  3. spdlog ~ logging back to the programmer

## Todo List:
- pick promoting piece (user) -> another screen + event processing without losing gamestate for Play?
  - easiest way of doing this would be to simply add another variable to the playscreen class or something, determining
    whether or not we are in the promotion pick state. If we are not, then act as usual, but if we are, then depict 
    another screen and have new update actions?
  - This seems very hacky, especially considering we already have a mechanism for doing this (state update). But the 
    problem is that we destroy the current game state whenever we change states. But we can't do that in this 
    circumstance, because the PlayScreen game state has important information about the board.
- Refactor to clean up code
- Refactor to simplify interface boundary between engine_io and controller
- maybe extract some functionality out of the playcontroller?
- widget classes:
  - Mute Button Class -> same deal with figuring out where to go, included as an object
  - Options Class -> Same deal
  - Back Button Class
  - Toggle Button:
- Save / serialize game state -> for example whose turn it is and the current position
- read pgn or output from c-chess-cli to playback in real-time what was happening
- Replay games


Idea:
- Screen contains a generic pointer to a View
- All concrete Views are pulled from View class
- Generic View contains a reference or pointer or something to an object (private accessed through protected 
  non-overridable functions), which points to a data class containing all the resource information.
  - This way all the views are accessing the exact same resource information (something like a singleton/flyweight 
    pattern in terms of construction of the resources (i.e. only construct on first use)), so we don't have to keep 
    rebuilding the resource data, and only one class manages it?

Idea 2:
- Screen contains a generic pointer to a Controller class
- All concrete controllers inherit from the Controller class
- Generic Controller has some functions (template or strategy visitor?) for saving the state to a json file with a timestamp
