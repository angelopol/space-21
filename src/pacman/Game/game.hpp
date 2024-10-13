#ifndef GAME_HPP
#define GAME_HPP

#include <fstream>

#include "../GameTile/__barrel__.hpp"
#include "../WindowTitle/window_title.hpp"
#include "../../ResourcePath.hpp"

class Game {
 public:
  Game();
  ~Game();

  void run();

 private:
  // ---------------- MEMBERS ---------------- //
  sf::RenderWindow *window;
  sf::Sound *ty_sound, *background_music;
  sf::Color background_color;
  sf::Text pause_text;
  sf::Text resume_text;
  sf::Text about_text;
  sf::Text main_menu_text;
  sf::RectangleShape pause_overlay;  // Nuevo rectángulo para opacidad
  vec3pGT map;
  WindowTitle *title, *game_over_title;
  bool game_over = false;
  bool is_paused = false;
  int selected_option = 0;
  std::vector<sf::Text> menuOptions;

  // ---------------- METHODS ---------------- //
  // Listen for events on the RenderWindow.
  void pollEvents();

  // Update all the game elements.
  // Game logic takes place here.
  void update();

  // Update the window by rendering new frames.
  void render();

  // Returns whether or not the RenderWindow is open, i.e. if
  // the game is currently running.
  bool isRunning() const;

  void initWindow();  // intialize the RenderWindow.
  void initMap();
  vector<vector<char>> readMap();
  void initTitles();
  void initSounds();
  void initPauseText();
};  // class Game

#endif  // GAME_HPP