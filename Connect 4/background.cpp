#include <SFML/Graphics.hpp>
#include <string>

class Background {
public:
    Background(const std::string& ,"img.png") {
        if (!texture.loadFromFile("img.png")) {
            throw std::runtime_error("Failed to load background image");
        }
        sprite.setTexture(texture);
    }

    void draw(sf::RenderWindow& window) {
        // Stretch the background to cover the full window
        sf::FloatRect visibleArea = window.getView().getViewport();
        sprite.setScale(
            window.getSize().x / texture.getSize().x,
            window.getSize().y / texture.getSize().y
        );
        window.draw(sprite);
    }

private:
    sf::Texture texture;
    sf::Sprite sprite;
};
