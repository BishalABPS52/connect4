// button.cpp
#include <SFML/Graphics.hpp>
#include <iostream>

class Button {
private:
    sf::RectangleShape button;
    sf::Text buttonText;
    sf::Font font;

public:
    Button(float x, float y, float width, float height, const std::string& text) {
        button.setSize(sf::Vector2f(width, height));
        button.setPosition(x, y);
        button.setFillColor(sf::Color::Green);

        if (!font.loadFromFile("path_to_font.ttf")) {
            // Handle error
        }

        buttonText.setFont(font);
        buttonText.setString(text);
        buttonText.setCharacterSize(24);
        buttonText.setFillColor(sf::Color::White);
        buttonText.setPosition(
            x + (width - buttonText.getGlobalBounds().width) / 2,
            y + (height - buttonText.getGlobalBounds().height) / 2
        );
    }

    void render(sf::RenderWindow& window) {
        window.draw(button);
        window.draw(buttonText);
    }

    bool isClicked(const sf::Vector2i& mousePos) {
        return button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    }
};

// Define the function that will be triggered
void printMessage() {
    std::cout << "How To Play?" << std::endl;
    std::cout << "1. By clicking arrow you can move ball right left." << std::endl;
    std::cout << "2. When You pressed Down Arrow, Ball will Fall exactly i same column where it was in up the Board " << std::endl;
    std::cout << "3. To win Game ,Player Have to Align their respective Color Balls in 4 that maybe Vertically, Horizontally or Diagonally" << std::endl;
     std::cout << "4. If no player aligns balls in 4 , then game will end in Draw" << std::endl;
      std::cout << "5. After Game , Game End Screen is Shown where Player can Exit or Restart Game Again." << std::endl;
}
