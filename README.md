# Lord of the Rings: Chess

## Download and Install


#### Source Dependencies
boost.process
sfml
imgui
spdlog

## TODO:
- automatic update of chess board - not based on processing inputs
- widget classes:
  - Mute Button Class -> same deal with figuring out where to go, included as an object
  - Options Class -> Same deal
  - Back Button Class
  - Toggle Button:
- Save / serialize game state -> for example whose turn it is and the current position
- read pgn or output from c-chess-cli to playback in real-time what was happening
- Flyweight pattern? // this is already sort of done (or SFML gives us the option to do this or does it itself)
- Replay games
- promoting pick piece

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

too many files and threads running around!
- need to simplify things

 