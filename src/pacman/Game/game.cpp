#include "game.hpp"
#include "../MainMenu/main_menu.hpp"
#include "../MainMenu/about_window.cpp" 

Game::Game() {
  initSounds();
  background_music->play();
  background_color = sf::Color(8, 9, 32);

  initMap();
  initWindow();
  initPauseText();
}

Game::~Game() {
  // Play "Thank you" sound, and wait for it to finish before proceeding.
  ty_sound->play();
  while (ty_sound->getStatus() == sf::Sound::Status::Playing) {
    sf::sleep(sf::milliseconds(100.f));
  }

  // stop background music
  background_music->stop();

  // Delete the config instance.
  delete Config::getInstance();

  // Delete the window.
  if (window != nullptr) {
    delete window;
  }

  // Delete every tile on the map.
  for (vec2pGT &vec2 : map) {
    for (vec1pGT &vec1 : vec2) {
      for (GameTile *tile : vec1) {
        if (tile != nullptr) delete tile;
      }
    }
  }

  // Write all the collected metrics to a CSV file.
  // PERFLOGGER_COMMIT_METRICS();
}

void Game::pollEvents() {
  sf::Event event;

  while (window->pollEvent(event)) {
    // If the window "x" button was pressed, close the window.
    if (event.type == sf::Event::Closed) {
      window->close();
    }

    // Once the game is over, allow the user to exit on any key press.
    if (game_over && event.type == sf::Event::KeyPressed) {
      window->close();
    }

    // Toggle pause state when Escape is pressed
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape) {
      is_paused = !is_paused;
    }

    if (is_paused && event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Up) {
        selected_option = (selected_option - 1 + 3) % 3;  // Mover hacia arriba
      } else if (event.key.code == sf::Keyboard::Down) {
        selected_option = (selected_option + 1) % 3;  // Mover hacia abajo
      } else if (event.key.code == sf::Keyboard::Enter) {
        if (selected_option == 0) {
          is_paused = false;  // Reanudar el juego
        } else if (selected_option == 1) {
          AboutWindow aboutWindow;
          aboutWindow.show();  // Mostrar ventana "About"
        } else if (selected_option == 2) {
        window->close();
        }
      }
    }
  }
}

void Game::update() {
  pollEvents();
  if (!is_paused && !game_over) {
    // Call the update method on every map tile.
    for (vec2pGT &vec2 : map) {
      for (vec1pGT &vec1 : vec2) {
        for (GameTile *tile : vec1) {
          if (tile != nullptr) {
            tile->update(window, map);
          }
        }
      }
    }

    if (Food::counter <= 0) game_over = true;

  } else if (game_over) {
    sf::sleep(sf::milliseconds(200));
  }
}

void Game::render() {
  // Clear the old frame from the window.
  window->clear(background_color);

  title->render(window);  // Render the title at the top of the screen.
  // Render every game tile.
  for (const vec2pGT &vec2 : map) {
    for (const vec1pGT &vec1 : vec2) {
      for (GameTile *tile : vec1) {
        if (tile != nullptr) tile->render(window);
      }
    }
  }

  if (game_over) {
    game_over_title->render(window);
  } else if (is_paused) {
    window->draw(pause_overlay);  // Dibujar el rectángulo de opacidad
    window->draw(pause_text);     // Render the pause text

    if (selected_option == 0) {
      resume_text.setFillColor(
          sf::Color(194, 0, 194));  // Resaltar opción seleccionada
      about_text.setFillColor(sf::Color::White);
      main_menu_text.setFillColor(sf::Color::White);
    } else if (selected_option == 1) {
      resume_text.setFillColor(sf::Color::White);
      about_text.setFillColor(
          sf::Color(194, 0, 194));  // Resaltar opción seleccionada
      main_menu_text.setFillColor(sf::Color::White);
    } else if (selected_option == 2) {
      resume_text.setFillColor(sf::Color::White);
      about_text.setFillColor(sf::Color::White);
      main_menu_text.setFillColor(
          sf::Color(194, 0, 194));  // Resaltar opción seleccionada
    }

    window->draw(resume_text);     // Render the resume text
    window->draw(about_text);      // Render the about text
    window->draw(main_menu_text);  // Render the main menu text
  }

  // Display the newly rendered frame onto the window.
  window->display();
}

   

bool Game::isRunning() const {
  if (window == nullptr) return false;
  return window->isOpen();
}

void Game::initWindow() {
  Config *config = Config::getInstance();
  config->window_size.x =
      config->map_size.x * config->tile_size + config->offset.x * 2;
  config->window_size.y =
      config->map_size.y * config->tile_size + config->offset.y * 2;

  // Create a 4:3 non-resizeable window.
  const sf::VideoMode videoMode =
      sf::VideoMode(config->window_size.x, config->window_size.y);
  const sf::Uint32 style = sf::Style::Close | sf::Style::Titlebar;

  // Limit the frame-rate to 60fps and enable V-Sync.
  window = new sf::RenderWindow(videoMode, config->window_title, style);
  window->setFramerateLimit(config->window_framerate);
  window->setVerticalSyncEnabled(true);

  initTitles();

  // Load and set the window icon.
  sf::Image icon;
  if (!icon.loadFromFile("res/icons/icono.png")) {
    std::cerr << "Error loading window icon" << std::endl;
  } else {
    window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
  }

  // Show the window.
  window->setVisible(true);
}

void Game::initPauseText() {
  selected_option = 0;

  pause_text.setFont(*Config::getInstance()->font);
  pause_text.setString("Paused");
  pause_text.setCharacterSize(50);
  pause_text.setFillColor(sf::Color(194, 0, 194));
  pause_text.setPosition(
      window->getSize().x / 2 - pause_text.getGlobalBounds().width / 2,
      window->getSize().y / 2 - pause_text.getGlobalBounds().height / 2);

  resume_text.setFont(*Config::getInstance()->font);
  resume_text.setString("Resume");
  resume_text.setCharacterSize(30);
  resume_text.setFillColor(sf::Color::White);
  resume_text.setPosition(
      window->getSize().x / 2 - resume_text.getGlobalBounds().width / 2,
      window->getSize().y / 2 + pause_text.getGlobalBounds().height);

  about_text.setFont(*Config::getInstance()->font);
  about_text.setString("About");
  about_text.setCharacterSize(30);
  about_text.setFillColor(sf::Color::White);
  about_text.setPosition(
      window->getSize().x / 2 - about_text.getGlobalBounds().width / 2,
      (window->getSize().y / 2 + pause_text.getGlobalBounds().height) + 45);

  
  main_menu_text.setFont(*Config::getInstance()->font);
  main_menu_text.setString("Exit");
  main_menu_text.setCharacterSize(30);
  main_menu_text.setFillColor(sf::Color::White);
  main_menu_text.setPosition(
      window->getSize().x / 2 - main_menu_text.getGlobalBounds().width / 2,
      (window->getSize().y / 2 + pause_text.getGlobalBounds().height) + 90);

  // Inicializar el rectángulo de opacidad
  pause_overlay.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
  pause_overlay.setFillColor(sf::Color(0, 0, 0, 220));  // Color negro con 150 de opacidad
}

vector<vector<char>> Game::readMap() {
  std::ifstream file(pac::getResourcePath(Config::getInstance()->selected_map),
                     std::ios::binary);
  std::string line = "";
  unsigned long int line_count = 0, line_length = 0;
  vector<vector<char>> char_map = {};

  if (!file.is_open()) {
    std::cout << "ERROR: Cannot find map file" << std::endl;
    // abort();
  }

  while (std::getline(file, line)) {
    if (line.size() > line_length) line_length = line.size();

    vector<char> temp = {};
    for (long unsigned int i = 0; i < line.size(); i++) {
      temp.push_back(line[i]);
    }
    char_map.push_back(temp);
    line_count++;
  }

  // Set map sizes.
  Config::getInstance()->map_size.x = line_length;
  Config::getInstance()->map_size.y = line_count;

  return char_map;
}

void Game::initMap() {
  vector<vector<char>> char_map = readMap();
  std::vector<Ghost *> ghosts[2];
  Pacman *pacmans[2];

  int i = 0, j = 0;
  for (auto &line : char_map) {
    vec2pGT temp = {};

    for (char &c : line) {
      sf::Vector2f position = {
          i * Config::getInstance()->tile_size,
          j * Config::getInstance()->tile_size,
      };
      GameTile *tile = nullptr;

      if (std::string("123456!@#$%^").find(c) != std::string::npos) {
        tile = new Wall(position, c);
      } else if (c == '*') {
        tile = new Food(position);
      } else if (c == '0') {
        tile = new PowerPellet(position);
      } else if (c == 'f') {
        tile = new Fruit(position);
      } else {
        int index = islower(c) == 0 ? 1 : 0;
        if (c == 'O' || c == 'o') {
          tile = new Pacman(position);
          pacmans[index] = (Pacman *)tile;
        } else if (c == 'B' || c == 'b') {
          tile = new Blinky(position);
          ghosts[index].push_back((Ghost *)tile);
        } else if (c == 'C' || c == 'c') {
          tile = new Clyde(position);
          ghosts[index].push_back((Ghost *)tile);
        } else if (c == 'I' || c == 'i') {
          tile = new Inky(position);
          ghosts[index].push_back((Ghost *)tile);
        } else if (c == 'P' || c == 'p') {
          tile = new Pinky(position);
          ghosts[index].push_back((Ghost *)tile);
        }
      }
      temp.push_back({tile});
      i++;
    }
    j++;
    i = 0;
    map.push_back(temp);
  }

  for (int i = 0; i < 2; i++) {
    for (Ghost *g : ghosts[i]) {
      g->addChasing(pacmans[i]);

      if (g->getName() == "Inky") {
        for (Ghost *b : ghosts[i]) {
          if (b->getName() == "Blinky") {
            g->addChasing(b);
          }
        }
      }
    }
  }
}

void Game::initTitles() {
  Config *config = Config::getInstance();

  const sf::Vector2f title_position = {
      static_cast<float>(config->window_size.x) / 2 - 100,
      static_cast<float>(config->offset.y) / 2 - 15,
  };
  title = new WindowTitle(Config::getInstance()->window_title, title_position);

  const sf::Vector2f go_title_position = {
      static_cast<float>(config->window_size.x) / 2 - 135,
      static_cast<float>(config->window_size.y) / 2 - 40,
  };
  game_over_title = new WindowTitle("Game Over", go_title_position);
}

void Game::initSounds() {
  Config *config = Config::getInstance();

  // Get a copy of the sound objects pointers.
  ty_sound = config->sounds["thank_you"];
  background_music = config->sounds["background_music"];

  // Turn down the volume of the background track
  background_music->setVolume(50.f);
}

void Game::run() {
  while (isRunning()) {
    update();
    render();
  }
}
