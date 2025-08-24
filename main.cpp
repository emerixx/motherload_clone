#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

double sprite_pos_x, sprite_pos_y = 0;
double sprite_vel_x, sprite_vel_y = 0;
double sprite_acc_x, sprite_acc_y = 0;
double drag_grounded = 0.97;
double drag_air = 0.99;
int input_acc_x_mult = 10;
int thruster_acc_mult = 7;
int ground_y = 500;
int g = 5;
bool grounded = 0;

// Create the main window
sf::RenderWindow window(sf::VideoMode({1920, 1080}), "sfml_win");
float deltaTime;
// Load a sprite to display
const sf::Texture texture_main_sprite("sprite.jpg");
const sf::Texture texture_bg("bg.png");
const sf::Texture texture_dirt("textures/png/dirt.png");
sf::Sprite sprite(texture_main_sprite);
sf::Sprite bg(texture_bg);
std::vector<std::vector<sf::Sprite>> blocks;

bool is_grounded() {
  if (sprite_pos_y < ground_y - 64) {
    return 0;
  }
  // check if theres a block at x: pos_x-32
  double i = std::round(sprite_pos_x / 64);
  if (std::abs(i - sprite_pos_x / 64) < 0.1) {
    // means the sprite is mostly on one block;
    if (blocks[0][i].getPosition().y == -1) {
      // block was deleted;
      return false;
    } else {
      return true;
    }
  } else {
    // have to check both blocks
    if (blocks[0][std::floor(sprite_pos_y / 64)].getPosition().y == -1 &&
        blocks[0][std::ceil(sprite_pos_y / 64)].getPosition().y == -1) {
      return false;
    } else {
      //problem here probably
      return true;
    }
  }
}

int main() {

  sf::Clock clock;
  for (int j = 0; j < 2; j++) {
    std::vector<sf::Sprite> sprvc;
    for (int i = 0; i < 30; i++) {
      sf::Sprite sprite(texture_dirt);
      sprite.setPosition({i * 64.f, static_cast<float>(ground_y + j * 64)});
      sprvc.push_back(sprite);
    }
    blocks.push_back(sprvc);
  }
  // Start the game loop
  while (window.isOpen()) {
    // Process events
    grounded = is_grounded();
    deltaTime = clock.restart().asSeconds();
    while (const std::optional event = window.pollEvent()) {
      // Close window: exit
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    // get some normal grounded checking :sob:

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
      sprite_acc_y = g * deltaTime;
    } else {
      sprite_acc_y = 0;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
      if (grounded) {
        // get block beneath me, delete it
        double i = std::round(sprite_pos_x / 64);
        blocks[0][i].setPosition({-100, -1});
      }
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
      sprite_vel_y *= drag_air;
    }
    // Clear screen
    window.clear();

    // Draw the sprite
    window.draw(bg);
    for (int j = 0; j < blocks.size(); j++) {
      for (int i = 0; i < blocks[j].size(); i++) {
        window.draw(blocks[j][i]);
      }
    }
    window.draw(sprite);
    sprite.setPosition(
        {static_cast<float>(sprite_pos_x), static_cast<float>(sprite_pos_y)});
    // Update the window
    window.display();
  }
}
