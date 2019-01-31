# Simply double clicking on index.html won't work!

The reason is that the grayscale effect in the game is done using a method that has a browser security issue. As a result, the game must be loaded via a server to see the grayscale effects. Some common options are listed below.

## Option #1. Running the game with Python 3

* On the command line, navigate to the folder containing "index.html". Then type:

```
python -m http.server
```

* In the browser, navigate to:

```
localhost:8000
```

* Be sure to zoom out if you cannot see the whole map

## Option #2. Running the game with Visual Studio

1. File -> Open -> Web Site... Choose the folder containing index.html
2. Press F5 to run in debug mode

## Option #3. Running the game with the grayscale function disabled

Alternatively, at the top of the "Frogger.js" file there is: `Frogger._enable_grayscale = true;`

Changing this to "false" will disable the grayscale function, and allow the code to work by just double clicking on the index.html.

# Game Instructions (*if you tried the game but got confused*)

## Character Selection Screen

Use the arrow keys to highlight a character and press ENTER to select the character

## Main Game

### Controls

Use arrow keys to move your selected character.

### Goal

The goal is to reach the house tile on the other side of the map.

### Rules for the human characters

* Colliding with bugs, cars, or fire ball will decrease health points (HP). When HP falls to zero, the character will die.
* Falling into the water at the other side of the map is instant death.

### Modified rules for the dinosaur

* The dinosaur is very slow and will get hit by all kinds of things.
* The dinosaur will eat any bug it encounters and gain health points.
* The dinosaur will blow up any car that crash into him and take reduced damage.

### Modified rules for the UFO
* The UFO flies over the bugs and cars, ignoring them completely.
* The UFO will collide with the dragon though, since they are both fliers.
* The dragon's fireball will hit both air and ground units, so the UFO has to avoid fireballs as well.

### Levels

The game has four levels.

* Level 1 \- show case all enemies, but they are few in number
* Level 2 \- bugs
* Level 3 \- cars
* Level 4 \- cars and dragons

### Likely Outcome

The game is not balanced and only the UFO has a good chance of passing all four levels.
The human will most likely die on level 3. This character cannot out run cars, and to pass level 3, the human has to be patient. Sit back and wait for the traffic to have a gap that runs from upper left to the lower right. This allows the human to move forward and avoid the cars.
The dinosaur moves too slowly and will be overwhelmed by enemies on level 4.
The UFO can usually pass level 4 – unless if the dragons are stacked on two adjacent rows. In that case, the stream of dragons, plus their fireballs, can make those two rows impassable for the UFO. Just run over the two rows and hope for the best. It's only a game.
















# Files

### docs\
design - design documentation

### images\

None of the artwork is drawn by me.

* picture_sources.txt - describes where some of the pictures come from.
* all remaining artworks are supplied by Udacity.
