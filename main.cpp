#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

int main() {
  double sprite_pos_x, sprite_pos_y = 0;

  // Create the main window
  sf::RenderWindow window(sf::VideoMode({800, 600}), "sfml_win");

  // Load a sprite to display
  const sf::Texture texture("sprite.jpg");
  sf::Sprite sprite(texture);

  // Start the game loop
  while (window.isOpen()) {
    // Process events
    while (const std::optional event = window.pollEvent()) {
      // Close window: exit
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    // Clear screen
    window.clear(sf::Color(50, 50, 255, 255));

    // Draw the sprite
    window.draw(sprite);
    sprite.setPosition({static_cast<float>(sprite_pos_x), static_cast<float>(sprite_pos_y)});
    // Update the window
    window.display();
  }
}
