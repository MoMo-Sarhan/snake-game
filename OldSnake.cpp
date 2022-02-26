#include <conio.h>

#include <cstdio>
#include <ctime>
#include <iostream>
#include <random>
using namespace std;
enum Direction { Top = 1, Down, Right, Left };

struct Map {
    int heigth, width, fruit_X, fruit_Y;
};
struct Snake {
    int head_X, head_Y, tailLength, tail_X[20], tail_Y[20];
    char dir;
};
struct Player {
    int score;
    bool lose;
};
Map map1;
Player player1;
Snake snake1;
void shiftTail(int tail[], int size) {
    for (int i = size - 2; i >= 0; i--) tail[i + 1] = tail[i];
}
void generateFruit() {
    srand(time(0));
    map1.fruit_X = ((rand() % map1.heigth - 3) + 2);
    map1.fruit_Y = ((rand() % map1.width - 3) + 2);
}
void setup() {
    //------properties of map
    map1.heigth = 20;
    map1.width = 40;
    generateFruit();
    //-----properties of snake
    snake1.head_X = map1.heigth / 2;
    snake1.head_Y = map1.width / 2;
    //-------properties of player
    player1.score = 0;
    player1.lose = false;
}
bool stop = false;
void input() {
    while (_kbhit()) {
        char c = getchar();
        switch (c) {
            case 'k':
                snake1.dir = Top;
                break;
            case 'h':
                snake1.dir = Left;
                break;
            case 'j':
                snake1.dir = Down;
                break;
            case 'l':
                snake1.dir = Right;
                break;
            case 'i':
                exit(0);
            case 'o':
                stop = true;
        }
    }
}

void move() {
    shiftTail(snake1.tail_X, 20);
    shiftTail(snake1.tail_Y, 20);
    snake1.tail_X[0] = snake1.head_X;
    snake1.tail_Y[0] = snake1.head_Y;

    switch (snake1.dir) {
        case Top:
            snake1.head_X--;
            break;
        case Down:
            snake1.head_X++;
            break;
        case Right:
            snake1.head_Y++;
            break;
        case Left:
            snake1.head_Y--;
            break;
    }
    if (snake1.head_X >= map1.heigth || snake1.head_X <= 0 ||
            snake1.head_Y >= map1.width || snake1.head_Y <= 0)
        exit(0);
    if (snake1.head_X == map1.fruit_X && snake1.head_Y == map1.fruit_Y) {
        generateFruit();
        snake1.tailLength++;
        player1.score++;
        snake1.tailLength++;
    }
}

void drew() {
    for (int i = 0; i < map1.heigth; i++) {
        for (int y = 0; y < map1.width; y++) {
            if (i == 0 || i == map1.heigth - 1)
                cout << "=";
            else if (y == 0 || y == map1.width - 1)
                cout << "|";
            else if (i == snake1.head_X && y == snake1.head_Y)
                cout << "#";
            else if (i == map1.fruit_X && y == map1.fruit_Y)
                cout << "$";
            else {
                {
                    bool printBlank = false;
                    for (int z = 0; z < snake1.tailLength; z++) {
                        if (snake1.tail_X[z] == i && snake1.tail_Y[z] == y) {
                            cout << "o";
                            printBlank = true;
                            break;
                        }
                    }
                    if (!printBlank) cout << " ";
                }
            }
        }
        cout << endl;
    }
    cout << "Score          :" << player1.score << endl;
}

int main() {
    setup();

    while (!player1.lose) {
        system("clear");
        drew();
        input();
        move();
        if (player1.score < 5)
            system("sleep .2");
        else
            system("sleep .1");
        if (stop) {
            system("read x");
            stop = false;
        }
    }

    system("read x");
    return 0;
}
