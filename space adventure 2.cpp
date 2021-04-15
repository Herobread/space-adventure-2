#include <iostream>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <conio.h>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

short width = 130, height = 50;

wchar_t* screen = new wchar_t[width * height * 200]; // Массив для записи в буфер
HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); // Буфер экрана

int ofset = 30, delay = 17, game = 0, debug = 0;
int len = 0, hp = 3, customAsteroidSpawnRate = 0, mercurySpawn = 2000, venusSpawn = 2800, earthSpawn = 3920, marsSpawn = 5488, asteroidBeltStart = 6500, asteroidBeltEnd = 7000, jupiterSpawn = 7683, saturnSpawn = 10756, uranusSpawn = 15059, neptuneSpawn = 21083, plutoSpawn = 29516, voyagerSpawn = 32000;
// some game var 
int voyagerX = 0, voyagerY = 0, victoryEnd = 0, set2 = 0, set3 = 0, set4 = 0, victory = 30, planetX = 0, planetY = 0, invincible = 0, invincibleE1 = 0, invincibleE2 = 0, colisionNum = 0, showLength = 1, event = 0, frame = 0, martianLength = 0, wave = 0, cooldown = 0, entityColisionX1 = 0, entityColisionX2 = 0, entityColisionY1 = 0, entityColisionY2 = 0;
string str, planetType, button;

int planets[10] = { 0, 2000, 2800, 3920, 5488, 7683, 10756, 15059, 21083, 29516 };

int shipCoordX[21] = { 0,1,2,3,1,2,3,4,0,1,2,3,4,5,6,1,2,3,4,0,1 }; // ship colision coords relative to x y
int shipCoordY[21] = { 0,0,0,0,1,1,1,1,2,2,2,2,2,2,2,3,3,3,3,4,4 };
int colisionX;
int colisionY;
int ufoColisionX[13] = { 3,3,4,5,2,3,4,0,1,2,3,4,5 };
int ufoColisionY[13] = { 0,0,0,0,1,1,1,2,2,2,3,3,3 };

float x = width / 8, y = height / 2 - 3; // flight

float playerX = width / 2, playerY = height / 2, jumpTime = 0;//walk

vector <int> trailY = { 0,0,0,0 };

vector <int> dotX;
vector <int> dotY;
vector <int> dotSpeed;

vector <int> asteroidX;
vector <int> asteroidY;
vector <int> asteroidType;
vector <int> asteroidHp;
vector <float> asteroidDestroyed;

vector <int> bulletXp;
vector <int> bulletYp;

void map(int pos);
void flight(int startPos = 0, int destination = 0);
void mainMenu();
void walk(int planet, int startPos = 0);

string getKey()
{
    string c;
    char b;
    int a;
    a = _getch();//получаем ascii код символа
    if (a == 224)// особые кнопки
    {
        a = _getch();
        if (a == 75)
        {
            return "leftArrow";
        }
        if (a == 72)
        {
            return "upArrow";
        }
        if (a == 80)
        {
            return "downArrow";
        }
        if (a == 77)
        {
            return "rightArrow";
        }
        if (a == 82)
        {
            return "insert";
        }
    }
    // больше особых кнопок
    else if (a == 32)
    {
        return "space";
    }
    if (a == 8)
    {
        return "backspace";
    }
    if (a == 9)
    {
        return "tab";
    }
    if (a == 13)
    {
        return "enter";
    }
    if (a == 27)
    {
        return "esc";
    }
    b = a;// превращяем int в чар
    c = b;// ревращаем char в string
    return c;// возвращаем значение функции
}

void text(int X, int Y, string text)
{
    for (int i = 0; i < text.size(); i += 1)
    {
        screen[Y * width + X + i] = text[i];
    }
}

void emptyBuffer()
{
    for (int i = 0; i < height; i += 1)
    {
        for (int j = 0; j < width; j += 1)
        {
            screen[i * width + j] = ' ';
        }
    }
}

void render()
{
    DWORD dwBytesWritten = 0; // Для дебага

    screen[width * height - 1] = '\0'; // end

    WriteConsoleOutputCharacter(hConsole, screen, width * height, { 0, 0 }, &dwBytesWritten); // Запись в буфер

    emptyBuffer(); // empty
}

string texture[200];

void drawTexture(int X, int Y, int n)
{
    for (int i = 0; i < n; i += 1)
    {
        for (int j = 0; j < texture[i].size(); j += 1)
        {
            if (texture[i][j] != 'q' && j + X < width && j + X >= 0)
            {
                screen[(i + Y) * width + j + X] = texture[i][j];
            }
        }
    }
}

void borderText(int X, int Y, string text)
{
    for (int i = 0; i < text.size(); i += 1)
    {
        if (i + X < width)
            screen[Y * width + X + i] = text[i];
    }
}

void shipMods(int X, int Y)
{
    texture[0] = "                    gun";
    texture[1] = "          <\\__    /   ";
    texture[2] = "           \\  \\  /   ";
    texture[3] = "          <  0 |-      ";
    texture[4] = "           /__/        ";
    texture[5] = "         /</           ";
    texture[6] = "        /              ";
    texture[7] = "boosters               ";
    texture[8] = "                       ";
    drawTexture(X, Y, 9);
}

void ship(int X, int Y)
{
    const int n = 5;
    texture[0] = "<\\__";
    texture[1] = "q\\  \\";
    texture[2] = "<= 0 |-";
    texture[3] = "q/__/";
    texture[4] = "</";

    drawTexture(X, Y, n);
}

void smallShip(int X, int Y)
{
    texture[0] = "\\___";
    texture[1] = "\\   \\";
    texture[2] = "/ __/";
    texture[3] = "/";
    drawTexture(X, Y, 4);
}

void verySmallShip(int X, int Y)
{
    texture[0] = ">=-";
    drawTexture(X, Y + 2, 1);
}

void veryVerySmallShip(int X, int Y)
{
    texture[0] = "`";
    drawTexture(X, Y + 2, 1);
}

void verticalShip(int X, int Y)
{
    texture[0] = "qqq|  ";
    texture[1] = "qq/ \\";
    texture[2] = "q|   |";
    texture[3] = "q| 0 |";
    texture[4] = "q|   |";
    texture[5] = "//-|-\\\\";
    drawTexture(X, Y, 6);
}

void landingAnimation(int planetPos)
{
    int deltaY = 0, deltaX = 0, deltaYBg = 0, h = 16;
    emptyBuffer();
    float rocketY = 0;

    string bg[50];
    if (planetPos == 3)
    {
        bg[0] =  "                     .--                                                                                                          ";
        bg[1] =  "                  .+(   )                                                                                                         ";
        bg[2] =  "                  (     `)                                                                                                        ";
        bg[3] =  "                 (       ))                                                                                                       ";
        bg[4] =  "                  `-___.'                                                                                                         ";
        bg[5] =  "                    \\||                                                                                              _           ";
        bg[6] =  "                     ||/                                                                                            / \\          ";
        bg[7] =  "                     ||                                                                                             \\_/          ";
        bg[8] =  "                     | \\                    OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO                             |            ";
        bg[9] =  "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOooooooooooooooooooooooooooooooooooooooooooooOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO";
        bg[10] = "oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo";
        bg[11] = "oooooooooooooooooooooooooooooooooooooooooooooo|o|o|||o||oo|o|o|o||o|o|||o||||o|||o||o|oooooooooooooooooooooooooooooooooooooooooooo";
        bg[12] = "||o|o||o|o|o|o|o||o||o||o||o||o|||o||o||o||o||o.||.|.|.||.||.|.||.|.|..|..|...|.||..||o|o|o|||oo||o||o||o|o|o||o||o|||o||o|o||o||o";
        bg[13] = ".|.|.|.|.|.|.|.|.|.|.|.|..||.|.|.||.|.||.||.|.|.|....|...|...|..|..|...|..|...|.|.|..|.|.|.|.|||.|.||.|||.||.|.|.|.||.||.||.|.||.|";
        bg[14] = "............................................................ .  . . . ............................................................";
        bg[15] = ". . . . .. . . . . . . . . .  . .  .   .  .. .. . . . . .                .    .  .  .  .  .  .   .  .  .  .  . .  .  . .. .  . . .";
        h = 16;
        deltaY = 3;
        deltaX = 2;
        deltaYBg = height/2;
    }
    else if (planetPos == 4)
    {
        bg[0] = " ",
            bg[1] = "                                                                                                                                  ";
        bg[2] = "                                                                                                                                  ";
        bg[3] = "                                                                                                                                  ";
        bg[4] = "                                                                                                                                  ";
        bg[5] = "                                                                                                                                  ";
        bg[6] = "                                                                                                                                  ";
        bg[7] = "                                                                                                                                  ";
        bg[8] = "                                                                                                                                  ";
        bg[9] = "00000000";
        bg[10] = "        0000000000000000                                                                                            00000000000000";
        bg[11] = "                        0000000000000                                                        00000000000000000000000";
        bg[12] = "                                     00000000000000000000000000000000000000000000000000000000";

        h = 13;
        deltaY = 7;
        deltaX = 2;
        deltaYBg = height / 2;
    }

    for ( int l = 0 ; l < 100; l += 1)
    {
        for (int i = 0; i < h; i += 1)
        {
            for (int ii = 0; ii < bg[i].length(); ii += 1)
            {
                screen[(i+ deltaYBg) * width + ii] = bg[i][ii];
            }
        }

        text(1, 1, to_string(l));

        if (rocketY < 10)
            rocketY += 0.5;
        else if (rocketY < 15)
            rocketY += 0.4;
        else if (rocketY < 20)
            rocketY += 0.25;
        else if (rocketY < 23)
            rocketY += 0.2;


        str = "_________________";
        text(width / 2 - str.length()/2 - 1 + deltaX, 28 + deltaY, str);
        str = "/  |  |  |  |  |  \\ ";
        text(width / 2 - str.length() / 2 + deltaX, 29 + deltaY, str);

        verticalShip(width/2-4 + deltaX,rocketY + deltaY);

        /// particles
        if (rocketY < 13)
        {
            text(width / 2 - 4 + deltaX, rocketY + 6 + deltaY, "#");
            text(width / 2 - 4 + deltaX, rocketY + 7 + deltaY, "#");
            text(width / 2 - 4 + deltaX, rocketY + 8 + deltaY, "%");
            text(width / 2 - 4 + deltaX, rocketY + 9 + deltaY, "%");
            text(width / 2 - 4 + deltaX, rocketY + 10 + deltaY, "|");
            if (rand() % 5 != 1)
                text(width / 2 - 4 + deltaX, rocketY + 11 + deltaY, "|");

            text(width / 2 - 1 + deltaX, rocketY + 6 + deltaY, "#");
            text(width / 2 - 1 + deltaX, rocketY + 7 + deltaY, "#");
            text(width / 2 - 1 + deltaX, rocketY + 8 + deltaY, "%");
            text(width / 2 - 1 + deltaX, rocketY + 9 + deltaY, "%");
            text(width / 2 - 1 + deltaX, rocketY + 10 + deltaY, "|");
            if (rand() % 5 != 1)
                text(width / 2 - 1 + deltaX, rocketY + 11 + deltaY, "|");

            text(width / 2 + 2 + deltaX, rocketY + 6 + deltaY, "#");
            text(width / 2 + 2 + deltaX, rocketY + 7 + deltaY, "#");
            text(width / 2 + 2 + deltaX, rocketY + 8 + deltaY, "%");
            text(width / 2 + 2 + deltaX, rocketY + 9 + deltaY, "%");
            text(width / 2 + 2 + deltaX, rocketY + 10 + deltaY, "|");
            if (rand() % 5 != 1)
                text(width / 2 + 2 + deltaX, rocketY + 11 + deltaY, "|");
        }
        else if (rocketY < 20)
        {
            text(width / 2 - 4 + deltaX, rocketY + 6 + deltaY, "%");
            text(width / 2 - 4 + deltaX, rocketY + 7 + deltaY, "%");
            text(width / 2 - 4 + deltaX, rocketY + 8 + deltaY, "|");
            if (rand() % 5 != 1)
                text(width / 2 - 4 + deltaX, rocketY + 8 + deltaY, "|");

            text(width / 2 - 1 + deltaX, rocketY + 6 + deltaY, "%");
            text(width / 2 - 1 + deltaX, rocketY + 7 + deltaY, "%");
            text(width / 2 - 1 + deltaX, rocketY + 8 + deltaY, "|");
            if (rand() % 5 != 1)
                text(width / 2 - 1 + deltaX, rocketY + 8 + deltaY, "|");

            text(width / 2 + 2 + deltaX, rocketY + 6 + deltaY, "%");
            text(width / 2 + 2 + deltaX, rocketY + 7 + deltaY, "%");
            text(width / 2 + 2 + deltaX, rocketY + 8 + deltaY, "|");
            if (rand() % 5 != 1)
                text(width / 2 + 2 + deltaX, rocketY + 8 + deltaY, "|");
        }
        else if (rocketY < 23)
        {
            text(width / 2 - 4 + deltaX, rocketY + 6 + deltaY, "%");
            text(width / 2 - 4 + deltaX, rocketY + 7 + deltaY, "|");

            text(width / 2 - 1 + deltaX, rocketY + 6 + deltaY, "%");
            text(width / 2 - 1 + deltaX, rocketY + 7 + deltaY, "|");

            text(width / 2 + 2 + deltaX, rocketY + 6 + deltaY, "%");
            text(width / 2 + 2 + deltaX, rocketY + 7 + deltaY, "|");
        }

        ///

        render();
        emptyBuffer();
        Sleep(30);
    }
    for (int i = 0; i < 20; i += 1)
    {
        for (int i = 0; i < h; i += 1)
        {
            for (int ii = 0; ii < bg[i].length(); ii += 1)
            {
                screen[(i + deltaYBg) * width + ii] = bg[i][ii];
            }
        }

        str = "_________________";
        text(width / 2 - str.length() / 2 - 1 + deltaX, 28 + deltaY, str);
        str = "/  |  |  |  |  |  \\ ";
        text(width / 2 - str.length() / 2 + deltaX, 29 + deltaY, str);

        verticalShip(width / 2 - 4 + deltaX, rocketY + deltaY);

        if (i < 4)
        {
            text(width / 2 - 2 + deltaX, 28 + deltaY -  2, "@");
        }
        else if (i < 7)
        {
            text(width / 2 - 2 + deltaX, 28 + deltaY - 1, "@");
        }
        else if (i < 11)
        {
            text(width / 2 - 2 + deltaX, 28 + deltaY, "@");
        }
        else
        {
            text(width / 2 - 2 + deltaX, 28 + deltaY + 1, "@");
        }

        render();
        emptyBuffer();
        Sleep(30);
    }

    walk(planetPos);
}

void liftOfAnimation(int planetPos)
{
    int deltaY = 0, deltaX = 0, deltaYBg = 0, h = 16;
    emptyBuffer();
    float rocketY = 23;

    string bg[50];
    if (planetPos == 3)
    {
        bg[0] = "                     .--                                                                                                          ";
        bg[1] = "                  .+(   )                                                                                                         ";
        bg[2] = "                  (     `)                                                                                                        ";
        bg[3] = "                 (       ))                                                                                                       ";
        bg[4] = "                  `-___.'                                                                                                         ";
        bg[5] = "                    \\||                                                                                              _           ";
        bg[6] = "                     ||/                                                                                            / \\          ";
        bg[7] = "                     ||                                                                                             \\_/          ";
        bg[8] = "                     | \\                    OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO                             |            ";
        bg[9] = "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOooooooooooooooooooooooooooooooooooooooooooooOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO";
        bg[10] = "oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo";
        bg[11] = "oooooooooooooooooooooooooooooooooooooooooooooo|o|o|||o||oo|o|o|o||o|o|||o||||o|||o||o|oooooooooooooooooooooooooooooooooooooooooooo";
        bg[12] = "||o|o||o|o|o|o|o||o||o||o||o||o|||o||o||o||o||o.||.|.|.||.||.|.||.|.|..|..|...|.||..||o|o|o|||oo||o||o||o|o|o||o||o|||o||o|o||o||o";
        bg[13] = ".|.|.|.|.|.|.|.|.|.|.|.|..||.|.|.||.|.||.||.|.|.|....|...|...|..|..|...|..|...|.|.|..|.|.|.|.|||.|.||.|||.||.|.|.|.||.||.||.|.||.|";
        bg[14] = "............................................................ .  . . . ............................................................";
        bg[15] = ". . . . .. . . . . . . . . .  . .  .   .  .. .. . . . . .                .    .  .  .  .  .  .   .  .  .  .  . .  .  . .. .  . . .";
        h = 16;
        deltaY = 3;
        deltaX = 2;
        deltaYBg = height / 2;
    }
    else if (planetPos == 4)
    {
        bg[0]  = " ",
        bg[1]  = "                                                                                                                                  ";
        bg[2]  = "                                                                                                                                  ";
        bg[3]  = "                                                                                                                                  ";
        bg[4]  = "                                                                                                                                  ";
        bg[5]  = "                                                                                                                                  ";
        bg[6]  = "                                                                                                                                  ";
        bg[7]  = "                                                                                                                                  ";
        bg[8]  = "                                                                                                                                  ";
        bg[9] = "00000000";
        bg[10] = "        0000000000000000                                                                                            00000000000000";
        bg[11] = "                        0000000000000                                                        00000000000000000000000";
        bg[12] = "                                     00000000000000000000000000000000000000000000000000000000";

        h = 13;
        deltaY = 7;
        deltaX = 2;
        deltaYBg = height / 2;
    }
    else
    {
        h = 0;
        deltaY = 3;
        deltaX = 2;
        deltaYBg = height / 2;
    }

    for (int i = 20; i > 0; i -= 1)
    {
        for (int i = 0; i < h; i += 1)
        {
            for (int ii = 0; ii < bg[i].length(); ii += 1)
            {
                screen[(i + deltaYBg) * width + ii] = bg[i][ii];
            }
        }

        str = "_________________";
        text(width / 2 - str.length() / 2 - 1 + deltaX, 28 + deltaY, str);
        str = "/  |  |  |  |  |  \\ ";
        text(width / 2 - str.length() / 2 + deltaX, 29 + deltaY, str);

        verticalShip(width / 2 - 4 + deltaX, rocketY + deltaY);

        if (i < 4)
        {
            text(width / 2 - 2 + deltaX, 28 + deltaY - 2, "@");
        }
        else if (i < 7)
        {
            text(width / 2 - 2 + deltaX, 28 + deltaY - 1, "@");
        }
        else if (i < 11)
        {
            text(width / 2 - 2 + deltaX, 28 + deltaY, "@");
        }
        else
        {
            text(width / 2 - 2 + deltaX, 28 + deltaY + 1, "@");
        }

        render();
        emptyBuffer();
        Sleep(30);
    }

    for (int l = 100; l > 0; l -= 1)
    {
        for (int i = 0; i < h; i += 1)
        {
            for (int ii = 0; ii < bg[i].length(); ii += 1)
            {
                screen[(i + deltaYBg) * width + ii] = bg[i][ii];
            }
        }

        text(1, 1, to_string(l));

        if (rocketY > -1)
        {
            if (rocketY < 10)
                rocketY -= 0.5;
            else if (rocketY < 15)
                rocketY -= 0.4;
            else if (rocketY < 25)
                rocketY -= 0.25;
        }
        else
            l = -1;


        str = "_________________";
        text(width / 2 - str.length() / 2 - 1 + deltaX, 28 + deltaY, str);
        str = "/  |  |  |  |  |  \\ ";
        text(width / 2 - str.length() / 2 + deltaX, 29 + deltaY, str);

        verticalShip(width / 2 - 4 + deltaX, rocketY + deltaY);

        /// particles
        if (rocketY < 13)
        {
            text(width / 2 - 4 + deltaX, rocketY + 6 + deltaY, "#");
            text(width / 2 - 4 + deltaX, rocketY + 7 + deltaY, "#");
            text(width / 2 - 4 + deltaX, rocketY + 8 + deltaY, "%");
            text(width / 2 - 4 + deltaX, rocketY + 9 + deltaY, "%");
            text(width / 2 - 4 + deltaX, rocketY + 10 + deltaY, "|");
            if (rand() % 5 != 1)
                text(width / 2 - 4 + deltaX, rocketY + 11 + deltaY, "|");

            text(width / 2 - 1 + deltaX, rocketY + 6 + deltaY, "#");
            text(width / 2 - 1 + deltaX, rocketY + 7 + deltaY, "#");
            text(width / 2 - 1 + deltaX, rocketY + 8 + deltaY, "%");
            text(width / 2 - 1 + deltaX, rocketY + 9 + deltaY, "%");
            text(width / 2 - 1 + deltaX, rocketY + 10 + deltaY, "|");
            if (rand() % 5 != 1)
                text(width / 2 - 1 + deltaX, rocketY + 11 + deltaY, "|");

            text(width / 2 + 2 + deltaX, rocketY + 6 + deltaY, "#");
            text(width / 2 + 2 + deltaX, rocketY + 7 + deltaY, "#");
            text(width / 2 + 2 + deltaX, rocketY + 8 + deltaY, "%");
            text(width / 2 + 2 + deltaX, rocketY + 9 + deltaY, "%");
            text(width / 2 + 2 + deltaX, rocketY + 10 + deltaY, "|");
            if (rand() % 5 != 1)
                text(width / 2 + 2 + deltaX, rocketY + 11 + deltaY, "|");
        }
        else if (rocketY < 20)
        {
            text(width / 2 - 4 + deltaX, rocketY + 6 + deltaY, "%");
            text(width / 2 - 4 + deltaX, rocketY + 7 + deltaY, "%");
            text(width / 2 - 4 + deltaX, rocketY + 8 + deltaY, "|");
            if (rand() % 5 != 1)
                text(width / 2 - 4 + deltaX, rocketY + 8 + deltaY, "|");

            text(width / 2 - 1 + deltaX, rocketY + 6 + deltaY, "%");
            text(width / 2 - 1 + deltaX, rocketY + 7 + deltaY, "%");
            text(width / 2 - 1 + deltaX, rocketY + 8 + deltaY, "|");
            if (rand() % 5 != 1)
                text(width / 2 - 1 + deltaX, rocketY + 8 + deltaY, "|");

            text(width / 2 + 2 + deltaX, rocketY + 6 + deltaY, "%");
            text(width / 2 + 2 + deltaX, rocketY + 7 + deltaY, "%");
            text(width / 2 + 2 + deltaX, rocketY + 8 + deltaY, "|");
            if (rand() % 5 != 1)
                text(width / 2 + 2 + deltaX, rocketY + 8 + deltaY, "|");
        }
        else if (rocketY < 23)
        {
            text(width / 2 - 4 + deltaX, rocketY + 6 + deltaY, "%");
            text(width / 2 - 4 + deltaX, rocketY + 7 + deltaY, "|");

            text(width / 2 - 1 + deltaX, rocketY + 6 + deltaY, "%");
            text(width / 2 - 1 + deltaX, rocketY + 7 + deltaY, "|");

            text(width / 2 + 2 + deltaX, rocketY + 6 + deltaY, "%");
            text(width / 2 + 2 + deltaX, rocketY + 7 + deltaY, "|");
        }

        ///

        render();
        emptyBuffer();
        Sleep(30);
    }
}

void logo(int X, int Y)
{
    texture[0] = "   _____                       ___       __                 __                 ___ ";
    texture[1] = "  / ___/____  ____ _________  /   | ____/ /   _____  ____  / /___  __________ |__ \\";
    texture[2] = "  \\__ \\/ __ \\/ __ `/ ___/ _ \\/ /| |/ __  / | / / _ \\/ __ \\/ __/ / / / ___/ _ \\__/ /";
    texture[3] = " ___/ / /_/ / /_/ / /__/  __/ ___ / /_/ /| |/ /  __/ / / / /_/ /_/ / /  /  __/ __/ ";
    texture[4] = "/____/ .___/\\__,_/\\___/\\___/_/  |_\\__,_/ |___/\\___/_/ /_/\\__/\\__,_/_/   \\___/____/ ";
    texture[5] = "    /_/                                                                            ";
    drawTexture(X, Y, 6);
}

void mercury(int X, int Y)
{
    texture[0] = "qqqqqqqqqq___---___";
    texture[1] = "qqqqqqq_--   /o    --_";
    texture[2] = "qqqq_--        \\  .   --_";
    texture[3] = "qqq/    /  / .. \\o .\\  . \\";
    texture[4] = "qq/  ..|. |   |  | . |.   \\";
    texture[5] = "q| . |    | ..| . |     .  |";
    texture[6] = "q|  |..|    . | .    .     |";
    texture[7] = "q| .|      | .   o..  .. . |";
    texture[8] = "|     .  /     .    .   .   |";
    texture[9] = "|.   .  /  .    .  .  ..  0 |";
    texture[10] = "| . .     \\  /    . . .     |";
    texture[11] = "|   . ..   \\|  . .   .. /   |";
    texture[12] = "|  . .  . . o.  .      /.   |";
    texture[13] = "q|.   .  .  .\\    ._   |   |";
    texture[14] = "q| . .      . \\  . .--|0|  |";
    texture[15] = "q|  .. . ..  . ... .  .|   | ";
    texture[16] = "qq\\.   .     . .   .   /  /";
    texture[17] = "qqq\\_ . . . .     .     _/";
    texture[18] = "qqqqq--   . . . .  .  --";
    texture[19] = "qqqqqqq---___   __.---";
    texture[20] = "qqqqqqqqqqqqq---";
    drawTexture(X, Y, 21);
}

void venus(int X, int Y)
{
    texture[0] = "qqqqqqqqqq___---___";
    texture[1] = "qqqqqqq_--   ---   --_";
    texture[2] = "qqqq_--  --+-___----  --_";
    texture[3] = "qqq/--_____      ______--\\";
    texture[4] = "qq/        -+----         \\";
    texture[5] = "q|---_____     .  ______---|";
    texture[6] = "q|        --------         |";
    texture[7] = "q|--- . -   .      --------|";
    texture[8] = "| . .  .. ---------   .     |";
    texture[9] = "|   ..... .          .    . |";
    texture[10] = "|-.. - . . . ---------------|";
    texture[11] = "| . ...  .  .  /   .  .  .  |";
    texture[12] = "|   . . .  ________ ..  .   |";
    texture[13] = "q|-.-.---          --------|";
    texture[14] = "q|   _____--------______   |";
    texture[15] = "q|---          +        -+-|";
    texture[16] = "qq\\    ___--------____    /";
    texture[17] = "qqq\\_--    \\____      --_/";
    texture[18] = "qqqqq--  ---    ---   --";
    texture[19] = "qqqqqqq---___---___---";
    texture[20] = "qqqqqqqqqqqqq---";
    drawTexture(X, Y, 21);
}

void earth(int X, int Y)
{
    texture[0] = "          ___---___";
    texture[1] = "       _--         --_";
    texture[2] = "    _--   .     ______--_";
    texture[3] = "   / ___ o .   /         \\";
    texture[4] = "  / |   \\__.   \\_--\\      \\";
    texture[5] = " |  |      \\    _  _|      |";
    texture[6] = " |   \\     |   / --        |";
    texture[7] = " |   |     |  |   ____     |";
    texture[8] = "|    |  __/    \\_-    -     |";
    texture[9] = "|     \\/ __      ____ /     |";
    texture[10] = "|      _-  \\    /    \\\\  ___|";
    texture[11] = "|     /    |    \\_    \\\\/   |";
    texture[12] = "|     |    /      \\    |    |";
    texture[13] = " |    |   /        \\  /    |";
    texture[14] = " |     \\  |        |  |    |";
    texture[15] = " |      |/          \\/     |";
    texture[16] = "  \\                       /";
    texture[17] = "   \\_       _           _/";
    texture[18] = "     --   -- ----__   --";
    texture[19] = "       ---___   ___---";
    texture[20] = "             ---";
    drawTexture(X, Y, 21);
}

void mars(int X, int Y)
{
    int n = 21;
    texture[0] = "qqqqqqqqqq___---___";
    texture[1] = "qqqqqqq_--        _--_";
    texture[2] = "qqqq_--      ___--    --_";
    texture[3] = "qqq/---___---      _     \\";
    texture[4] = "qq/      _        / \\     \\";
    texture[5] = "q| |    / \\      |   | .   |";
    texture[6] = "q|     /_/      /\\\\_/      |";
    texture[7] = "q|     /        \\/         |";
    texture[8] = "|            .              |";
    texture[9] = "|           .               |";
    texture[10] = "|                           |";
    texture[11] = "|____                       |";
    texture[12] = "|    ----||__      ____     |";
    texture[13] = "q|           ----_/    \\__ |";
    texture[14] = "q|-_                      -|";
    texture[15] = "q| _-  _--_                |";
    texture[16] = "qq\\     --       ______   /";
    texture[17] = "qqq\\_       __---      -_/";
    texture[18] = "qqqqq--  _--          --";
    texture[19] = "qqqqqqq---___   ___---";
    texture[20] = "qqqqqqqqqqqqq---";

    drawTexture(X, Y, n);
}

void jupiter(int X, int Y)
{
    texture[0] = "q";
    texture[1] = "qqqqqqqqq__-------__";
    texture[2] = "qqqqqq__-            -_";
    texture[3] = "qqqq_-            -    -_";
    texture[4] = "qqq/      -*-  - .-      \\";
    texture[5] = "qq/                       \\";
    texture[6] = "q| - - -   -  ---    --  - |";
    texture[7] = "q|              .    .     |";
    texture[8] = "q|=--==-==-=----==--==-=---|";
    texture[9] = "|                           |";
    texture[10] = "|  -- -  -  -  -    -    -  |";
    texture[11] = "|               .           |";
    texture[12] = "|--===--==--=--===--=-=-=-==|";
    texture[13] = "|                           |";
    texture[14] = "q|-=__              _--_   |";
    texture[15] = "q|    --=---===--==-_ 0_-==|";
    texture[16] = "q|                   --    |";
    texture[17] = "qq\\   -   -  -   - -o o- -/";
    texture[18] = "qqq\\                     /";
    texture[19] = "qqqq--_               _--";
    texture[20] = "qqqqqqq--__       __--";
    texture[21] = "qqqqqqqqqqq-------";
    int n = 22;
    drawTexture(X, Y, n);
}

void uran(int X, int Y) // Uran
{
    texture[0] = "qqqqqqqqq__-------__";
    texture[1] = "qqqqqq__-           -_";
    texture[2] = "qqqq_-                 -_";
    texture[3] = "qqq/                     \\";
    texture[4] = "qq/                       \\";
    texture[5] = "q|                         |";
    texture[6] = "q|                         |";
    texture[7] = "q|                         |";
    texture[8] = "|                           |";
    texture[9] = "|                           |";
    texture[10] = "|                           |";
    texture[11] = "|                           |";
    texture[12] = "|                           |";
    texture[13] = "q|                         |";
    texture[14] = "q|                         |";
    texture[15] = "q|                         |";
    texture[16] = "qq\\                       /";
    texture[17] = "qqq\\                     /";
    texture[18] = "qqqq--_               _--";
    texture[19] = "qqqqqqq--__       __--";
    texture[20] = "qqqqqqqqqqq-------";
    int n = 21;
    drawTexture(X, Y, n);
}

void saturn(int X, int Y)
{
    texture[0] = "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq...";
    texture[1] = "qqqqqqqqqqqqqqqqqqqqqqqqqqqq.......";
    texture[2] = "qqqqqqqqqqqqqqq__-------__ ....qq..";
    texture[3] = "qqqqqqqqqqqq__-           -_.qqqq.";
    texture[4] = "qqqqqqqqqq_-       / /       -_..q.";
    texture[5] = "qqqqqqqqq/         /  /       .\\q.";
    texture[6] = "qqqqqqqq/      / /       /  ... \\";
    texture[7] = "qqqqqqq|          / /      . .   |";
    texture[8] = "qqqqqqq|   // /   /    .. .      |";
    texture[9] = "qqqqqqq|  /           .. ....    |";
    texture[10] = "qqqqqq|  //         . . .  .      |";
    texture[11] = "qqqqqq|  /        .... .          |";
    texture[12] = "qqqqqq|  /         .  .        /  |";
    texture[13] = "qqqqqq|         . . .       / /   |";
    texture[14] = "qqqqq.|       .  . .       /   /  |";
    texture[15] = "qqq..qq|    . .. ..      / //    |";
    texture[16] = "qqq..qq|   . ..          /       |";
    texture[17] = "qq..qqq| .. ..     /    /  /     |";
    texture[18] = "q.q.qqq.\\  .         /  /       /";
    texture[19] = ".qqq.q.qq\\.   /   /    /       /";
    texture[20] = "..qqq.....--_   / / /       _--";
    texture[21] = "qqq..q..qqqqq--__       __--";
    texture[22] = "qqqqqqqqqqqqqqqqq-------";
    int n = 23;
    drawTexture(X, Y, n);
}

void neptune(int X, int Y)
{
    texture[0] = "q";
    texture[1] = "qqqqqqqqq__-------__";
    texture[2] = "qqqqqq__-           -_";
    texture[3] = "qqqq_-     _ _-        -_";
    texture[4] = "qqq/   ---- -            \\";
    texture[5] = "qq/                       \\";
    texture[6] = "q|                         |";
    texture[7] = "q|          =-             |";
    texture[8] = "q|                         |";
    texture[9] = "|              __           |";
    texture[10] = "|             _\\_\\__        |";
    texture[11] = "|              -     --     |";
    texture[12] = "|                           |";
    texture[13] = "|            .              |";
    texture[14] = "q|  =                      |";
    texture[15] = "q|                         |";
    texture[16] = "q|                         |";
    texture[17] = "qq\\ o                     /";
    texture[18] = "qqq\\     __---------___  /";
    texture[19] = "qqqq--_--     __      _--";
    texture[20] = "qqqqqqq--__       __--";
    texture[21] = "qqqqqqqqqqq-------";
    drawTexture(X, Y, 22);
}

void pluto(int X, int Y)
{
    texture[0] = "qqq__---__";
    texture[1] = "qq/    .  \\";
    texture[2] = "q| . . . . |";
    texture[3] = "|  . .  :  :|";
    texture[4] = "| . . .__: _|";
    texture[5] = "|  .. /  \\/ |";
    texture[6] = "q|. . |    |";
    texture[7] = "qq\\__/  __/";
    texture[8] = "qqqqq---";
    drawTexture(X, Y, 9);
}

void border(int X, int Y, int xSize, int ySize)
{
    int delta = X + Y * width;

    for (int i = 0; i < xSize; i += 1)
    {
        screen[delta + i + 2 + width * (ySize + 2)] = '-';
        screen[delta + i + 2 + width] = '-';
    }

    for (int i = 0; i < ySize; i += 1)
    {
        screen[delta + 1 + width * (i + 2)] = '|';
        screen[delta + 2 + width * (i + 2) + xSize] = '|';
    }

    screen[delta + 1 + width] = '+';
    screen[delta + 2 + xSize + width] = '+';
    screen[delta + 1 + width * (ySize + 2)] = '+';
    screen[delta + 2 + width * (ySize + 2) + xSize] = '+';
}

void shipExplosion(int X, int Y, int pos)
{
    int n = 0;
    if (pos == 0)
    {
        texture[0] = "q";
        texture[1] = "#\\_#";
        texture[2] = "q\\  \\";
        texture[3] = "<= 0 |-";
        texture[4] = "q#__#";
        texture[5] = "</q#";
        n = 6;
    }
    if (pos == 1)
    {
        texture[0] = "q#qq#";
        texture[1] = "#q##q#";
        texture[2] = "q#q##\\";
        texture[3] = "q<# # |-";
        texture[4] = "q#q#q#";
        texture[5] = "q<#q#";
        n = 6;
    }
    if (pos == 2)
    {
        texture[0] = "qq#q##";
        texture[1] = "q#q#qq#";
        texture[2] = "#qqq#qq#";
        texture[3] = "q#qq%q#";
        texture[4] = "qq#qq#";
        texture[5] = "q#qqqqq#";
        texture[6] = "qq#q#q#";
        n = 7;
    }
    if (pos == 3)
    {
        texture[0] = "qq#q%";
        texture[1] = "q%q#qq#";
        texture[2] = "#qqq#qq#";
        texture[3] = "q%qq=q#";
        texture[4] = "qq#qq%";
        texture[5] = "q%qqqqq%";
        texture[6] = "qq%q#q#";
        n = 7;
    }
    if (pos == 4)
    {
        texture[0] = "qq=q=";
        texture[1] = "q%q%qq%";
        texture[2] = "#qqq#qq%";
        texture[3] = "q=qq+q%";
        texture[4] = "qq#qq%";
        texture[5] = "q%qqqqq=";
        texture[6] = "qq=q#q=";
        n = 7;
    }
    if (pos == 5)
    {
        texture[0] = "qq+q+";
        texture[1] = "q%q%qq%";
        texture[2] = "%qqq%qq=";
        texture[3] = "q+qqqq=";
        texture[4] = "qq%qq%";
        texture[5] = "q=qqqqq=";
        texture[6] = "qq+q%q+";
        n = 7;
    }
    if (pos == 6)
    {
        texture[0] = "q";
        texture[1] = "qqq+";
        texture[2] = "=qqq=qq+";
        texture[3] = "q";
        texture[4] = "qq=qq=";
        texture[5] = "q+qqqqq+";
        texture[6] = "qqqq=";
        n = 7;
    }
    if (pos == 7)
    {
        texture[0] = "q";
        texture[1] = "q";
        texture[2] = "+qqq+";
        texture[3] = "q";
        texture[4] = "q";
        texture[5] = "q";
        texture[6] = "qqqq+";
        n = 7;
    }
    if (pos == 8)
    {
        texture[0] = "q";
        texture[1] = "q";
        texture[2] = "q";
        texture[3] = "q";
        texture[4] = "q";
        texture[5] = "q";
        texture[6] = "q";
        n = 7;
    }

    drawTexture(X, Y, n);
}

void asteroid(int X, int Y, int type)
{
    int n = 0;

    if (type == 0)
    {
        n = 6;
        texture[0] = "_/\\";
        texture[1] = "\\. |";
        texture[2] = "|  .\\";
        texture[3] = "[    >";
        texture[4] = "|  _/";
        texture[5] = "q\\/";
    }
    else if (type == 1)
    {
        n = 4;
        texture[0] = "qq_";
        texture[1] = "q/.\\";
        texture[2] = "|  .|";
        texture[3] = "q\\_/";
    }
    else if (type == 2)
    {
        n = 5;
        texture[0] = "q___";
        texture[1] = "/ . \\";
        texture[2] = "|   .\\";
        texture[3] = "q\\_  |";
        texture[4] = "qqq\\/";
    }
    else if (type == 3)
    {
        n = 6;
        texture[0] = "qqq_";
        texture[1] = "qq/.\\";
        texture[2] = "q/  .|";
        texture[3] = "|    |";
        texture[4] = "|   /";
        texture[5] = "q\\_/";
    }
    else if (type == 4)
    {
        n = 5;
        texture[0] = "qq_";
        texture[1] = "q/.\\";
        texture[2] = "|  .|";
        texture[3] = "|  /";
        texture[4] = "q\\/";
    }
    else if (type == 5)
    {
        n = 5;
        texture[0] = "q/\\";
        texture[1] = "| .|";
        texture[2] = "|  .\\";
        texture[3] = "|  _/";
        texture[4] = "q\\/";
    }

    drawTexture(X, Y, n);
}

void particleHit(int X, int Y, int num)
{
    if (num == 0)
    {
        screen[Y * width + X] = '#';
    }
    if (num == 1)
    {
        screen[Y * width + X + 1] = '%';
        screen[Y * width + X - 1] = '%';
        screen[(Y + 1) * width + X] = '%';
        screen[(Y - 1) * width + X] = '%';
    }
    if (num == 2)
    {
        screen[Y * width + X + 1] = '=';
        screen[Y * width + X - 1] = '=';
        screen[(Y + 1) * width + X] = '=';
        screen[(Y - 1) * width + X] = '=';
    }
    if (num == 3)
    {
        screen[Y * width + X + 1] = '+';
        screen[Y * width + X - 1] = '-';
        screen[(Y + 1) * width + X] = '+';
        screen[(Y - 1) * width + X] = '+';
    }
}

int isColided()
{
    for (int i = 0; i < 21; i += 1)
    {
        if (screen[(int)(y + shipCoordY[i]) * width + (int)x + shipCoordX[i]] != ' ' && screen[(int)(y + shipCoordY[i]) * width + (int)x + shipCoordX[i]] != '-')
        {
            colisionX = shipCoordX[i];
            colisionY = shipCoordY[i];
            return i;
        }
    }
    return 0;
}

void trail()
{
    borderText((int)x - 2, trailY[3], "%%");
    borderText((int)x - 2, trailY[3] + 2, "%%");
    borderText((int)x - 2, trailY[3] + 4, "%%");

    borderText((int)x - 4, trailY[2], "==");
    borderText((int)x - 4, trailY[2] + 2, "==");
    borderText((int)x - 4, trailY[2] + 4, "==");

    borderText((int)x - 5, trailY[1], "-");
    borderText((int)x - 5, trailY[1] + 2, "-");
    borderText((int)x - 5, trailY[1] + 4, "-");

    int temp1 = rand() % 3;
    int temp2 = rand() % 3;
    int temp3 = rand() % 3;

    if (temp1)
        borderText((int)x - 6, trailY[1] + 2, "-");
    else
        borderText((int)x - 4, trailY[2] + 2, "-");

    if (temp2)
        borderText((int)x - 6, trailY[1] + 4, "-");
    else
        borderText((int)x - 4, trailY[2] + 4, "-");

    if (temp3)
        borderText((int)x - 6, trailY[1], "-");
    else
        borderText((int)x - 4, trailY[2], "-");
}

void asteroidExplosion(int X, int Y, int type, int pos)
{
    int n = 0;
    if (type == 0 || type == 4 || type == 5)
    {
        if (pos == 0)
        {
            texture[0] = "_/\\";
            texture[1] = "\\# |";
            texture[2] = "|   #";
            texture[3] = "[    >";
            texture[4] = "|# #/";
            texture[5] = " \\/";
            n = 6;
        }
        if (pos == 1)
        {
            texture[0] = "_#\\";
            texture[1] = "# #|#";
            texture[2] = "|# # #";
            texture[3] = "[# ##>";
            texture[4] = "# # #";
            texture[5] = " #/#";
            n = 6;
        }
        if (pos == 2)
        {
            texture[0] = "_=#";
            texture[1] = "= =#=";
            texture[2] = "|= = =";
            texture[3] = "#= ==>";
            texture[4] = "= = =";
            texture[5] = " =/=";
            n = 6;
        }
        if (pos == 3)
        {
            texture[0] = "_+=";
            texture[1] = "+ +=+";
            texture[2] = "-+ - +";
            texture[3] = "=+ ++>";
            texture[4] = "+ + +";
            texture[5] = " +/+";
            n = 6;
        }
        if (pos == 4)
        {
            texture[0] = "+q=";
            texture[1] = "q-q=";
            texture[2] = "-qq-";
            texture[3] = "+q-qq+";
            texture[4] = "q";
            texture[5] = "q-+";
            n = 6;
        }
        if (pos == 5)
        {
            texture[0] = "q";
            texture[1] = "qqq+";
            texture[2] = "q";
            texture[3] = "+qqqqq";
            texture[4] = "q";
            texture[5] = "qq+";
            n = 6;
        }

    }
    if (type == 1)
    {
        if (pos == 0)
        {
            texture[0] = "qq_";
            texture[1] = "q#.\\";
            texture[2] = "|  .#";
            texture[3] = "q\\_/";
            n = 4;
        }
        if (pos == 1)
        {
            texture[0] = " #_";
            texture[1] = "#%#\#";
            texture[2] = "|# #%#";
            texture[3] = " \%/#";
            n = 4;
        }
        if (pos == 2)
        {
            texture[0] = " %=";
            texture[1] = "%=%+%";
            texture[2] = "|% %=%";
            texture[3] = " -=-%";
            n = 4;
        }
        if (pos == 3)
        {
            texture[0] = " =+";
            texture[1] = "=+=-=";
            texture[2] = "-= =+=";
            texture[3] = "  - =";
            n = 4;
        }
        if (pos == 4)
        {
            texture[0] = "+";
            texture[1] = "q-q+";
            texture[2] = "+q+";
            texture[3] = "q+";
            n = 4;
        }
        if (pos == 5)
        {
            texture[0] = "q";
            texture[1] = "qqq+";
            texture[2] = "q";
            texture[3] = "q+";
            n = 4;
        }
    }
    if (type == 2 || type == 3)
    {
        if (pos == 0)
        {
            texture[0] = " ___";
            texture[1] = "/ .#\\";
            texture[2] = "|   .\\";
            texture[3] = " #_ #|";
            texture[4] = "   \\/";
            n = 5;
        }
        if (pos == 1)
        {
            texture[0] = " __#";
            texture[1] = "/ #%#";
            texture[2] = "|# ##\\";
            texture[3] = "#%##%#";
            texture[4] = " # \\#";
            n = 5;
        }
        if (pos == 2)
        {
            texture[0] = " __%";
            texture[1] = "= +=%";
            texture[2] = "|# %+=";
            texture[3] = "%=%+=%";
            texture[4] = " % =%";
            n = 5;
        }
        if (pos == 3)
        {
            texture[0] = "qq-=";
            texture[1] = "-  +=";
            texture[2] = "-= +-+";
            texture[3] = "=+= =";
            texture[4] = " = +=";
            n = 5;
        }
        if (pos == 4)
        {
            texture[0] = "qq+ =";
            texture[1] = "-=";
            texture[2] = "qq=";
            texture[3] = "q=qq-";
            n = 5;
        }
        if (pos == 5)
        {
            texture[0] = "qq+";
            texture[1] = "q";
            texture[2] = "q";
            texture[3] = "q+";
            n = 5;
        }
    }
    drawTexture(X, Y, n);
}

void isAsteroidColided()
{
    int asteroidHeight[6] = { 6,4,5,6,5,5 };
    for (int i = 0; i < bulletXp.size(); i += 1)
    {
        for (int j = 0; j < asteroidX.size(); j += 1)
        {
            for (int k = 0; k < asteroidHeight[asteroidType[j]]; k += 1)
            {
                if (debug)
                    text(asteroidX[j], asteroidY[j] - 1, to_string(asteroidHp[j]) + " " + to_string(bulletXp[i]));

                if ((bulletXp[i] == asteroidX[j] || bulletXp[i] == asteroidX[j] + 1 || bulletXp[i] == asteroidX[j] + 2 || bulletXp[i] == asteroidX[j] + 3) && (bulletYp[i] == asteroidY[j] + k) && (asteroidHp[j] > 0))
                {
                    asteroidHp[j] -= 1;
                }
            }
        }
    }

    for (int j = 0; j < asteroidX.size(); j += 1)
    {
        if (asteroidHp[j] <= 0)
        {
            asteroidHp[j] -= 1;
        }
        if (asteroidHp[j] <= 0)
            asteroidExplosion(asteroidX[j], asteroidY[j], asteroidType[j], abs((int)asteroidHp[j]) / 6);
    }
}

void reset()
{
    ofset = 30, delay = 17, game = 0, debug = 0;
    len = 0, hp = 3, customAsteroidSpawnRate = 0, marsSpawn = 3500, asteroidBeltStart = 4500, asteroidBeltEnd = 6000, jupiterSpawn = 8000, saturnSpawn = 10000, uranusSpawn = 12000, neptuneSpawn = 14500, plutoSpawn = 16000, voyagerSpawn = 17000;
    voyagerX = 0, voyagerY = 0, victoryEnd = 0, set2 = 0, set3 = 0, set4 = 0, victory = 30, planetX = 0, planetY = 0, invincible = 0, invincibleE1 = 0, invincibleE2 = 0, colisionNum = 0, showLength = 1, event = 0, frame = 0, martianLength = 0, wave = 0, cooldown = 0, entityColisionX1 = 0, entityColisionX2 = 0, entityColisionY1 = 0, entityColisionY2 = 0;
    string str, planetType, button;

    x = width / 8, y = height / 2 - 3;

    trailY;

    dotX.clear();
    dotY.clear();
    dotSpeed.clear();

    asteroidX.clear();
    asteroidY.clear();
    asteroidType.clear();
    asteroidHp.clear();
    asteroidDestroyed.clear();

    bulletXp.clear();
    bulletYp.clear();
}

// 5000 1
void flight(int startPos, int destination)
{
    planetX = -70;
    invincible = 0;
    len = startPos;
    //len = 5300;
    game += 1;
    float colisionTime = 0.1, crash = 0, totalTime = 0, totalTimes = 0, shootCooldown = 5;
    int destinationPos = destination, animPos = 0, end = 0, temp = 15;
    bool isAnim = 0;
    destination = planets[destination];

    string dir;
    if (len > destination)
        dir = "LEFT";
    else
        dir = "RIGHT";

    while (true)
    {
        auto begin = chrono::steady_clock::now(); // timer

        int startHp = hp;
        if (event == 0)
        {
            if (dir == "RIGHT")
                len += 1;
            else
                len -= 1;
        }

        frame += 1;
        if (frame == 101)
            frame = 1;

        invincible -= 1;

        // controls
        if (isAnim == 0)
        {
            if (GetAsyncKeyState((unsigned short)'W') && y > 1)
                y -= 0.5;
            if (GetAsyncKeyState((unsigned short)'S') && y < height - 6)
                y += 0.5;
            if (GetAsyncKeyState((unsigned short)'A') && x > 0)
                x -= 0.4;
            if (GetAsyncKeyState((unsigned short)'D') && x < width - 8)
                x += 0.4;
            if (GetAsyncKeyState((unsigned short)32) && shootCooldown < 0)
            {
                bulletXp.push_back(x + 7);
                bulletYp.push_back(y + 2);
                shootCooldown = 10;
            }
        }
        else
        {
            if (animPos == 1)
            {
                if (y > height / 2 - 4)
                {
                    y -= 0.5;
                }
                if (y < height / 2 - 4)
                {
                    y += 0.5;
                }
                if (x > width * 0.3)
                {
                    x -= 0.4;
                }
                if (x < width * 0.3)
                {
                    x += 0.4;
                }
            }
        }

        shootCooldown -= 1;
        if (_kbhit())
            button = getKey();
        else
            button = "no button lmao";

        // trailY
        trailY.push_back(y);
        trailY.erase(trailY.begin());
        // generating dots

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////

        if (len + 100 == destination || len - 100 == destination)
        {
            isAnim = 1;
            animPos = 1;
            invincible = 30000;
        }

        if (event)
            temp = 40;
        else
            temp = 15;
        if (frame % temp == 0)
        {
            dotX.push_back(width);
            dotSpeed.push_back(0);
            dotY.push_back(rand() % (height - 2) + 1);
            dotX.push_back(width);
            dotSpeed.push_back(1);
            dotY.push_back(rand() % (height - 2) + 1);
        }
        // generating asteroids
        if (len < asteroidBeltEnd && len > asteroidBeltStart)
            temp = 15;
        else
            temp = 60;
        if (customAsteroidSpawnRate > 0)
        {
            temp = customAsteroidSpawnRate;
        }
        if (len % temp == 0 && isAnim == 0)
        {
            if (len < asteroidBeltEnd && len > asteroidBeltStart && rand() % 2 == 0)
            {
                asteroidY.push_back(rand() % (height - 8) + 1);
                asteroidX.push_back(width + rand() % 10 + 8);
                temp = rand() % 6;
                asteroidType.push_back(temp);
                asteroidHp.push_back(1);
            }
            asteroidY.push_back(rand() % (height - 8) + 1);
            asteroidX.push_back(width);
            temp = rand() % 6;
            asteroidType.push_back(temp);
            asteroidHp.push_back(1);
        }

        // putting Mercury
        if (len == mercurySpawn)
        {
            planetX = width;
            planetY = height / 4;
            planetType = "mercury";
        }
        // putting Venus
        if (len == venusSpawn)
        {
            planetX = width;
            planetY = height / 4;
            planetType = "venus";
        }
        // putting Earth
        if (len == earthSpawn)
        {
            planetX = width;
            planetY = height / 4;
            planetType = "earth";
        }
        // putting Mars
        if (len == marsSpawn)
        {
            planetX = width;
            planetY = height / 4;
            planetType = "mars";
        }
        // putting Jupiter
        if (len == jupiterSpawn)
        {
            planetX = width;
            planetY = height / 4;
            planetType = "jupiter";
        }
        // putting Saturn
        if (len == saturnSpawn)
        {
            planetX = width;
            planetY = height / 3;
            planetType = "saturn";
        }
        // putting Uranus
        if (len == uranusSpawn)
        {
            planetX = width;
            planetY = height - 25;
            planetType = "uranus";
        }
        // putting Uranus
        if (len == neptuneSpawn)
        {
            planetX = width;
            planetY = height / 4;
            planetType = "neptune";
        }
        // putting Uranus
        if (len == plutoSpawn)
        {
            planetX = width;
            planetY = height / 4;
            planetType = "pluto";
        }
        // putting voyager 2
        if (len == voyagerSpawn)
        {
            voyagerX = width;
            voyagerY = height * 0.75;
        }

        // moving flighter bullets
        for (int i = 0; i < bulletXp.size(); i += 1)
        {
            if (bulletXp[i] > width - 2)
            {
                bulletXp.erase(bulletXp.begin() + i);
                bulletYp.erase(bulletYp.begin() + i);
            }
            else
            {
                bulletXp[i] += 2;
            }
        }
        // moving dots
        for (int i = 0; i < dotX.size(); i += 1)
        {
            if (dotX[i] <= 0)
            {
                dotX.erase(dotX.begin() + i);
                dotY.erase(dotY.begin() + i);
                dotSpeed.erase(dotSpeed.begin() + i);
            }
            else if (dotSpeed[i] == 1)
            {
                dotX[i] -= 1;
            }
            else if (dotSpeed[i] == 0 && len % 2 == 0)
            {
                dotX[i] -= 1;
            }
        }
        // moving asteroids (tag)
        for (int i = 0; i < asteroidX.size(); i += 1)
        {
            if (asteroidX[i] > -10 && asteroidHp[i] > -50)
                asteroidX[i] -= 1;
            else
            {
                asteroidX.erase(asteroidX.begin() + i);
                asteroidY.erase(asteroidY.begin() + i);
                asteroidType.erase(asteroidType.begin() + i);
                asteroidHp.erase(asteroidHp.begin() + i);
            }
        }
        // moving planet
        if (planetX > -70 && len % 2 == 0)
        {
            planetX -= 1;
        }
        // moving voyager
        if (voyagerX > -10 && len % 2 == 0)
        {
            voyagerX -= 1;
        }

        // putting planet on screen, zLayer = -1, bg
        if (planetType == "mercury")
            mercury(planetX, planetY);
        if (planetType == "venus")
            venus(planetX, planetY);
        if (planetType == "earth")
            earth(planetX, planetY);
        if (planetType == "mars")
            mars(planetX, planetY);
        if (planetType == "uranus")
            uran(planetX, planetY);
        if (planetType == "saturn")
            saturn(planetX, planetY);
        if (planetType == "jupiter")
            jupiter(planetX, planetY);
        if (planetType == "neptune")
            neptune(planetX, planetY);
        if (planetType == "pluto")
            pluto(planetX, planetY);
        // putting dots on screen, bg layer
        for (int i = 0; i < dotX.size(); i += 1)
            screen[dotY[i] * width + dotX[i]] = '-';
        // putting asteroids on screen, top layer
        for (int i = 0; i < asteroidX.size(); i += 1)
        {
            if (asteroidHp[i] > 0)
                asteroid(asteroidX[i], asteroidY[i], asteroidType[i]);
        }
        // putting flighter bullets on screen
        for (int i = 0; i < bulletXp.size(); i += 1)
            text(bulletXp[i], bulletYp[i], "=");

        if (len < asteroidBeltEnd && len > asteroidBeltStart)
        {
            str = "Asteroid belt";
            text(width / 2 - str.length() / 2, height - 3, str);
        }
        // colision detection, before ship on screen
        if (invincible < 0)
        {
            colisionNum = isColided();
        }
        if (colisionNum != 0)
        {
            colisionTime += 0.2;
        }
        else
        {
            colisionTime = -1;
        }

        // bullet and asteroid colision
        isAsteroidColided();

        if (colisionNum != 0 && invincible < 0 && hp > 0)
        {
            hp -= 1;
            invincible = 30;
        }
        //putting ship, top layer
        if (isAnim == 0)
        {
            if (hp > 0)
            {
                trail();
                ship(x, y);
            }
            else  // crash
            {
                crash += 0.2;
                if (crash < 8 && crash > -1)
                    shipExplosion(x - 2, y - 2, (int)crash);
                x -= 0.25;
            }
        }
        else
        {
            str = "Distance to planet : " + to_string(abs(planetX - 30));
            if (planetX > 0)
                text(width / 2 - str.length() / 2, height - 2, str);
            //text(3,3, to_string(planetX));

            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (-3 == planetX - 30)
            {
                landingAnimation(destinationPos);
                walk(destinationPos);
            }
            if (x < planetX - 6)
            {
                if (x > planetX - 5)
                {
                    end = 1;
                    veryVerySmallShip(x, y);
                }
                else if (x > planetX - 15)
                {
                    verySmallShip(x, y);
                }
                else if (x > planetX - 30)
                {
                    smallShip(x, y);
                }
                else if (x > planetX - 2281337)
                {
                    trail();
                    ship(x, y);
                }
            }
            else if (end == 0 && planetX < 0)
            {
                trail();
                ship(x, y);
            }
        }

        particleHit(x + shipCoordX[colisionNum], y + shipCoordY[colisionNum], (int)colisionTime); // hit particle


        /// info ///
        if (showLength)
        {
            str = "Distance : " + to_string(len) + ", ship health : " + to_string(hp);
            text(width / 2 - str.length() / 2, height - 1, str);
        }
        else
        {
            str = "Ship health : " + to_string(hp);
            text(width / 2 - str.length() / 2, height - 1, str);
        }

        /// Sleep(rand()%(delay-1)); // lag creator for test

        /// final

        auto end = chrono::steady_clock::now();// timer end
        auto elapsed_ms = chrono::duration_cast<chrono::milliseconds>(end - begin);
        int frameRenderTime = elapsed_ms.count(); // anti lag
        if (frameRenderTime > 0)
            text(0, 0, "!");
        text(1, 0, "Frame render time: " + to_string(frameRenderTime) + " ms, 60 frames delay : " + to_string(totalTime));

        totalTime += frameRenderTime;
        totalTimes += 1;
        if (totalTimes == 60)
        {
            totalTime = 0;
        }

        if (delay - frameRenderTime > 1)
            Sleep(delay - frameRenderTime);
        else
            Sleep(1);

        // Sleep(delay);

        // planet names
        if (planetX > -30)
        {
            if (planetType == "mercury")
                str = "Mercury";
            else if (planetType == "venus")
                str = "Venus";
            else if (planetType == "earth")
                str = "Earth";
            else if (planetType == "moon")
                str = "Moon";
            else if (planetType == "mars")
                str = "Mars";
            else if (planetType == "uranus")
                str = "Uranus(Yes it is circle)";
            else if (planetType == "saturn")
                str = "Saturn";
            else if (planetType == "jupiter")
                str = "Jupiter";
            else if (planetType == "neptune")
                str = "Neptune";
            else if (planetType == "pluto")
                str = "Pluto";
            else
                str = "";
            text(width / 2 - str.length() / 2, height - 4, str);
        }

        if (GetAsyncKeyState((unsigned short)27))
        {
            str = "Paused";
            text(width / 2 - str.length() / 2, height / 2, str);
            str = "Press enter to exit";
            text(width / 2 - str.length() / 2, height / 2 + 2, str);
            render();
            button = getKey();
            if (button == "enter")
                mainMenu();
        }

        /// death ///
        if (hp <= 0)
        {
            str = "You died!";
            text(width / 2 - str.length() / 2, height / 2, str);
            str = "Enter - try again, esc - main menu";
            text(width / 2 - str.length() / 2, height / 2 + 2, str);
            if (_kbhit())
                button = getKey();
            if (button == "enter")
            {
                reset();
                map(3);
            }
            if (button == "esc")
            {
                game = 0;
                reset();
                mainMenu();
            }
        }

        render();
    }
}

void map(int pos)
{
    emptyBuffer();
    int start = pos;
    int menu = 1, firstTime = 1, errTime = 0;
    string button = "sds";
    while (true)
    {
        if (!firstTime && _kbhit())
            button = getKey();

        if (firstTime)
            Sleep(200);

        firstTime = 0;

        if (GetAsyncKeyState((unsigned short)'D') && pos > 1)
            pos -= 1;
        if (GetAsyncKeyState((unsigned short)'A') && pos < 8)
            pos += 1;
        if (GetAsyncKeyState((unsigned short)13))
        {
            if (start != pos)
            {
                liftOfAnimation(start);
                flight(planets[start], pos);
            }
            else
                errTime = 20;
        }
        if (GetAsyncKeyState((unsigned short)27))//esc
            walk(start);

        if (errTime > 0)
        {
            str = "You are already on this planet";
            text(width/2-str.length()/2,2,str);
        }

        errTime -= 1;

        shipMods(width/2-14, height/3);

        border(0, height - 18, width - 4, 15);



        texture[0] = "                                                                     ..";
        texture[1] = "                                                                     .                                           _--";
        texture[2] = "                                                                    ..                                          /";
        texture[3] = "                                                                   ..                                          |";
        texture[4] = "                                     _ /             _             .                                           |";
        texture[5] = "                                    / /             / \\            ..                                         |";
        texture[6] = "              <>       /\\          | / |           |   |            .        *      0       o        *        |";
        texture[7] = "                       \\/           /_/             \\_/            . .                                        |";
        texture[8] = "                                                                    ..                                         |";
        texture[9] = "                                                                     ..                                        |";
        texture[10] = "                                                                     ..                                         \\_";
        texture[11] = "                                                                      ..                                          --";
        texture[12] = "                                                                      ..";

        drawTexture(12, height - 15, 13);

        string str = "MAP";
        text(21 + (width - 23) / 2 - str.length() / 2, height - 17, str);

        str = "INFO";
        text(9, height - 17, str);

        if (pos == 1)
        {
            text(111, 41, "[");
            text(115, 41, "]");
            texture[0] = " Name:Mercury     ";
            texture[1] = "                  ";
            texture[2] = " Temperature:350K ";
            texture[3] = "                  ";
            texture[4] = " Minerals:        ";
            texture[5] = "                  ";
            texture[6] = " -Fe              ";
            texture[7] = " -Fe2O3           ";
            texture[8] = " -Cu              ";
            texture[9] = "                  ";
            texture[10] = " No atmosphere,  ";
            texture[11] = " solid surface   ";
            drawTexture(2, height - 15, 12);
        }
        if (pos == 2)
        {
            text(102, 41, "[");
            text(106, 41, "]");
            texture[0] = " Name:Venus     ";
            texture[1] = "                  ";
            texture[2] = " Temperature:737K ";
            texture[3] = " (CAP suit needed)";
            texture[4] = "                  ";
            texture[5] = " Minerals:        ";
            texture[6] = " -idk             ";
            texture[7] = " -idk             ";
            texture[8] = "                  ";
            texture[9] = " Extreme pressure ";
            texture[10] = " No atmosphere,   ";
            texture[11] = " solid surface    ";
            drawTexture(2, height - 15, 12);
        }
        if (pos == 3)
        {
            text(94, 41, "[");
            text(98, 41, "]");
            texture[0] = " Name:Earth     ";
            texture[1] = "                  ";
            texture[2] = " Temperature:280K ";
            texture[3] = "                  ";
            texture[4] = " Minerals:        ";
            texture[5] = "                  ";
            texture[6] = " -idk             ";
            texture[7] = " -idk             ";
            texture[8] = "                  ";
            texture[9] = " Atmosphere,";
            texture[10] = " solid surface";
            drawTexture(2, height - 15, 11);
        }
        if (pos == 4)
        {
            text(87, 41, "[");
            text(91, 41, "]");
            texture[0] = " Name:Mars     ";
            texture[1] = "                  ";
            texture[2] = " Temperature:210K ";
            texture[3] = "                  ";
            texture[4] = " Minerals:        ";
            texture[5] = "                  ";
            texture[6] = " -Sn             ";
            texture[7] = " -Cu             ";
            texture[8] = "                  ";
            texture[9] = " No atmosphere,";
            texture[10] = " solid surface";
            drawTexture(2, height - 15, 11);
        }
        if (pos == 5)
        {
            text(61, 41, "[");
            text(69, 41, "]");
            texture[0] = " Name:Jupiter     ";
            texture[1] = "                  ";
            texture[2] = " Temperature:yes ";
            texture[3] = "                  ";
            texture[4] = " Minerals:        ";
            texture[5] = "                  ";
            texture[6] = " -Sn             ";
            texture[7] = " -Cu             ";
            texture[8] = "                  ";
            texture[9] = " No atmosphere,";
            texture[10] = " solid surface";
            drawTexture(2, height - 15, 11);
        }
        if (pos == 6)
        {
            text(45, 41, "[");
            text(53, 41, "]");
            texture[0] = " Name:Saturn     ";
            texture[1] = "                  ";
            texture[2] = " Temperature:yes ";
            texture[3] = "                  ";
            texture[4] = " Minerals:        ";
            texture[5] = "                  ";
            texture[6] = " -Sn             ";
            texture[7] = " -Cu             ";
            texture[8] = "                  ";
            texture[9] = " No atmosphere,";
            texture[10] = " solid surface";
            drawTexture(2, height - 15, 11);
        }
        if (pos == 7)
        {
            text(33, 41, "[");
            text(38, 41, "]");
            texture[0] = " Name:Uranus     ";
            texture[1] = "                  ";
            texture[2] = " Temperature:yes ";
            texture[3] = "                  ";
            texture[4] = " Minerals:        ";
            texture[5] = "                  ";
            texture[6] = " -Sn             ";
            texture[7] = " -Cu             ";
            texture[8] = "                  ";
            texture[9] = " No atmosphere,";
            texture[10] = " solid surface";
            drawTexture(2, height - 15, 11);
        }
        if (pos == 8)
        {
            text(24, 41, "[");
            text(29, 41, "]");
            texture[0] = " Name:Neptune     ";
            texture[1] = "                  ";
            texture[2] = " Temperature:yes ";
            texture[3] = "                  ";
            texture[4] = " Minerals:        ";
            texture[5] = "                  ";
            texture[6] = " -Sn             ";
            texture[7] = " -Cu             ";
            texture[8] = "                  ";
            texture[9] = " No atmosphere,";
            texture[10] = " solid surface";
            drawTexture(2, height - 15, 11);
        }

        text(20, height - 17, "+");
        text(20, height - 1, "+");

        for (int i = 0; i < 15; i += 1)
        {
            text(20, height - 16 + i, "|");
        }


        text(1, 1, to_string(start) + " " + to_string(pos));

        render();
        Sleep(200);
    }
}

void cloud(int X, int Y, int type)
{
    texture[0] = "qqqq.--";
    texture[1] = "q.+(   )";
    texture[2] = " (   . `+";
    texture[3] = "(   (   .)";
    texture[4] = "q`-.___.`";
    drawTexture(X, Y, 5);
}

void sunMoon(int X, int Y, int type)
{
    if (type == 1)
    {
        texture[0] = "qqqq|";
        texture[1] = "q\\qq_qq/";
        texture[2] = "qqq/ \\";
        texture[3] = "-q|   |q-";
        texture[4] = "qqq\\_/";
        texture[5] = "q/qqqqq\\";
        texture[6] = "qqqq|";
        drawTexture(X, Y, 7);
    }
    else
    {
        texture[0] = "  _  ";
        texture[1] = " /.\\";
        texture[2] = "|  .|";
        texture[3] = " \\_/";
        drawTexture(X, Y, 4);
    }
}

void shop(int start)
{
    while (true)
    {
        text(2, 2, "shop");

        if (GetAsyncKeyState((unsigned short)27))//esc
            walk(start, 72);

        render();
        Sleep(40);
        emptyBuffer();
    }
}

void walk(int planet, int startPos)
{
    if (planet == 4)
    {
        vector <vector <string>> chunk(10, vector<string>(10, ""));
        vector <vector <string>> chunkTextured(10, vector<string>(10, ""));

        chunk[0] = {
    " ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "------                                                --------------------------------                                            ",
    "      ---------                    -------------------                                -----------------------                     ",
    "               --------------------                                                                          ---------------------"
        };
        chunkTextured[0] = {
    " ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "0000000                                               000000000000000000000000000000000                                           ",
    "      0000000000                   00000000000000000000                               000000000000000000000000                    ",
    "               000000000000000000000                                                                         000000000000000000000"

        };
        chunk[1] = {
    " ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "----------------                                                                                             ---------------------",
    "                -------------                           fff                          ------------------------                      ",
    "                             --------------------------------------------------------                                             "

        };
        chunkTextured[1] = {
    " ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                          |                                                                       ",
    "                                                         / \\                                                                      ",
    "                                                        |   |                                                                     ",
    "                                                        | 0 |                                                                     ",
    "                                                        |   |                                                                      ",
    "0000000000000000                                  _____//-|-\\\\_____                                          0000000000000000000000",
    "                0000000000000                    /| | | | | | | | |\\                 000000000000000000000000                     ",
    "                             00000000000000000000000000000000000000000000000000000000                                             "
        };
        chunk[2] = {
    " ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                   --------------------------                                                                     ",
    "                 ------------------                          -------------------                                                  ",
    "-----------------                                                               ---------------------                             ",
    "                                                                                                     -----------------------------"
        };
        chunkTextured[2] = {
    " ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                   000000000000000000000000000                                                                    ",
    "                 0000000000000000000                         00000000000000000000                                                 ",
    "000000000000000000                                                              0000000000000000000000                            ",
    "                                                                                                     00000000000000000000000000000",
        };
        chunk[3] = {
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                        ------------------------------------------------------                                          ----------",
    "------------------------                                                      ------------------------------------------          "
        };
        chunkTextured[3] = {
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                                                                                                                                  ",
    "                       0000000000000000000000000000000000000000000000000000000                                          0000000000",
    "000000000000000000000000                                                      0000000000000000000000000000000000000000000         "
        };

        vector <int> interactionsX;
        vector <char> interactionsType;

        const int lim = width * height + 1;

        if (startPos == 0)
            playerX = 187;
        else
            playerX = startPos;

        const int heightT = 50;
        const int widthT = 780;
        char terrain[heightT][widthT] = { ' ' };

        int amountOfChunks = 4;
        int deltaY = height / 2;

        // height calculation
        vector <int> chunkHeight(9999, 0);
        for (int i = 0; i < chunk.size(); i += 1)
        {
            for (int k = 0; k < chunk[i].size(); k += 1)
            {
                for (int j = 0; j < chunk[i][k].size(); j += 1)
                {
                    if (chunk[i][k][j] == '-')
                        chunkHeight[j + width * i] = k;
                    if (chunk[i][k][j] == 'f')
                    {
                        interactionsX.push_back(j + width * i);
                        interactionsType.push_back('f');
                    }
                    if (chunk[i][k][j] == 's')
                    {
                        interactionsX.push_back(j + width * i);
                        interactionsType.push_back('s');
                    }
                }
            }
        }

        float smX = width, smY = height * 0.2;
        int smType = 3;

        while (true)
        {
            jumpTime -= 1;

            if (GetAsyncKeyState((unsigned short)'A') && playerX > 0)
                playerX -= 1;
            if (GetAsyncKeyState((unsigned short)'D') && playerX < width * amountOfChunks - 1)
                playerX += 1;
            if (GetAsyncKeyState((unsigned short)32) && jumpTime < -5)
                jumpTime = 10;
            if (GetAsyncKeyState((unsigned short)13) || GetAsyncKeyState((unsigned short)'E'))
            {
                for (int i = 0; i < interactionsX.size(); i += 1)
                {
                    if (playerX == interactionsX[i])
                    {
                        if (interactionsType[i] == 'f')
                        {
                            map(4);
                        }
                        if (interactionsType[i] == 's')
                        {
                            shop(3);
                        }
                    }
                }
            }

            text(2, 2, to_string(playerX));

            for (int i = 0; i < chunkTextured.size(); i += 1)
            {
                for (int k = 0; k < chunkTextured[i].size(); k += 1)
                {
                    for (int j = 0; j < chunkTextured[i][k].size(); j += 1)
                    {
                        terrain[k][j + width * i] = chunkTextured[i][k][j];
                    }
                }
            }

            int k = 0;
            int temp2 = 0;

            if (playerX < 65)
                temp2 = 0;
            else if (playerX > width * amountOfChunks - 65)
                temp2 = width * (amountOfChunks - 1);
            else
                temp2 = playerX - 65;

            for (int i = 0; i < heightT; i += 1)
            {
                for (int j = temp2; j < widthT + playerX + 65; j += 1)
                {
                    screen[(i + deltaY) * width + k] = terrain[i][j];
                    k += 1;
                }
                k = 0;
            }

            //sun and moon
            smX -= 0.05;
            sunMoon(smX, smY, smType % 2);
            if (smX > 65)
                smY -= 0.005;
            else
                smY += 0.005;

            if (smX < -10)
            {
                smX = width;
                smY = height * 0.2;
                smType += 1;
            }



            //jump and player render
            //borders
            int deltaX = 0;
            // left
            if (playerX < 65)
                deltaX = playerX - 65;
            // right
            else if (playerX > width * amountOfChunks - 65)
                deltaX = playerX - (amountOfChunks - 0) * width + 65;
            else
                deltaX = 0;


            if (jumpTime < 0)
                screen[(int)((chunkHeight[playerX] + deltaY - 1) * width + 65 + deltaX)] = '@';
            else
            {
                if (jumpTime == 10 || jumpTime == 9)
                    screen[(int)((chunkHeight[playerX] + deltaY - 1 - 1) * width + 65 + deltaX)] = '@';
                if (jumpTime == 8 || jumpTime == 7)
                    screen[(int)((chunkHeight[playerX] + deltaY - 1 - 2) * width + 65 + deltaX)] = '@';
                if (jumpTime == 6 || jumpTime == 5 || jumpTime == 4)
                    screen[(int)((chunkHeight[playerX] + deltaY - 1 - 3) * width + 65 + deltaX)] = '@';
                if (jumpTime == 2 || jumpTime == 3)
                    screen[(int)((chunkHeight[playerX] + deltaY - 1 - 2) * width + 65 + deltaX)] = '@';
                if (jumpTime == 0 || jumpTime == 1)
                    screen[(int)((chunkHeight[playerX] + deltaY - 1 - 1) * width + 65 + deltaX)] = '@';
            }

            for (int i = 0; i < interactionsX.size(); i += 1)
            {
                if (playerX == interactionsX[i])
                {
                    if (interactionsType[i] == 'f')
                    {
                        str = "Press enter to hop on the rocket";
                        text(width / 2 - str.length() / 2, height - 2, str);
                    }
                }
            }

            render();
            Sleep(40);
            emptyBuffer();
        }
    }
    else
    if (planet == 3)
    {
        vector <vector <string>> chunk(10, vector<string>(10, ""));
        vector <vector <string>> chunkTextured(10, vector<string>(10, ""));

        chunk[0] = {
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    "                                                                      ssss                ----------------------------------------",
    "--------------------------------                                --------------------------",
    "                                --------------------------------",
        };
        chunkTextured[0] = {
    " ",
    " ",
    "                                                                     ______                                                       ",
    "                                                                   _/      \\_                                                    ",
    "                                                                  /   shop   \\                                                   ",
    "                                                                  \\ __----__ /                                                   ",
    "                                                                   |  ____  |                                                     ",
    "                                                                   |  |  |  |                                                     ",
    "                                                                   |  |  |  |                                                     ",
    "                                                                   |  |  |  |             OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO                                OOOOOOOOOOOOOOOOOOOOOOOOOOoooooooooooooooooooooooooooooooooooooooo",
    "oooooooooooooooooooooooooooooooOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOoooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
    "oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo|oo|o|o|o|||o||o|||o|||o||o|o||o|o||o|",
    "||o||||o||o||||o|||o||o|o|||o|o||o|o|||o|||oo|o||o||o||o|||o|||o|o||o|||oo||o||o||||o||o|o|||.|..||.||.||.||.||.|.||.||.|.|.|.|.|.",
    ".|.|..||.||.||.|.||.|.|.|.||.||.|.|.|.|.|.||.||.||.|.||.|||.||.||.|||.||.|.|.|.||.|||.|||.|.......................................",
    ".............................................................................................. ... .  . ..  . .  . . . . . . . . .",
    ". . . . . . . . . . . . . . . . . . . .  . . . ..  . . . . . . . ... . .  . .  . .  .  .  . .                           "

        };
        chunk[1] = {
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    "                                                      fffffff",
    "                                   --------------------------------------------",
    "-----------------------------------                                            ---------------------------------------------------"
        };
        chunkTextured[1] = {
    "            .--                                                                                                                   ",
    "         .+(   )                                         |                                                                        ",
    "         (     `)                                       / \\                                                                       ",
    "        (       ))                                     |   |                                                                      ",
    "         `-___.'                                       | 0 |                                                                      ",
    "           \\||                                         |   |                                                _                     ",
    "            ||/                                  _____//-|-\\\\_____                                         / \\                    ",
    "            ||                                  /  |  |  |  |  |  \\                                        \\_/                    ",
    "            | \\                    OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO                             |                     ",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOooooooooooooooooooooooooooooooooooooooooooooOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
    "oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
    "ooooooooooooooooooooooooooooooooooooo|o|o|||o||oo|o|o|o||o|o|||o||||o|||o||o|ooooooooooooooooooooooooooooooooooooooooooooooooooooo",
    "o|o|o|o||o||o||o||o||o|||o||o||o||o||o.||.|.|.||.||.|.||.|.|..|..|...|.||..||o|o|o|||oo||o||o||o|o|o||o||o|||o||o|o||o||o|o|o||o|o",
    "|.|.|.|.|.|.|.|..||.|.|.||.|.||.||.|.|.|....|...|...|..|..|...|..|...|.|.|..|.|.|.|.|||.|.||.|||.||.|.|.|.||.||.||.|.||.||.|.||.|.",
    "................................................... .  . . . .....................................................................",
    ". . . . . . . . . .  . .  .   .  .. .. . . . . .                .    .  .  .  .  .  .   .  .  .  .  . .  .  . .. .  . . . . . . .."
        };
        chunk[2] = {
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    "----------------------------------------------------------------------------------------------------------------------------------"
        };
        chunkTextured[2] = {
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    "##################################################################################################################################",
    "##################################################################################################################################",
    "##################################################################################################################################",
    "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
    "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
    "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
    "..................................................................................................................................",
    "..................................................................................................................................",
    ".................................................................................................................................."
        };
        chunk[3] = {
    " ",
    "----------------------------------------------------------------------------------------------------------------------------------"
        };
        chunkTextured[3] = {
    " ",
    "##################################################################################################################################"
        };
        chunk[4] = {
    " ",
    "-                                                                                                                                 ",
    " -                                                                                                                                ",
    "  -                                                                                                                               ",
    "   -                                                                                                                              ",
    "    -                                    --                                                                                       ",
    "     -                                  -  -                                                                                      ",
    "      -                                -    -                    |                                                                ",
    "       -                              -      -          --------------------------------------------------------------------------",
    "        -                            -        -        -                                                                          ",
    "         -                          -          -      -                                                                           ",
    "          -                        -            -    -                                                                            ",
    "           -                      -              -  -                                                                             ",
    "            -                    -                --                                                                              ",
    "             -                  -                                                                                                 ",
    "              -                -                                                                                                  ",
    "               -              -                                                                                                   ",
    "                -            -                                                                                                    ",
    "                 -          -                                                                                                     ",
    "                  -        -                                                                                                      ",
    "                   -      -                                                                                                       ",
    "                    ------                                                                                                        "
        };
        chunkTextured[4] = {
    " ",
    "#                                                                                                                                 ",
    " #                                                                                                                                ",
    "  #                                                                                                                               ",
    "   #                                                                                                                              ",
    "    #                                    ##                                                                                       ",
    "     #                                  #  #                                                                                      ",
    "      #                                #    #                    |                                                                ",
    "       #                              #      #          ##########################################################################",
    "        #                            #        #        #                                                                          ",
    "         #                          #          #      #                                                                           ",
    "          #                        #            #    #                                                                            ",
    "           #                      #              #  #                                                                             ",
    "            #                    #                ##                                                                              ",
    "             #                  #                                                                                                 ",
    "              #                #                                                                                                  ",
    "               #              #                                                                                                   ",
    "                #            #                                                                                                    ",
    "                 #          #                                                                                                     ",
    "                  #        #                                                                                                      ",
    "                   #      #                                                                                                       ",
    "                    ######                                                                                                        "
        };

        vector <int> interactionsX;
        vector <char> interactionsType;

        const int lim = width * height + 1;

        if (startPos == 0)
            playerX = 186;
        else
            playerX = startPos;

        const int heightT = 50;
        const int widthT = 780;
        char terrain[heightT][widthT] = { ' ' };

        int amountOfChunks = 5;
        int deltaY = height / 2;

        // height calculation
        vector <int> chunkHeight(9999, 0);
        for (int i = 0; i < chunk.size(); i += 1)
        {
            for (int k = 0; k < chunk[i].size(); k += 1)
            {
                for (int j = 0; j < chunk[i][k].size(); j += 1)
                {
                    if (chunk[i][k][j] == '-')
                        chunkHeight[j + width * i] = k;
                    if (chunk[i][k][j] == 'f')
                    {
                        interactionsX.push_back(j + width * i);
                        interactionsType.push_back('f');
                    }
                    if (chunk[i][k][j] == 's')
                    {
                        interactionsX.push_back(j + width * i);
                        interactionsType.push_back('s');
                    }
                }
            }
        }

        float smX = width, smY = height * 0.2;
        int smType = 3;

        while (true)
        {
            jumpTime -= 1;

            if (GetAsyncKeyState((unsigned short)'A') && playerX > 0)
                playerX -= 1;
            if (GetAsyncKeyState((unsigned short)'D') && playerX < width * amountOfChunks - 1)
                playerX += 1;
            if (GetAsyncKeyState((unsigned short)32) && jumpTime < -5)
                jumpTime = 10;
            if (GetAsyncKeyState((unsigned short)13) || GetAsyncKeyState((unsigned short)'E'))
            {
                for (int i = 0; i < interactionsX.size(); i += 1)
                {
                    if (playerX == interactionsX[i])
                    {
                        if (interactionsType[i] == 'f')
                        {
                            map(3);
                        }
                        if (interactionsType[i] == 's')
                        {
                            shop(3);
                        }
                    }
                }
            }

            text(2, 2, to_string(playerX));

            for (int i = 0; i < chunkTextured.size(); i += 1)
            {
                for (int k = 0; k < chunkTextured[i].size(); k += 1)
                {
                    for (int j = 0; j < chunkTextured[i][k].size(); j += 1)
                    {
                        terrain[k][j + width * i] = chunkTextured[i][k][j];
                    }
                }
            }

            int k = 0;
            int temp2 = 0;

            if (playerX < 65)
                temp2 = 0;
            else if (playerX > width * amountOfChunks - 65)
                temp2 = width * (amountOfChunks - 1);
            else
                temp2 = playerX - 65;

            for (int i = 0; i < heightT; i += 1)
            {
                for (int j = temp2; j < widthT + playerX + 65; j += 1)
                {
                    screen[(i + deltaY) * width + k] = terrain[i][j];
                    k += 1;
                }
                k = 0;
            }

            //sun and moon
            smX -= 0.05;
            sunMoon(smX, smY, smType % 2);
            if (smX > 65)
                smY -= 0.005;
            else
                smY += 0.005;

            if (smX < -10)
            {
                smX = width;
                smY = height * 0.2;
                smType += 1;
            }



            //jump and player render
            //borders
            int deltaX = 0;
            // left
            if (playerX < 65)
                deltaX = playerX - 65;
            // right
            else if (playerX > width * amountOfChunks - 65)
                deltaX = playerX - (amountOfChunks - 0) * width + 65;
            else
                deltaX = 0;


            if (jumpTime < 0)
                screen[(int)((chunkHeight[playerX] + deltaY - 1) * width + 65 + deltaX)] = '@';
            else
            {
                if (jumpTime == 10 || jumpTime == 9)
                    screen[(int)((chunkHeight[playerX] + deltaY - 1 - 1) * width + 65 + deltaX)] = '@';
                if (jumpTime == 8 || jumpTime == 7)
                    screen[(int)((chunkHeight[playerX] + deltaY - 1 - 2) * width + 65 + deltaX)] = '@';
                if (jumpTime == 6 || jumpTime == 5 || jumpTime == 4)
                    screen[(int)((chunkHeight[playerX] + deltaY - 1 - 3) * width + 65 + deltaX)] = '@';
                if (jumpTime == 2 || jumpTime == 3)
                    screen[(int)((chunkHeight[playerX] + deltaY - 1 - 2) * width + 65 + deltaX)] = '@';
                if (jumpTime == 0 || jumpTime == 1)
                    screen[(int)((chunkHeight[playerX] + deltaY - 1 - 1) * width + 65 + deltaX)] = '@';
            }

            for (int i = 0; i < interactionsX.size(); i += 1)
            {
                if (playerX == interactionsX[i])
                {
                    if (interactionsType[i] == 'f')
                    {
                        str = "Press enter to hop on the rocket";
                        text(width / 2 - str.length() / 2, height - 2, str);
                    }
                }
            }

            /*
            if (playerX == amountOfChunks * width - 65)
                playerX = 65;
            else if (playerX == 64)
                playerX = amountOfChunks * width - 65;
            */

            render();
            Sleep(40);
            emptyBuffer();
        }
    }
    else
    {
        map(planet);
    }
}

void mainMenu()
{
    string str, button;
    int menu = 0, firstTime = 1;
    emptyBuffer();
    while (true)
    {
        logo(width / 2 - 40, height / 4 - 4);

        if (!firstTime)
            button = getKey();
        firstTime = 0;

        if ((button == "w" || button == "upArrow") && menu > 0)
            menu -= 1;
        if ((button == "s" || button == "downArrow") && menu < 2)
            menu += 1;
        if (button == "enter" && menu == 0)
        {
            emptyBuffer();
            map(3);
        }
        if (button == "enter" && menu == 1)
        {
            exit(0);
        }
        if (button == "enter" && menu == 2)
        {
            exit(0);
        }

        if (menu == 0)
            str = "> Play <";
        else
            str = "Play";
        text(width / 2 - str.size() / 2, height / 2, str);

        if (menu == 1)
            str = "> Options <";
        else
            str = "Options";
        text(width / 2 - str.size() / 2, height / 2 + 2, str);

        if (menu == 2)
            str = "> Exit <";
        else
            str = "Exit";
        text(width / 2 - str.size() / 2, height / 2 + 4, str);

        render();
    }
}

int main()
{
    srand(time(NULL));

    //Сначала задаем минимальные значения
    SMALL_RECT zeroWindow = { 0, 0, 20, 20 };
    COORD zBuffer = { 20, 20 };
    SetConsoleWindowInfo(hConsole, TRUE, &zeroWindow);
    SetConsoleScreenBufferSize(hConsole, zBuffer);

    //А уже потом изменяем значения на нужные нам
    COORD bufferSize = { width, height };
    SMALL_RECT windowSize = { 0, 0, width - 1, height - 1 };
    SetConsoleScreenBufferSize(hConsole, bufferSize);
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

    SetConsoleActiveScreenBuffer(hConsole); // Настройка консоли
    emptyBuffer();

    walk(3);

    mainMenu();

    mainMenu();
}


/*
          ___---___
       _--         --_
    _--   .     ______--_
   / ___ o .   /         \
  / |   \__.   \_--\      \
 |  |      \    _  _|      |
 |   \     |   / --        |
 |   |     |  |   ____     |
|    |  __/    \_-    -     |
|     \/ __      ____ /     |
|      _-  \    /    \\  ___|
|     /    |    \_    \\/   |
|     |    /      \    |    |
 |    |   /        \  /    |
 |     \  |        |  |    |
 |      |/          \/     |
  \                       /
   \_       _           _/
     --   -- ----__   --
       ---___   ___---
             ---
*/