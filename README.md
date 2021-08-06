# Lord of the Rings: Chess


## TODO:
- widget classes:
  - Title class -> figures out where within the page to go by itself, included as an object by all the viewers
  - Mute Button Class -> same deal with figuring out where to go, included as an object
  - Options Class -> Same deal
  - Back Button Class
  - Toggle Button: 
- Save / serialize game state -> for example whose turn it is and the current position
- read pgn or output from c-chess-cli to playback in real-time what was happening
- inheritance between view clases?
- inheritance between dialog clases?
- Flyweight pattern? // this is already sort of done (or SFML gives us the option to do this or does it itself)
- Replay games
- multithreading for reading from engine + updating things on screen etc. etc.


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

Idea 3: -> not worth it.
- Screen class also behaves like template by implementing skeleton of both the state handlers so we don't repeat shit

Three threads:
3. engine communication


too many files and threads running around!
- need to simplify things

 