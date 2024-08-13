//best code
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp> // Include SFML audio module
#include<vector>
#include<string>
#include<iostream>

const int CELL_SIZE=100;
const int ROWS=6;
const int COLS=7;
const sf::Vector2f BALL_SIZE(80,80);

enum class Player
{ None, Red, Yellow };
enum class GameState
{ Menu, Playing, GameOver, Paused, help };

// Custom colors
const sf::Color Black(0,0,0);
const sf::Color Blue(0,0,250);
const sf::Color Orange(255,165,0);
const sf::Color lilac(200,162,200);

void drawBoard(sf::RenderWindow& window, const std::vector<std::vector<Player>>& board, sf::Vector2f boardPosition, sf::CircleShape& currentBall);
bool checkWin(const std::vector<std::vector<Player>>& board, Player currentPlayer);
std::vector<std::string> wrapText(const std::string& text, unsigned int maxWidth, sf::Font& font, unsigned int charSize);
bool isBoardFull(const std::vector<std::vector<Player>>& board);
void exitGame(sf::RenderWindow& window);
void resetBoard(std::vector<std::vector<Player>>& board);

int main()
{
     sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Connect 4", sf::Style::Fullscreen);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("backgroundab1.jpg"))
    {
        // Handle loading error
        return -1;
    }

    sf::Sprite backgroundSprite(backgroundTexture);
    // Scale the background to fit the entire window
    backgroundSprite.setScale(
        float(window.getSize().x) / backgroundTexture.getSize().x,
        float(window.getSize().y) / backgroundTexture.getSize().y
    );

    // Load and play background music
    sf::Music backgroundMusic;
    if (!backgroundMusic.openFromFile("background music.ogg"))
    {
        return -1; // Music file failed to load
    }
    backgroundMusic.setLoop(true);
    backgroundMusic.play();

    sf::Font font;
    if (!font.loadFromFile("times new roman.ttf"))
    {
        return -1;
    }

    std::vector<std::string> bulletPoints =
    {
        "By clicking arrow you can move ball right left.\n",
        "When You pressed Down Arrow, Ball will Fall exactly in  same column where it was in up the Board .\n",
        "To win Game ,Player Have to Align their respective Color Balls in 4 that maybe Vertically, Horizontally or Diagonally.\n",
        "If no player aligns balls in 4 , then game will end in Draw.\n"
        "After Game , Game End Screen is Shown where Player can Exit or Restart Game Again.\n"
    };

    unsigned int charSize = 30;
    unsigned int maxWidth = 780; // Maximum width for each line
    sf::Color textColor = sf::Color::White; // Set your desired text color
    std::vector<sf::Text> texts;
    float yPosition = 300;
    float bulletIndent = 150;

    // Create text objects for each bullet point
    for (const auto& point : bulletPoints)
    {
        std::string bulletPoint = "• " + point; // Add bullet character
        std::vector<std::string> lines = wrapText(bulletPoint, maxWidth, font, charSize);

        for (const auto& line : lines)
        {
            sf::Text text(line, font, charSize);
            text.setFillColor(sf::Color::White); // Set text color
            text.setPosition(bulletIndent, yPosition);
            texts.push_back(text);
            yPosition += text.getLocalBounds().height + 5; // Move to next line
        }
    }


    //banner text
    sf::Text bannerText("Connect 4", font, 300);
    bannerText.setFillColor(sf::Color::Blue);
    bannerText.setStyle(sf::Text::Bold);
    bannerText.setPosition(window.getSize().x / 2 - bannerText.getGlobalBounds().width / 2, 20);

    sf::Text pbannerText("Connect 4", font, 300);
    pbannerText.setFillColor(sf::Color::Yellow);
    pbannerText.setStyle(sf::Text::Bold);
    pbannerText.setPosition(window.getSize().x / 2 - pbannerText.getGlobalBounds().width / 2, 20);

    sf::Text ebannerText("Connect 4", font, 300);
    ebannerText.setFillColor(sf::Color::Red);
    ebannerText.setStyle(sf::Text::Bold);
    ebannerText.setPosition(window.getSize().x / 2 - ebannerText.getGlobalBounds().width / 2, 20);


    // Menu Buttons
    sf::RectangleShape newGameButton(sf::Vector2f(250, 60));
    newGameButton.setFillColor(sf::Color::Blue);
    newGameButton.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2 - 75);
    sf::Text newGameText("New Game", font, 35);
    newGameText.setStyle(sf::Text::Bold);
    newGameText.setFillColor(sf::Color::White);
    newGameText.setPosition(window.getSize().x / 2 - 60, window.getSize().y / 2 - 70);

    sf::RectangleShape menuExitButton(sf::Vector2f(250, 60));
    menuExitButton.setFillColor(sf::Color::Red);
    menuExitButton.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2);
    sf::Text menuExitText("Exit Game", font, 35);
    menuExitText.setStyle(sf::Text::Bold);
    menuExitText.setFillColor(sf::Color::Black);
    menuExitText.setPosition(window.getSize().x / 2 - 60, window.getSize().y / 2 + 5);

    sf::RectangleShape howtoplayButton(sf::Vector2f(250, 60));
    howtoplayButton.setFillColor(sf::Color::Yellow);
    howtoplayButton.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2 +75);
    sf::Text howtoplayText("Help ?", font, 35);
    howtoplayText.setStyle(sf::Text::Bold);
    howtoplayText.setFillColor(sf::Color::Black);
    howtoplayText.setPosition(window.getSize().x / 2 - 60, window.getSize().y / 2 +75);

    //helpexit
    sf::RectangleShape helpExitButton(sf::Vector2f(400, 50));
    helpExitButton.setFillColor(sf::Color::Red);
    helpExitButton.setPosition(window.getSize().x - 180, 10);
    sf::Text helpExitText("Main Menu", font, 30);
    helpExitText.setStyle(sf::Text::Bold);
    helpExitText.setFillColor(sf::Color::Black);
    helpExitText.setPosition(window.getSize().x - 170, 15);

    // In-Game Exit Button
    sf::RectangleShape inGameExitButton(sf::Vector2f(200, 50));
    inGameExitButton.setFillColor(sf::Color::Red);
    inGameExitButton.setPosition(window.getSize().x - 160, 10);
    sf::Text inGameExitText("Exit", font, 30);
    inGameExitText.setStyle(sf::Text::Bold);
    inGameExitText.setFillColor(sf::Color::Black);
    inGameExitText.setPosition(window.getSize().x - 120, 15);

    sf::RectangleShape pauseButton(sf::Vector2f(150, 50));
    pauseButton.setFillColor(sf::Color::Yellow);
    pauseButton.setPosition(10, 10);
    sf::Text pauseText("Pause", font, 30);
    pauseText.setStyle(sf::Text::Bold);
    pauseText.setFillColor(sf::Color::Black);
    pauseText.setPosition(20, 15);

    sf::RectangleShape resumeButton(sf::Vector2f(200, 50));
    resumeButton.setFillColor(sf::Color::Blue);
    resumeButton.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2);
    sf::Text resumeText("Resume", font, 30);
    resumeText.setStyle(sf::Text::Bold);
    resumeText.setFillColor(sf::Color::White);
    resumeText.setPosition(window.getSize().x / 2 - 45, window.getSize().y / 2 + 5);


    //mute & unmute
    sf::RectangleShape muteButton(sf::Vector2f(200, 50));
    muteButton.setFillColor(sf::Color::Red);
    muteButton.setPosition(window.getSize().x - 160,window.getSize().y-80 );
    sf::Text muteText("Mute", font, 30);
    muteText.setStyle(sf::Text::Bold);
    muteText.setFillColor(sf::Color::Black);
    muteText.setPosition(window.getSize().x - 120,window.getSize().y-80 );

    sf::RectangleShape unmuteButton(sf::Vector2f(200, 50));
    unmuteButton.setFillColor(sf::Color::Blue);
    unmuteButton.setPosition(10, window.getSize().y-80 );
    sf::Text unmuteText("Unmute", font, 30);
    unmuteText.setStyle(sf::Text::Bold);
    unmuteText.setFillColor(sf::Color::White);
    unmuteText.setPosition(20, window.getSize().y-80 );


    sf::RectangleShape restartButton(sf::Vector2f(250, 60));
    restartButton.setFillColor(sf::Color::Blue);
    restartButton.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2 + 70);
    sf::Text restartText("Restart", font, 30);
    restartText.setStyle(sf::Text::Bold);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition(window.getSize().x / 2 - 50, window.getSize().y / 2 + 70);

    //exit game end screen
    sf::RectangleShape endExitButton(sf::Vector2f(250, 60));
    endExitButton.setFillColor(sf::Color::Red);
    endExitButton.setPosition(window.getSize().x - 160, 10);
    sf::Text endExitText("Exit", font, 30);
    endExitText.setStyle(sf::Text::Bold);
    endExitText.setFillColor(sf::Color::White);
    endExitText.setPosition(window.getSize().x - 140, 10);

    sf::Text winnerText("", font, 50);
    winnerText.setStyle(sf::Text::Bold);

    sf::Text turnText("", font, 50);
    turnText.setStyle(sf::Text::Bold);

    sf::Text pauseScreenText("CONNECT 4 IS PAUSED", font, 55);
    pauseScreenText.setStyle(sf::Text::Bold);
    pauseScreenText.setFillColor(sf::Color::Red);
    pauseScreenText.setPosition(window.getSize().x / 2 - pauseScreenText.getGlobalBounds().width / 2, window.getSize().y / 2 - 100);

    std::vector<std::vector<Player>> board(ROWS, std::vector<Player>(COLS, Player::None));
    Player currentPlayer = Player::Red;
    GameState gameState = GameState::Menu;

    bool showEndScreen = false;
    int currentCol = 0;
    bool pieceDropped = false;

    sf::CircleShape currentBall(CELL_SIZE / 2.5f);
    currentBall.setFillColor(currentPlayer == Player::Red ? sf::Color::Red : sf::Color::Yellow);
    currentBall.setPosition(window.getSize().x / 2 - BALL_SIZE.x / 2, 10);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (gameState == GameState::Menu)
            {
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (newGameButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        gameState = GameState::Playing;
                    }
                    else if (menuExitButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        exitGame(window);
                    }
                    else if (muteButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        backgroundMusic.stop();
                    }
                    else if (unmuteButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        backgroundMusic.setLoop(true);
                        backgroundMusic.play();
                    }
                    else if (howtoplayButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        gameState = GameState::help;
                    }
                }
            }
            else if (gameState == GameState::help)
            {
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (helpExitButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        gameState = GameState::Menu;
                    }
                    else if (muteButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        backgroundMusic.stop();
                    }
                    else if (unmuteButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        backgroundMusic.setLoop(true);
                        backgroundMusic.play();
                    }
                }
            }
            else if (gameState == GameState::Playing)
            {
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (inGameExitButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        exitGame(window);
                    }
                    else if (pauseButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        gameState = GameState::Paused;
                    }
                    else if (muteButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        backgroundMusic.stop();
                    }
                    else if (unmuteButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        backgroundMusic.setLoop(true);
                        backgroundMusic.play();
                    }

                }

                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Left)
                    {
                        if (currentCol > 0) currentCol--;
                        currentBall.setPosition(window.getSize().x / 2 - BALL_SIZE.x / 2 + (currentCol - COLS / 2) * CELL_SIZE, 10);
                        pieceDropped = false; // Reset pieceDropped when moving the ball
                    }
                    if (event.key.code == sf::Keyboard::Right)
                    {
                        if (currentCol < COLS - 1) currentCol++;
                        currentBall.setPosition(window.getSize().x / 2 - BALL_SIZE.x / 2 + (currentCol - COLS / 2) * CELL_SIZE, 10);
                        pieceDropped = false; // Reset pieceDropped when moving the ball
                    }
                    if (event.key.code == sf::Keyboard::Down)
                    {
                        if (!pieceDropped)
                        {
                            for (int row = ROWS - 1; row >= 0; --row)
                            {
                                if (board[row][currentCol] == Player::None)
                                {
                                    board[row][currentCol] = currentPlayer;

                                    pieceDropped = true;
                                    if (checkWin(board, currentPlayer))
                                    {
                                        gameState = GameState::GameOver;
                                        showEndScreen = true;
                                        winnerText.setString((currentPlayer == Player::Red ? "Red Player Wins!" : "Yellow Player Wins!"));
                                        winnerText.setFillColor(currentPlayer == Player::Red ? sf::Color::Red : sf::Color::Yellow);
                                        ebannerText.setFillColor(currentPlayer == Player::Red ? sf::Color::Red : sf::Color::Yellow);
                                        winnerText.setPosition(window.getSize().x / 2 - winnerText.getGlobalBounds().width / 2, window.getSize().y / 2 - 100);
                                        restartButton.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2 + 70);
                                        restartText.setPosition(window.getSize().x / 2 - 50, window.getSize().y / 2 + 85);
                                        endExitButton.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2 + 150);
                                        endExitText.setPosition(window.getSize().x / 2 - 50, window.getSize().y / 2 + 165);
                                    }
                                    // Check for a draw
                                    else if (isBoardFull(board))
                                    {
                                        gameState = GameState::GameOver;
                                        showEndScreen = true;
                                        winnerText.setString("It's a Draw!");
                                        winnerText.setFillColor(sf::Color::Blue);
                                        winnerText.setPosition(window.getSize().x / 2 - winnerText.getGlobalBounds().width / 2, window.getSize().y / 2 - 100);
                                    }
                                    currentPlayer = (currentPlayer == Player::Red) ? Player::Yellow : Player::Red;
                                    turnText.setString((currentPlayer == Player::Red ? "Red Player Turn!" : "Yellow Player Turn!"));
                                    turnText.setFillColor(currentPlayer == Player::Red ? sf::Color::Red : sf::Color::Yellow);
                                    turnText.setPosition(window.getSize().x / 2 - turnText.getGlobalBounds().width / 2, window.getSize().y-80 );
                                    currentBall.setFillColor(currentPlayer == Player::Red ? sf::Color::Red : sf::Color::Yellow);
                                    pieceDropped = false; // Reset pieceDropped for next turn
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            else if (gameState == GameState::Paused)
            {
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (resumeButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        gameState = GameState::Playing;
                        pauseText.setString("Pause");
                    }
                    else if (inGameExitButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        exitGame(window);
                    }
                }

            }
            else if (gameState == GameState::GameOver)
            {
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (restartButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        resetBoard(board);
                        currentPlayer = Player::Red;
                        currentBall.setFillColor(sf::Color::Red);
                        gameState = GameState::Playing;
                        showEndScreen = false;
                    }
                    else if (endExitButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        exitGame(window);
                    }
                }
            }
        }
        window.clear();
        // Draw the background image first
        window.draw(backgroundSprite);


        //window.clear(sf::Color::Black);

        if (gameState == GameState::Menu)
        {
            window.draw(bannerText);
            window.draw(newGameButton);
            window.draw(newGameText);
            window.draw(menuExitButton);
            window.draw(menuExitText);
            window.draw(howtoplayButton);
            window.draw(howtoplayText);
            window.draw(muteButton);
            window.draw(muteText);
            window.draw(unmuteButton);
            window.draw(unmuteText);
        }
        else if (gameState == GameState::help)
        {
            for (const auto& text : texts)
            {
                window.draw(text);
            }
            window.draw(helpExitButton);
            window.draw(helpExitText);
            window.draw(muteButton);
            window.draw(muteText);
            window.draw(unmuteButton);
            window.draw(unmuteText);
        }
        else if (gameState == GameState::Playing)
        {
            drawBoard(window, board, sf::Vector2f(window.getSize().x / 2 - COLS * CELL_SIZE / 2, 200), currentBall);
            window.draw(currentBall);
            window.draw(turnText);
            window.draw(pauseButton);
            window.draw(muteButton);
            window.draw(muteText);
            window.draw(unmuteButton);
            window.draw(unmuteText);
            window.draw(inGameExitButton);
            window.draw(inGameExitText);
            window.draw(pauseText);
            if (showEndScreen)
            {
                window.draw(ebannerText);
                window.draw(winnerText);
                window.draw(restartButton);
                window.draw(restartText);
                window.draw(endExitButton);
                window.draw(endExitText);
            }
        }
        else if (gameState == GameState::Paused)
        {
            window.draw(pbannerText);
            window.draw(pauseButton);
            window.draw(pauseText);
            window.draw(pauseScreenText); // Always draw pause screen text
            window.draw(resumeButton);
            window.draw(resumeText);
            window.draw(inGameExitButton);
            window.draw(inGameExitText);
        }
        else if (gameState == GameState::GameOver)
        {
            window.draw(ebannerText);
            window.draw(winnerText);
            window.draw(restartButton);
            window.draw(restartText);
            window.draw(endExitButton);
            window.draw(endExitText);
        }
        window.display();

    }
    return 0;
}


void drawBoard(sf::RenderWindow& window, const std::vector<std::vector<Player>>& board, sf::Vector2f boardPosition, sf::CircleShape& currentBall)
{
    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    cell.setFillColor(Blue);
    cell.setOutlineColor(Blue);
    cell.setOutlineThickness(5);

    sf::CircleShape hole(CELL_SIZE / 2.5f);
    hole.setFillColor(sf::Color::Black);

    for (int r = 0; r < ROWS; ++r)
    {
        for (int c = 0; c < COLS; ++c)
        {
            cell.setPosition(boardPosition.x + c * CELL_SIZE, boardPosition.y + r * CELL_SIZE);
            window.draw(cell);

            hole.setPosition(boardPosition.x + c * CELL_SIZE + CELL_SIZE / 2 - hole.getRadius(), boardPosition.y + r * CELL_SIZE + CELL_SIZE / 2 - hole.getRadius());
            window.draw(hole);

            if (board[r][c] != Player::None)
            {
                sf::CircleShape piece(CELL_SIZE / 2.5f - 5);
                piece.setFillColor(board[r][c] == Player::Red ? sf::Color::Red : sf::Color::Yellow);
                piece.setPosition(boardPosition.x + c * CELL_SIZE + CELL_SIZE / 2 - piece.getRadius(), boardPosition.y + r * CELL_SIZE + CELL_SIZE / 2 - piece.getRadius());
                window.draw(piece);
            }
        }
    }
}

bool checkWin(const std::vector<std::vector<Player>>& board, Player currentPlayer)
{
    // Check horizontal, vertical, and diagonal lines for a win
    for (int r = 0; r < ROWS; ++r)
    {
        for (int c = 0; c < COLS; ++c)
        {
            if (board[r][c] == currentPlayer)
            {
                // Check horizontally
                if (c + 3 < COLS &&
                        board[r][c + 1] == currentPlayer &&
                        board[r][c + 2] == currentPlayer &&
                        board[r][c + 3] == currentPlayer)
                {
                    return true;
                }
                // Check vertically
                if (r + 3 < ROWS &&
                        board[r + 1][c] == currentPlayer &&
                        board[r + 2][c] == currentPlayer &&
                        board[r + 3][c] == currentPlayer)
                {
                    return true;
                }
                // Check diagonal (top-left to bottom-right)
                if (r + 3 < ROWS && c + 3 < COLS &&
                        board[r + 1][c + 1] == currentPlayer &&
                        board[r + 2][c + 2] == currentPlayer &&
                        board[r + 3][c + 3] == currentPlayer)
                {
                    return true;
                }
                // Check diagonal (bottom-left to top-right)
                if (r - 3 >= 0 && c + 3 < COLS &&
                        board[r - 1][c + 1] == currentPlayer &&
                        board[r - 2][c + 2] == currentPlayer &&
                        board[r - 3][c + 3] == currentPlayer)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool isBoardFull(const std::vector<std::vector<Player>>& board)
{
    for (const auto& row : board)
    {
        for (Player cell : row)
        {
            if (cell == Player::None)
            {
                return false;
            }
        }
    }
    return true;
}

std::vector<std::string> wrapText(const std::string& text, unsigned int maxWidth, sf::Font& font, unsigned int charSize)
{
    std::vector<std::string> lines;
    std::string line;
    sf::Text tempText("", font, charSize);

    for (char c : text)
    {
        line += c;
        tempText.setString(line);
        if (tempText.getLocalBounds().width > maxWidth)
        {
            line.pop_back(); // Remove last character (overflowed)
            lines.push_back(line); // Add line to vector
            line = c; // Start new line with the character that caused the overflow
        }
    }

    if (!line.empty())
    {
        lines.push_back(line); // Add the last line
    }

    return lines;
}


void exitGame(sf::RenderWindow& window)
{
    window.close();
}

void resetBoard(std::vector<std::vector<Player>>& board)
{
    for (int r = 0; r < ROWS; ++r)
    {
        for (int c = 0; c < COLS; ++c)
        {
            board[r][c] = Player::None;
        }
    }
}
