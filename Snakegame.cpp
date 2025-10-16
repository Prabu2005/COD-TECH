#include <iostream>
#include <vector>
#include <conio.h> // For _kbhit() and _getch() on Windows
#include <windows.h> // For Sleep() and console functions

// Define game area dimensions
const int width = 40;
const int height = 20;

// Game state variables
bool gameOver;
int x, y, fruitX, fruitY, score;
std::vector<int> tailX, tailY;
int nTail;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

// Set up the initial state of the game
void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
    tailX.clear();
    tailY.clear();
}

// Draw the game board, snake, fruit, and score
void Draw() {
    system("cls"); // Clear the console screen
    for (int i = 0; i < width + 2; i++) std::cout << "#";
    std::cout << std::endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) std::cout << "#";
            if (i == y && j == x) std::cout << "O"; // Snake head
            else if (i == fruitY && j == fruitX) std::cout << "F"; // Fruit
            else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        std::cout << "o"; // Snake tail
                        print = true;
                    }
                }
                if (!print) std::cout << " ";
            }
            if (j == width - 1) std::cout << "#";
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < width + 2; i++) std::cout << "#";
    std::cout << std::endl;
    std::cout << "Score:" << score << std::endl;
}

// Handle user keyboard input
void Input() {
    if (_kbhit()) { // Check if a key has been pressed
        switch (_getch()) { // Get the character of the pressed key
            case 'a': if (dir != RIGHT) dir = LEFT; break;
            case 'd': if (dir != LEFT) dir = RIGHT; break;
            case 'w': if (dir != DOWN) dir = UP; break;
            case 's': if (dir != UP) dir = DOWN; break;
            case 'x': gameOver = true; break;
        }
    }
}

// Update game logic (snake movement, collisions, etc.)
void Logic() {
    // Update tail positions
    int prevX = tailX.empty() ? x : tailX[0];
    int prevY = tailY.empty() ? y : tailY[0];
    int prev2X, prev2Y;
    if (!tailX.empty()) {
        tailX[0] = x;
        tailY[0] = y;
    } else if (nTail > 0) { // First element of tail
        tailX.push_back(x);
        tailY.push_back(y);
    }

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Update head position based on direction
    switch (dir) {
        case LEFT: x--; break;
        case RIGHT: x++; break;
        case UP: y--; break;
        case DOWN: y++; break;
        default: break;
    }

    // Check for collision with walls
    if (x >= width || x < 0 || y >= height || y < 0) {
        gameOver = true;
    }
    
    // Check for collision with tail
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            gameOver = true;
        }
    }

    // Check for eating fruit
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
        // Ensure new tail segment is properly initialized
        if (nTail > tailX.size()) {
            tailX.push_back(prevX);
            tailY.push_back(prevY);
        }
    }
}

int main() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(100); // Control game speed
    }
    std::cout << "Game Over!" << std::endl;
    return 0;
}
