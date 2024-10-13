// about_window.cpp
#include "about_window.hpp"

AboutWindow::AboutWindow() {
  initWindow();

  if (!font.loadFromFile("res/fonts/BlackFuture.otf")) {
    // Handle error
    std::cerr << "Error loading font BlackFuture.otf" << std::endl;
  }

  text.setFont(font);
  text.setString(
      "Objetivo:\n\n"
      " -   Comer todos los puntos pequeños del laberinto.\n"
      " -   Evitar ser capturado por los robots.\n\n"
      "Mecánica del juego:\n\n"
      " -   Movimiento: Bob se mueve por el laberinto usando las teclas de\n"
      " dirección; mientras que N-011 se mueve con las teclas (A, S, D, W).\n"
      " Los robots tienen patrones de movimiento predefinidos.\n"
      " Izquierda: A\n"
      " Arriba: W\n"
      " Abajo: S\n"
      " Derecha: D\n"
      " -   Comer puntos: Al comer puntos, suma puntos.\n"
      " -   Descomponer a los robots: Cuando alguno de los experimentos come un\n"
      " punto grande, cambia de color y puede descomponer a los robots. Los\n"
      " robots se vuelven azules y tienen un patrón de movimiento distinto. Si\n"
      " B-255 o N-011 toca a un robot normal, pierde una vida.\n"
      " -   Vidas: los experimentos tienen un número limitado de vidas. Al ser\n"
      " capturado por un robot, pierde una vida. Si se quedan sin vidas, el\n"
      " juego termina.");
  text.setCharacterSize(20);
  text.setFillColor(sf::Color::White);
  text.setPosition({20.f, 20.f});
}

void AboutWindow::initWindow() {
  const sf::VideoMode videoMode = sf::VideoMode(850, 600);
  const sf::Uint32 style = sf::Style::Close | sf::Style::Titlebar;

  window = new sf::RenderWindow(videoMode, "About", style);
  window->setFramerateLimit(60);
  window->setVerticalSyncEnabled(true);

  // Load and set the window icon.
  sf::Image icon;
  if (!icon.loadFromFile("res/icons/icono.png")) {
    std::cerr << "Error loading window icon" << std::endl;
  } else {
    window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
  }
}

void AboutWindow::render() const {
  window->clear(sf::Color(8, 9, 32));
  window->draw(text);
  window->display();
}

void AboutWindow::show() {
  while (window->isOpen()) {
    sf::Event event;
    while (window->pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window->close();
      }
    }
    render();
  }
  delete window;
}