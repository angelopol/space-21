// about_window.hpp
#ifndef ABOUT_WINDOW_HPP
#define ABOUT_WINDOW_HPP

#include <SFML/Graphics.hpp>

class AboutWindow {
 public:
  AboutWindow();
  void show();

 private:
  void initWindow();
  void render() const;

  sf::RenderWindow *window;
  sf::Text text;
  sf::Font font;
};

#endif  // ABOUT_WINDOW_HPP