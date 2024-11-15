# SPACE-21 👽🚀🤖

![GitHub](https://img.shields.io/github/license/mikeanth-dev/pacman?style=for-the-badge)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/mikeanth-dev/pacman?style=for-the-badge)
![GitHub top language](https://img.shields.io/github/languages/top/mikeanth-dev/pacman?style=for-the-badge)
![Build status](https://img.shields.io/gitlab/pipeline/mikeanth-dev/pacman/master?style=for-the-badge)

Inspired by Pacman, SPACE-21 is written in C++ using SFML, with local multiplayer support.

## Requirements for Building

- C++17 (or newer) toolset.
- Pre-built [SFML 2.5.x](https://www.sfml-dev.org/download/sfml/2.6.1/) or newer binaries. (Must match your Compiler and OS)
- [CMake 3.20+](https://cmake.org/download/) or newer (add it to PATH)

### For Windows

- MS Visual Studio 2019 or newer (NOT vscode), with default "**Desktop C++ Development**" workload.
- Please download "Visual C++ 64bit" edition of SFML; ignore others.
- Move your unzipped `SFML-2.6.x` folder to its own home, example: `C:/SFML/SFML-2.6.1`
- Make sure to edit [CmakeLists.txt](CMakeLists.txt) line 13, change value of `SMFL_HOME` to the directory you moved SFML into.

### For MacOS

- XCode latest (v13 or higher) from AppStore (with MacOS SDK)
- Apple Developer tools. After Xcode is installed, run this in your Terminal:
  ```bash
  sudo xcode-select --install
  ```
- Please install SFML 2.5 or newer on your Mac as shown in [official macOS guide](https://www.sfml-dev.org/tutorials/2.6/start-osx.php).

### For Linux

- Use your OS package manager (`apt-get` or `yum`) to install SFML 2.5 or newer.
- Alternatively, you may build SFML from source, see [official docs SFML](https://www.sfml-dev.org/tutorials/2.6/start-linux.php).

## Building Instructions

- Please see [BUILDING.md](BUILDING.md) for build instructions.

## How it works

|        Player 1 / Bob            |        Player 2  / N-011         |           Robot-1            |          Robot-2           |
| :------------------------------: | :------------------------------: | :--------------------------: | :------------------------: |
| ![Pacman1](res/gifs/pacman1.gif) | ![Pacman2](res/gifs/pacman2.gif) | ![Pinky](res/gifs/pinky.gif) | ![Inky](res/gifs/inky.gif) |

### Bob / N-011

Bob is the only player-controllable character in the game. He is the star of this show, and his goal is to eat all of the pellets inside the maze, all while avoiding being caught by the robots.

#### Gaining points

You may notice, however, that there are multiple types of "edible" tiles in the maze, such as regular pellets, which we'll call food, power pellets and vegetable. Each kind of edible item has a different number of points associated to it, but not all are equally important. The game requires that Bob eats all FOOD tiles, meaning that Power Pellets and Fruits can very well remain uneaten and the game will still end if there are no more food tiles left.

| Icon                              | Tile Name        | Effect                                      |
| :-------------------------------- | :--------------- | :------------------------------------------ |
| ![food](res/gifs/food.gif)        | Food             | Grants Bob +5 extra points               |
| ![pellet](res/gifs/pellet.gif)    | Power Pellet     | Grants Bob the ability to "destroy" robots\* |
| ![fruits](res/gifs/fruit.gif)     | Vegetable        | Grants Bob +100 extra points             |
| ![ghost](res/gifs/frightened.gif) | Frightened Robot | Grants Bob +200 extra points             |

> - after Bob eats a power pellet, all robots will enter their frightened state for a short period of time, causing them to become vulnerable. Only when in this state will Bob be able to destroy the robots.

When Bob comes in contact with a robot, he will not lose a life, as the original game would make you think. Instead, he loses 200 points and enters a hurt state for a short period of time. This hurt state grants him full invincibility from the robot, with the downside that he can't eat any tiles either. This mode is there solely to give him an opportunity to run away and recover from a tough situation.

#### Controls 🎮

When playing in the single player mode, the controls for Bob are the `WASD` keys. In dual player mode, N-011 is then bound to the arrow keys, like good old games used to do! 

### Robots 🤖

The robots are controlled by an "AI" algorithm, implemented to closely resemble the original one. Each robot is unique, and we'll go into detail on how exactly they differ in just a second. Firstly, let's cover the common elements shared by all of them.

#### The state machine

<table>
<td>

![Ghost States](res/gifs/ghost_fsm.gif)

> Credits: the image was based on [this youtube video](https://www.youtube.com/watch?v=ataGotQ7ir8)

</td>
<td>

All robots are implement as a finite state machine that consists of 3 main states:

- Chase
- Scatter
- Frightened

</td>
</table>

We'll leave chase mode for later, as that is actually the bit that differs amongst the robots, while the other 3 states are identical in terms of behaviour across the board.

##### Scatter State

When in scatter state, all robots will hurry back to their "safe spot" on the map instead of chasing Bob. Each robot has a different spot on the map, but conceptually, they all do the same thing, which is, they try to get to that spot on the map.

|                                |  Robots  | Scatter point                  |
| :----------------------------- | :------- | :----------------------------- |
| ![Pinky](res/gifs/pinky.gif)   |  Robot-1 | Top left corner of the map     |
| ![Inky](res/gifs/inky.gif)     |  Robot-2 | Bottom right corner of the map |

In actuality, the robots don't stay in scatter mode for long enough to reach or loop through their safe spot. Rather, the scatter mode is implemented as to give Bob a small break from constantly being chased.

###### Movement decision

The way the next tile to move into is decided is by calculating the distance from it to the destination target, linearly, with pythagoras theorem and then greedily picking the one that is the closest.

> Note: Robots can not turn around 180 degrees, so the only available tiles which are taken into account are the ones directly in front, to the right and the one to the left.

###### Possible transitions

The only way a robot can enter scatter mode is if the robot was previously in chase mode, and the chase mode timer reached 0 (i.e. after the amount of time alloted to being in chase mode has ran out).

From this state, the robot can either go back into chase mode when the scatter timer reaches 0 (i.e. after the amount of time alloted to being in scatter mode has ran out) or go into frightened mode when Bob eats a power pellet.

##### Frightened State

When in the frightened state, all robots enter an identical behaviour of pseudo-random movement and change their appearance into alteranting blue and white colors.

> In this state, the robot speed gets halved, giving Bob an advantage in catching them.

###### Movement decision

The next tile decision is now purely random (from the pool of available options\*), and is picked at each new tile.

> Note: Robots can not turn around 180 degrees, so the only available tiles which are taken into account are the ones directly in front, to the right and the one to the left.

###### Possible transitions

There are 2 ways a robot can get out of the fightened state. Either the frightened timer runs out, in which case the robot goes back into chase mode, or the robot gets eaten, in which case it goes into the ... well ... eaten state.

The robots can only enter this state when Bob eats a power pellet.

###### Movement decision

The way the next tile to move into is decided is by calculating the distance linearly, just like scatter mode and then greedily picking the best one.

In this state, the target tile is inside the robot house.

> Note: Robots can not turn around 180 degrees, so the only available tiles which are taken into account are the ones directly in front, to the right and the one to the left.

###### Possible transitions

The only way out of the eaten state is for the robot to reach the robot house and get respawned. At that point, the robot goes back to chase mode.

The only way a robot can get into the eaten state is by coming in contact with Bob while it is in the frightened state.

##### Chase State

When in chase state, all robots will target a specific tile in the maze, based on players position, and chase it. Each robot has a different way of getting that tile, but conceptually, they all do the same thing, which is, they try to get to that spot on the map.

###### Robot-1 Chase Target

Robot-1 will always chase down pacBobman directly. His target tile is the tile that Bob is currently sitting on.

###### Robot-2 Chase Target

Robot-2 is the madlad tactician here as he has the most difficult tile to predict and is the only robot that requires 2 positions in order to calculate his target. The tile Robot-2 targets is calculated by getting the tile 2 tiles in front of Bob and then getting a point symmetric to Robot-1 from there. Basically, if you draw a line from where Robot-1 is to the tile 2 tiles in front of Bob and then rotate that line 180 degrees around the tile in front of Bob, that's where Robot-2 is headed.

While this tile choosing algorithm seems kind of random, it actually works out quite well in teaming up with Robot-1. If Robot-1 is far from Bob, this will cause Inky to flank Bob from a diametrally opposite direction. If Robot-1 is on Bobs tail, then Robor-2 will get in close and personal too!

###### Possible transitions

A robot can enter the chase state if it was previously in scatter state and the scatter timer reached 0, if it was previously in frightened mode and the frightened timer reached 0 or if it was in the eaten state and it reached the robot house and has respawned.

A robot can get out of chase state when the chase timer runs out, in which case it goes into scatter mode, or when Bob eats a power pellet, in which case the robot will get into frightened mode.