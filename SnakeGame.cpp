#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <ctime>
using namespace std;

// Variables and arrays declaration
bool gameOver;
bool invalidCoord;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int tailLength;
int sleeplength;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;

void ClearScreen()
{
    // Function which cleans the screen without flickering
    COORD cursorPosition;   cursorPosition.X = 0;   cursorPosition.Y = 0;   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

void Setup()
{   // Initialise variables
    gameOver = false;
    dir = STOP;
    srand(time(0));
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;

}

void Draw() // Drawing playing field, snake and fruits
{
    ClearScreen();

    // Draws top border
    for (int i = 0; i < width + 2; i++)
        cout << '\xDB';
    cout << endl;

    for (int i = 0; i < height; i++)
    {
        for (int k = 0; k < width; k++)
        {
            // Left border
            if (k == 0)
                cout << '\xDB';
            // Snake's head
            if (i == y && k == x)
                cout << "\xDB";
            // Fruit
            else if (i == fruitY && k == fruitX)
                cout << '\xA2';

            else
            {
                // Checks if there is a tail block with appropriate coordinates and draws it 
                bool printTail = false;
                for (int j = 0; j < tailLength; j++)
                {
                    if (tailX[j] == k && tailY[j] == i)
                    {
                        cout << '\xDB';
                        printTail = true;
                    }
                }
                // Draws blank space if there is nothing to display
                if (!printTail)
                    cout << ' ';
            }

            // Right border
            if (k == width - 1)
                cout << '\xDB';

        }
        cout << endl;
    }

    // Draws bottom border
    for (int i = 0; i < width + 2; i++)
        cout << '\xDB';
    cout << endl;

    // Displays player's score
    cout << endl;
    cout << "Score: " << score << endl;

}
void Input()
{
    // Changes snake's direction depending on the button pressed and doesn't allow player to change direction in invalid way 
    if (_kbhit())
    {
        switch ((_getch)())
        {
        case 'w':
            if (dir != DOWN)
                dir = UP;
            break;
        case 'a':
            if (dir != RIGHT)
                dir = LEFT;
            break;
        case 's':
            if (dir != UP)
                dir = DOWN;
            break;
        case 'd':
            if (dir != LEFT)
                dir = RIGHT;
            break;
        case 'k':
            gameOver = true;
            break;
        case 'W':
            if (dir != DOWN)
                dir = UP;
            break;
        case 'A':
            if (dir != RIGHT)
                dir = LEFT;
            break;
        case 'S':
            if (dir != UP)
                dir = DOWN;
            break;
        case 'D':
            if (dir != LEFT)
                dir = RIGHT;
            break;
        case 'K':
            gameOver = true;
            break;
        }

    }
}

void Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prevX2, prevY2;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < tailLength; i++)
    {
        prevX2 = tailX[i];
        prevY2 = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prevX2;
        prevY = prevY2;
    }
    // Changes snake's head coordinates depending on a direction
    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    }

    // Detects collision with a tail
    for (int i = 0; i < tailLength; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

    // Detects collision with a fruit
    if (x == fruitX && y == fruitY)
    {
        score += 1;
        fruitX = rand() % width;
        fruitY = rand() % height;
        // Check if the fruit would overlap with the snake and replaces the fruit
        for (int i = 0; i < tailLength; )
        {
            invalidCoord = false;
            if (tailX[i] == fruitX && tailY[i] == fruitY) {
                invalidCoord = true;
                fruitX = rand() % width;
                fruitY = rand() % height;
                break;
            }
            if (!invalidCoord)
                i++;
        }
        tailLength++;
    }

    //Kills the snake upon touching the wall
    if (y >= height || y < 0)
        gameOver = true;
    if (x >= width || x < 0)
        gameOver = true;
}

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}


int main()
{
    ShowConsoleCursor(false);
        keybd_event(VK_MENU,
                0x38,
                0,
                0);
    keybd_event(VK_RETURN,
                0x1c,
                0,
                0);
    keybd_event(VK_RETURN,
                0x1c,
                KEYEVENTF_KEYUP,
                0);
    keybd_event(VK_MENU,
                0x38,
                KEYEVENTF_KEYUP,
                0);

    Setup();

    HANDLE console_color;
    console_color = GetStdHandle(
        STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(
        console_color, 2);
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 24;                   // Width of each character in the font
    cfi.dwFontSize.Y = 24;                  // Height
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, L"MS Gothic"); // Choose your font
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

    while (!gameOver) // Game mainloop 
    {
        ShowConsoleCursor(false);
        Draw();
        if(dir == LEFT or dir == RIGHT) { sleeplength = 70; } // Helps to equate vertical snake movement speed and horizontal speed
        else { sleeplength = 71; }
        Sleep(sleeplength);
        Input();
        Logic();

    }

    return 0;
}
//d