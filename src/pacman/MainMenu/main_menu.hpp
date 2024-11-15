#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "../WindowTitle/window_title.hpp"
#include "MenuAnimation/menu_animation.hpp"
#include "MenuEntry/menu_entry.hpp"

class MainMenu {
 public:
  MainMenu();
  ~MainMenu();

  void show();

 private:
  // ---------------- MEMBERS ---------------- //
  sf::RenderWindow *window;
  // A pacman animation that will be displayed to the left of the title.
  MenuAnimation *animation;
  sf::Sound *press_start, *okey;  // Sound effects.
  sf::Color background_color;
  WindowTitle *title;
  MenuEntry *options[4];  // A list of all the menu entries.
  int selection_index;    // The currently selected menu entry.

  // ---------------- METHODS ---------------- //
  // Listen for events on the RenderWindow.
  void pollEvents();

  // Update all UI elements to reflect state changes.
  void update();

  // Update the window to reflect state changes.
  void render() const;

  // Returns whether or not the RenderWindow is open, i.e. if
  // the menu is currently open.
  bool isRunning() const {
    if (window == nullptr) return false;
    return window->isOpen();
  }

  void initWindow();
  void initText();
  void initSounds();

};  // MainMenu

#endif  // MAIN_MENU_HPP