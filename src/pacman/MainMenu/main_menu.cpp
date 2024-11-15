#include "main_menu.hpp"
#include "about_window.hpp"

MainMenu::MainMenu() {
  selection_index = 0;
  initWindow();
  initText();
  initSounds();
  animation = new MenuAnimation({200.f, 145.f});
  background_color = sf::Color(8, 9, 32);

  press_start->play();
}
MainMenu::~MainMenu() {
  // Delete the window.
  if (window != nullptr) delete window;

  // Delete window title.
  delete title;

  // Delete menu entries.
  for (MenuEntry *entry : options) delete entry;
}

void MainMenu::pollEvents() {
  sf::Event event;
  while (window->pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      exit(1);
    }

    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Up) {
        if (selection_index > 0) selection_index--;
      } else if (event.key.code == sf::Keyboard::Down) {
        if (selection_index < 3) selection_index++;
      } else if (event.key.code == sf::Keyboard::Enter ||
                 event.key.code == sf::Keyboard::Space) {
        okey->play();

        while (okey->getStatus() == sf::Sound::Status::Playing) {
          sf::sleep(sf::milliseconds(10));
        }

        switch (selection_index) {
          case 0:
            Config::getInstance()->selected_map =
                Config::getInstance()->maps["single-player"];
            window->close();
            break;
          case 1:
            Config::getInstance()->selected_map =
                Config::getInstance()->maps["dual-player"];
            window->close();
            break;
          case 2: {
            AboutWindow aboutWindow;
            aboutWindow.show();
            break;
          }
          case 3:
            exit(1);
        }
      }
    }
  }
}

void MainMenu::update() {
  options[selection_index]->setActive(false);
  pollEvents();
  options[selection_index]->setActive(true);

  title->update();
  for (MenuEntry *entry : options) entry->update();
  animation->update();
}

void MainMenu::render() const {
  window->clear(background_color);

  // Render menu UI.
  animation->render(window);
  title->render(window);
  for (auto &entry : options) entry->render(window);

  window->display();
}

void MainMenu::initWindow() {
  Config *config = Config::getInstance();
  // Create a 4:3 non-resizeable window.
  const sf::VideoMode videoMode = sf::VideoMode(800, 600);
  const sf::Uint32 style = sf::Style::Close | sf::Style::Titlebar;

  // Limit the frame-rate to 60fps and enable V-Sync.
  window = new sf::RenderWindow(videoMode, config->window_title, style);
  window->setFramerateLimit(config->window_framerate);
  window->setVerticalSyncEnabled(true);

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

void MainMenu::initText() {
  title = new WindowTitle(Config::getInstance()->window_title, {260.f, 135.f});
  options[0] = new MenuEntry("Start Singleplayer", {200.f, 260.f});
  options[1] = new MenuEntry("Start Multiplayer", {200.f, 300.f});
  options[2] = new MenuEntry("About", {200.f, 340.f});
  options[3] = new MenuEntry("Exit", {200.f, 380.f});
}

void MainMenu::initSounds() {
  Config *config = Config::getInstance();
  // Get a copy of the sound objects pointers.
  press_start = config->sounds["press_start"];
  okey = config->sounds["okey_dokey"];
}

void MainMenu::show() {
  while (isRunning()) {
    update();
    render();
  }
}
