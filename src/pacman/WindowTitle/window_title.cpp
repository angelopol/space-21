#include "window_title.hpp"

WindowTitle::WindowTitle(const std::string &_content,
                         const sf::Vector2f &_position) {
  content = _content;
  text = sf::Text();

  if (!font.loadFromFile("res/fonts/BlackFuture.otf")) {
    // Handle error
    std::cerr << "Error loading font BlackFuture.otf" << std::endl;
  }

  text.setFont(font);
  text.setString(_content);
  text.setCharacterSize(40);
  text.setLetterSpacing(1.25);
  text.setFillColor(sf::Color(64,161,7));
  text.setPosition(_position);
}
WindowTitle::~WindowTitle() { /* nothing to do here */
}

void WindowTitle::render(sf::RenderTarget *_target) const {
  _target->draw(text);
}

void WindowTitle::update() { /* Nothing to do here */
}
