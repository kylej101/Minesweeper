#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <random>
#include <chrono>
#include <SFML/Graphics.hpp>

int main() {
    // Reading config file
    std::ifstream inputFile("files/config.cfg");
    if (!inputFile.is_open()) {
        std::cout << "Error opening file" << std::endl;
        return 1;
    }
    int numCol;
    inputFile >> numCol;
    int numRow;
    inputFile >> numRow;
    int numMines;
    inputFile >> numMines;
    inputFile.close();

    // Start of Welcome Window
    sf::RenderWindow welcomeWindow(sf::VideoMode(numCol*32, (numRow*32)+100), "Minesweeper", sf::Style::Close);

    sf::Font font;
    font.loadFromFile("files/font.ttf");

    sf::Text welcomeText("WELCOME TO MINESWEEPER!", font, 24);
    welcomeText.setPosition(welcomeWindow.getSize().x / 2 - welcomeText.getLocalBounds().width / 2, welcomeWindow.getSize().y / 2 - 150);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    welcomeText.setFillColor(sf::Color::White);

    sf::Text nameText("Enter your name:", font, 20);
    nameText.setPosition(welcomeWindow.getSize().x / 2 - nameText.getLocalBounds().width / 2, welcomeWindow.getSize().y / 2 - 75);
    nameText.setStyle(sf::Text::Bold);
    nameText.setFillColor(sf::Color::White);

    sf::Text userInputText("", font, 18);
    userInputText.setPosition(welcomeWindow.getSize().x / 2, welcomeWindow.getSize().y / 2 - 45);
    userInputText.setStyle(sf::Text::Bold);
    userInputText.setFillColor(sf::Color::Yellow);

    sf::Text cursor("|", font, 18);
    cursor.setStyle(sf::Text::Bold);
    cursor.setFillColor(sf::Color::Yellow);
    cursor.setPosition(userInputText.getPosition().x + userInputText.getLocalBounds().width, userInputText.getPosition().y);


    while (welcomeWindow.isOpen()) {
        sf::Event event;
        while (welcomeWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                welcomeWindow.close();
                return 0;
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    if (userInputText.getString().getSize() > 0) {
                        welcomeWindow.close();
                    }
                }
            }
            else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode >= 'A' && event.text.unicode <= 'Z') {
                    char typedChar = static_cast<char>(event.text.unicode);
                    if (userInputText.getString().getSize() > 0) {
                        typedChar = std::tolower(typedChar);
                    }
                    userInputText.setString(userInputText.getString() + typedChar);
                }
                else if (event.text.unicode >= 'a' && event.text.unicode <= 'z' && userInputText.getString().getSize() < 10) {
                    char typedChar = static_cast<char>(event.text.unicode);
                    if (userInputText.getString().getSize() == 0) {
                        typedChar = std::toupper(typedChar);
                    }
                    userInputText.setString(userInputText.getString() + typedChar);
                }
                else if (event.text.unicode == '\b' && userInputText.getString().getSize() > 0) {
                    userInputText.setString(userInputText.getString().substring(0, userInputText.getString().getSize() - 1));
                }
            }
        }

        cursor.setPosition(userInputText.getPosition().x + userInputText.getLocalBounds().width, userInputText.getPosition().y);

        welcomeWindow.clear(sf::Color::Blue);
        welcomeWindow.draw(welcomeText);
        welcomeWindow.draw(nameText);
        welcomeWindow.draw(userInputText);
        welcomeWindow.draw(cursor);
        welcomeWindow.display();
    }

    // Start of Game Window
    sf::RenderWindow gameWindow(sf::VideoMode(numCol*32, (numRow*32)+100), "Minesweeper", sf::Style::Close);

    sf::Texture happyFaceTex;
    happyFaceTex.loadFromFile("files/images/face_happy.png");
    sf::Sprite happyFace;
    happyFace.setTexture(happyFaceTex);
    happyFace.setPosition(((numCol/2)*32)-32,32*(numRow+0.5));

    sf::Texture sadFaceTex;
    sadFaceTex.loadFromFile("files/images/face_lose.png");
    sf::Sprite sadFace;
    sadFace.setTexture(sadFaceTex);
    sadFace.setPosition(((numCol/2)*32)-32,32*(numRow+0.5));

    sf::Texture winFaceTex;
    winFaceTex.loadFromFile("files/images/face_win.png");
    sf::Sprite winFace;
    winFace.setTexture(winFaceTex);
    winFace.setPosition(((numCol/2)*32)-32,32*(numRow+0.5));

    sf::Texture debugTex;
    debugTex.loadFromFile("files/images/debug.png");
    sf::Sprite debug;
    debug.setTexture(debugTex);
    debug.setPosition((numCol*32)-304,32*(numRow+0.5));

    sf::Texture playTex;
    playTex.loadFromFile("files/images/play.png");
    sf::Sprite play;
    play.setTexture(playTex);
    play.setPosition((numCol*32)-240,32*(numRow+0.5));

    sf::Texture pauseTex;
    pauseTex.loadFromFile("files/images/pause.png");
    sf::Sprite pause;
    pause.setTexture(pauseTex);
    pause.setPosition((numCol*32)-240,32*(numRow+0.5));

    sf::Texture leaderboardTex;
    leaderboardTex.loadFromFile("files/images/leaderboard.png");
    sf::Sprite leaderboard;
    leaderboard.setTexture(leaderboardTex);
    leaderboard.setPosition((numCol*32)-176,32*(numRow+0.5));

    sf::Texture tileTexture;
    tileTexture.loadFromFile("files/images/tile_hidden.png");

    sf::Texture mineTexture;
    mineTexture.loadFromFile("files/images/mine.png");

    sf::Texture flagTexture;
    flagTexture.loadFromFile("files/images/flag.png");

    sf::Texture revealedTileTexture;
    revealedTileTexture.loadFromFile("files/images/tile_revealed.png");

    // Files for neighboring mine count display
    sf::Texture number1;
    number1.loadFromFile("files/images/number_1.png");
    sf::Texture number2;
    number2.loadFromFile("files/images/number_2.png");
    sf::Texture number3;
    number3.loadFromFile("files/images/number_3.png");
    sf::Texture number4;
    number4.loadFromFile("files/images/number_4.png");
    sf::Texture number5;
    number5.loadFromFile("files/images/number_5.png");
    sf::Texture number6;
    number6.loadFromFile("files/images/number_6.png");
    sf::Texture number7;
    number7.loadFromFile("files/images/number_7.png");
    sf::Texture number8;
    number8.loadFromFile("files/images/number_8.png");

    sf::Texture digitsTexture;
    digitsTexture.loadFromFile("files/images/digits.png");

    std::vector<sf::Sprite> digitSprites;
    int digitWidth = 21;
    int digitHeight = 32;

    for (int i = 0; i < 11; ++i) {
        sf::IntRect textureRect(i * digitWidth, 0, digitWidth, digitHeight);
        sf::Sprite digitSprite(digitsTexture, textureRect);
        digitSprites.push_back(digitSprite);
    }

    const int numRows = numRow;
    const int numCols = numCol;

    // Vector with tiles and mines
    std::vector<std::vector<sf::Sprite>> tiles(numRows, std::vector<sf::Sprite>(numCols, sf::Sprite(tileTexture)));
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            tiles[row][col].setPosition(col * 32, row * 32);
        }
    }

    // Vector with just unrevealed tiles
    std::vector<std::vector<sf::Sprite>> blankTiles(numRows, std::vector<sf::Sprite>(numCols, sf::Sprite(tileTexture)));
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            blankTiles[row][col].setPosition(col * 32, row * 32);
        }
    }

    // Vector with revealed tile for background
    std::vector<std::vector<sf::Sprite>> unrevealed(numRows, std::vector<sf::Sprite>(numCols, sf::Sprite(revealedTileTexture)));
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            unrevealed[row][col].setPosition(col * 32, row * 32);
        }
    }

    // Randomizer for mines
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> randRow(0, numRow - 1);
    std::uniform_int_distribution<int> randCol(0, numCol - 1);

    for (int i = 0; i < numMines; ++i) {
        int row, col;
        do {
            row = randRow(gen);
            col = randCol(gen);
        } while (tiles[row][col].getTexture() == &mineTexture);
        tiles[row][col].setTexture(mineTexture);
    }

    // Timer initializing stuff
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point currentTime;
    std::chrono::steady_clock::time_point pauseTime;

    int minesRemaining = numMines;
    int flagsPlaced = 0;
    bool timerStarted = false;
    bool gamePaused = false;
    bool showMines = false;
    bool gameOver = false;
    bool faceClicked = false;

    while (gameWindow.isOpen()) {
        sf::Event event;
        while (gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                gameWindow.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (!timerStarted) {
                        startTime = std::chrono::steady_clock::now();
                        timerStarted = true;
                    }
                    sf::Vector2i mousePos = sf::Mouse::getPosition(gameWindow);
                    // Leaderboard opens
                    if (leaderboard.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        gamePaused = true;
                        pauseTime = std::chrono::steady_clock::now();
                        sf::RenderWindow leaderboardWindow(sf::VideoMode(numCol*16, (numRow*16)+50), "Leaderboard", sf::Style::Close);

                        sf::Text leaderText("LEADERBOARD", font, 20);
                        leaderText.setPosition(leaderboardWindow.getSize().x / 2 - leaderText.getLocalBounds().width / 2, leaderboardWindow.getSize().y / 2 - 120);
                        leaderText.setStyle(sf::Text::Bold | sf::Text::Underlined);
                        leaderText.setFillColor(sf::Color::White);

                        sf::Text leaderboardDisplay("", font, 18);
                        leaderboardDisplay.setStyle(sf::Text::Bold);
                        leaderboardDisplay.setFillColor(sf::Color::White);

                        std::ifstream leaderboardFile("files/leaderboard.txt");
                        if (!leaderboardFile.is_open()) {
                            std::cout << "Error opening leaderboard file" << std::endl;
                            return 1;
                        }

                        std::string line;
                        int position = 1;

                        while (std::getline(leaderboardFile, line)) {
                            std::istringstream iss(line);
                            std::string time, name;
                            std::getline(iss, time, ',');
                            std::getline(iss, name, ',');

                            leaderboardDisplay.setString(leaderboardDisplay.getString() + std::to_string(position) + ".\t" + time + "\t" + name + "\n\n");
                            ++position;
                        }

                        leaderboardFile.close();

                        leaderboardDisplay.setOrigin(leaderboardDisplay.getLocalBounds().width / 2, leaderboardDisplay.getLocalBounds().height / 2);
                        leaderboardDisplay.setPosition(leaderboardWindow.getSize().x / 2, leaderboardWindow.getSize().y / 2+20);

                        while (leaderboardWindow.isOpen()) {
                            sf::Event event;
                            while (leaderboardWindow.pollEvent(event)) {
                                if (event.type == sf::Event::Closed) {
                                    leaderboardWindow.close();
                                    gamePaused = false;
                                    startTime += std::chrono::steady_clock::now() - pauseTime;
                                }
                            }
                            leaderboardWindow.clear(sf::Color::Blue);
                            leaderboardWindow.draw(leaderText);
                            leaderboardWindow.draw(leaderboardDisplay);
                            leaderboardWindow.display();
                        }
                    }
                    // Pause button
                    else if (pause.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        if (gamePaused == true) {
                            gamePaused = false;
                            startTime += std::chrono::steady_clock::now() - pauseTime;
                        }
                        else {
                            gamePaused = true;
                            pauseTime = std::chrono::steady_clock::now();
                        }
                    }
                    // Debug button
                    else if (debug.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        if (showMines == true) {
                            showMines = false;
                        }
                        else {
                            showMines = true;
                        }
                    }
                    // Face button to reset board
                    else if (happyFace.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        faceClicked = true;
                    }
                    else {
                        // Revealing tile stuff
                        sf::Vector2i mousePos = sf::Mouse::getPosition(gameWindow);
                        int col = mousePos.x / 32;
                        int row = mousePos.y / 32;

                        if (col >= 0 && col < numCols && row >= 0 && row < numRows) {
                            if (tiles[row][col].getTexture() == &mineTexture) {
                                gameOver = true;
                                showMines = true;
                            } else {
                                if (tiles[row][col].getTexture() == &tileTexture) {
                                    std::vector<sf::Vector2i> stack;
                                    stack.push_back(sf::Vector2i(row, col));

                                    while (!stack.empty()) {
                                        sf::Vector2i currentTile = stack.back();
                                        stack.pop_back();

                                        int currentRow = currentTile.x;
                                        int currentCol = currentTile.y;

                                        if (currentRow >= 0 && currentRow < numRows && currentCol >= 0 && currentCol < numCols) {
                                            if (tiles[currentRow][currentCol].getTexture() == &tileTexture) {
                                                int count = 0;
                                                const int directions[8][2] = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };

                                                for (int i = 0; i < 8; ++i) {
                                                    int newRow = currentRow + directions[i][0];
                                                    int newCol = currentCol + directions[i][1];

                                                    if (newRow >= 0 && newRow < tiles.size() && newCol >= 0 && newCol < tiles[0].size()) {
                                                        if (tiles[newRow][newCol].getTexture() == &mineTexture) {
                                                            count++;
                                                        }
                                                    }
                                                }
                                                if (count == 1) {
                                                    tiles[currentRow][currentCol].setTexture(number1);
                                                    blankTiles[currentRow][currentCol].setTexture(number1);
                                                } else if (count == 2) {
                                                    tiles[currentRow][currentCol].setTexture(number2);
                                                    blankTiles[currentRow][currentCol].setTexture(number2);
                                                } else if (count == 3) {
                                                    tiles[currentRow][currentCol].setTexture(number3);
                                                    blankTiles[currentRow][currentCol].setTexture(number3);
                                                } else if (count == 4) {
                                                    tiles[currentRow][currentCol].setTexture(number4);
                                                    blankTiles[currentRow][currentCol].setTexture(number4);
                                                } else if (count == 5) {
                                                    tiles[currentRow][currentCol].setTexture(number5);
                                                    blankTiles[currentRow][currentCol].setTexture(number5);
                                                } else if (count == 6) {
                                                    tiles[currentRow][currentCol].setTexture(number6);
                                                    blankTiles[currentRow][currentCol].setTexture(number6);
                                                } else if (count == 7) {
                                                    tiles[currentRow][currentCol].setTexture(number7);
                                                    blankTiles[currentRow][currentCol].setTexture(number7);
                                                } else if (count == 8) {
                                                    tiles[currentRow][currentCol].setTexture(number8);
                                                    blankTiles[currentRow][currentCol].setTexture(number8);
                                                } else {
                                                    tiles[currentRow][currentCol].setTexture(revealedTileTexture);
                                                    blankTiles[currentRow][currentCol].setTexture(revealedTileTexture);
                                                    for (int i = 0; i < 8; ++i) {
                                                        int newRow = currentRow + directions[i][0];
                                                        int newCol = currentCol + directions[i][1];
                                                        if (newRow >= 0 && newRow < numRows && newCol >= 0 && newCol < numCols &&
                                                            tiles[newRow][newCol].getTexture() == &tileTexture) {
                                                            stack.push_back(sf::Vector2i(newRow, newCol));
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    // Places a flag
                    sf::Vector2i mousePos = sf::Mouse::getPosition(gameWindow);
                    int col = mousePos.x / 32;
                    int row = mousePos.y / 32;

                    if (col >= 0 && col < numCols && row >= 0 && row < numRows) {
                        if (tiles[row][col].getTexture() == &flagTexture) {
                            // Remove the flag
                            tiles[row][col].setTexture(tileTexture);
                            blankTiles[row][col].setTexture(tileTexture);
                            minesRemaining++;
                            flagsPlaced--;
                        } else {
                            // Place flag if no flag
                            tiles[row][col].setTexture(flagTexture);
                            blankTiles[row][col].setTexture(flagTexture);
                            minesRemaining--;
                            flagsPlaced++;
                        }
                        if (flagsPlaced == numMines) {
                            gameOver = true;
                        }
                    }
                }
            }
        }

        gameWindow.clear(sf::Color::White);

        if (gameOver and (minesRemaining == 0)) {
            gameWindow.draw(winFace);
        }
        else if (gameOver and (minesRemaining != 0)) {
            gameWindow.draw(sadFace);
        }
        else {
            gameWindow.draw(happyFace);
        }

        gameWindow.draw(debug);
        gameWindow.draw(pause);
        gameWindow.draw(leaderboard);

        for (int row = 0; row < numRows; ++row) {
            for (int col = 0; col < numCols; ++col) {
                gameWindow.draw(unrevealed[row][col]);
            }
        }

        for (int row = 0; row < numRows; ++row) {
            for (int col = 0; col < numCols; ++col) {
                gameWindow.draw(blankTiles[row][col]);
            }
        }

        if (showMines) {
            for (int row = 0; row < numRows; ++row) {
                for (int col = 0; col < numCols; ++col) {
                    gameWindow.draw(tiles[row][col]);
                }
            }
        }

        // Reset the board
        if (faceClicked) {
            for (int row = 0; row < numRows; ++row) {
                for (int col = 0; col < numCols; ++col) {
                    tiles[row][col].setTexture(tileTexture);
                    blankTiles[row][col].setTexture(tileTexture);
                }
            }

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> randRow(0, numRow - 1);
            std::uniform_int_distribution<int> randCol(0, numCol - 1);

            for (int i = 0; i < numMines; ++i) {
                int row, col;
                do {
                    row = randRow(gen);
                    col = randCol(gen);
                } while (tiles[row][col].getTexture() == &mineTexture);
                tiles[row][col].setTexture(mineTexture);
            }

            minesRemaining = numMines;
            flagsPlaced = 0;
            timerStarted = false;
            gameOver = false;
            showMines = false;
            faceClicked = false;
            startTime = std::chrono::steady_clock::now();
        }

        if (!gamePaused and !gameOver) {
            currentTime = std::chrono::steady_clock::now();
        }
        else if (gamePaused) {
            gameWindow.draw(play);
        }

        std::chrono::duration<double> elapsedSeconds = currentTime - startTime;
        int elapsedMinutes = static_cast<int>(elapsedSeconds.count()) / 60;
        int elapsedSecondsRemainder = static_cast<int>(elapsedSeconds.count()) % 60;

        int minutesPositionX = (numCols * 32) - 97;
        int secondsPositionX = (numCols * 32) - 54;

        digitSprites[0].setPosition((numCol * 32) - 97, 32 * (numRow + 0.5) + 16);
        gameWindow.draw(digitSprites[0]);
        digitSprites[0].setPosition((numCol * 32) - 97 + digitWidth, 32 * (numRow + 0.5) + 16);
        gameWindow.draw(digitSprites[0]);
        digitSprites[0].setPosition((numCol * 32) - 54, 32 * (numRow + 0.5) + 16);
        gameWindow.draw(digitSprites[0]);
        digitSprites[0].setPosition((numCol * 32) - 54 + digitWidth, 32 * (numRow + 0.5) + 16);
        gameWindow.draw(digitSprites[0]);

        if (timerStarted) {
            int displayMinutes = elapsedMinutes;
            while (displayMinutes > 0) {
                int digit = displayMinutes % 10;
                digitSprites[digit].setPosition(minutesPositionX + digitWidth, 32 * (numRow + 0.5) + 16);
                gameWindow.draw(digitSprites[digit]);
                minutesPositionX -= digitWidth;
                displayMinutes /= 10;
            }

            int displaySeconds = elapsedSecondsRemainder;
            while (displaySeconds > 0) {
                int digit = displaySeconds % 10;
                digitSprites[digit].setPosition(secondsPositionX + digitWidth, 32 * (numRow + 0.5) + 16);
                gameWindow.draw(digitSprites[digit]);
                secondsPositionX -= digitWidth;
                displaySeconds /= 10;
            }
        }

        // Counter displaying stuff
        digitSprites[0].setPosition(33, 32 * (numRow + 0.5) + 16);
        gameWindow.draw(digitSprites[0]);
        digitSprites[0].setPosition(33 + digitWidth, 32 * (numRow + 0.5) + 16);
        gameWindow.draw(digitSprites[0]);
        digitSprites[0].setPosition(33 + digitWidth + digitWidth, 32 * (numRow + 0.5) + 16);
        gameWindow.draw(digitSprites[0]);

        int displayNumMines = minesRemaining;
        if (displayNumMines == 0) {
            gameWindow.draw(digitSprites[0]);
        } else {
            int digitPositionX = 33+42;
            while (displayNumMines > 0) {
                int digit = displayNumMines % 10;
                digitSprites[digit].setPosition(digitPositionX, 32 * (numRow + 0.5) + 16);
                gameWindow.draw(digitSprites[digit]);
                digitPositionX -= digitWidth;
                displayNumMines /= 10;
            }
        }
        gameWindow.display();
    }
    return 0;
}