#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>
#include <vector>

int main() {
  double sprite_pos_x, sprite_pos_y = 0;
  double sprite_vel_x, sprite_vel_y = 0;
  double sprite_acc_x, sprite_acc_y = 0;
  double drag_grounded = 0.95;
  double drag_air = 0.99;
  int input_acc_x_mult = 10;
  int thruster_acc_mult = 10;
  int ground_y = 500;
  int g = 5;
  bool grounded = 0;

  // Create the main window
  sf::RenderWindow window(sf::VideoMode({1920, 1080}), "sfml_win");
  sf::Clock clock;
  float deltaTime;
  // Load a sprite to display
  const sf::Texture texture_main_sprite("sprite.jpg");
  const sf::Texture texture_bg("bg.png");
  const sf::Texture texture_dirt("textures/png/dirt.png");
  sf::Sprite sprite(texture_main_sprite);
  sf::Sprite bg(texture_bg);
  std::vector<sf::Sprite> dirt_blocks;

  for (int i = 0; i < 30; i++) {
    sf::Sprite sprite(texture_dirt);
    sprite.setPosition({i * 64.f, static_cast<float>(ground_y)});
    dirt_blocks.push_back(sprite);
  }
  std::cout << dirt_blocks.size() << std::endl;

  // Start the game loop
  while (window.isOpen()) {
    // Process events
    deltaTime = clock.restart().asSeconds();
    while (const std::optional event = window.pollEvent()) {
      // Close window: exit
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    // get some normal grounded checking :sob:
    if (sprite_pos_y < (ground_y - 64)) {
      grounded = 0;
    } else if (sprite_pos_y >= (ground_y - 64)) {
      grounded = 1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
      sprite_acc_x = -input_acc_x_mult * deltaTime;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
      sprite_acc_x = input_acc_x_mult * deltaTime;
    } else {
      sprite_acc_x = 0;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
      sprite_acc_y = -thruster_acc_mult * deltaTime;
    } else if (!grounded) {
      sprite_acc_y = 9.81 * deltaTime;
    } else {
      sprite_acc_y = 0;
    }

    if (grounded) {
      sprite_vel_y = 0;
      sprite_pos_y = ground_y - 64;
    }

    // assume dt is 1
    sprite_vel_x += sprite_acc_x;
    sprite_vel_y += sprite_acc_y;

    sprite_pos_x += sprite_vel_x;
    sprite_pos_y += sprite_vel_y;

    std::cout << "x: " << sprite_pos_x << "; y: " << sprite_pos_y << std::endl;
    if (grounded) {
      sprite_vel_x *= drag_grounded;

    } else {
      sprite_vel_x *= drag_air;
      sprite_vel_y*=drag_air;
    }
    // Clear screen
    window.clear(sf::Color(50, 50, 255, 255));

    // Draw the sprite
    window.draw(bg);
    for (int i = 0; i < dirt_blocks.size(); i++) {
      window.draw(dirt_blocks[i]);
    }
    window.draw(sprite);
    sprite.setPosition(
        {static_cast<float>(sprite_pos_x), static_cast<float>(sprite_pos_y)});
    // Update the window
    window.display();
  }
}
