#include <conio.h>
#include <sys/ioctl.h>
#include <termios.h>

#include <ctime>
#include <iostream>
using namespace std;

void getTermSize(int &height, int &width);

enum Direction { Top = 1, Down, Left, Right };

// properties of map
struct Map {
    int height, width;
};

// properties of snake
struct Snake {
    int head_X, head_Y, tailLength, tail_X[20], tail_Y[20];
    char headShap;
    Direction direction;
};

// properties of fruit
struct Fruit {
    int fruit_X, fruit_Y;
    char gender;
};

// properties of player
struct Player {
    int score;
    bool lose;
};

Map map;
Snake snake;
Player player;
Fruit fruit;

void genratFruit();

// function to shift the tail
void shiftTail(int arr[], int size) {
    for (int i = size - 2; i >= 0; i--) arr[i + 1] = arr[i];
}

// function to determine the dirction
void getDirecion() {
    if (_kbhit()) {
        char input;
        cin >> input;
        switch (input) {
            case 'k':
                snake.direction = Top;
                snake.headShap = '^';
                break;
            case 'j':
                snake.direction = Down;
                snake.headShap = 'v';
                break;
            case 'h':
                snake.direction = Left;
                snake.headShap = '<';
                break;
            case 'l':
                snake.direction = Right;
                snake.headShap = '>';
                break;
            case 'i':
                exit(-1);
            case 'o':
                getTermSize(map.height, map.width);
        }
    }
}

void move() {
    // shift the tail
    shiftTail(snake.tail_X, snake.tailLength);
    shiftTail(snake.tail_Y, snake.tailLength);
    snake.tail_X[0] = snake.head_X;
    snake.tail_Y[0] = snake.head_Y;

    switch (snake.direction) {
        case Top:
            snake.head_X--;
            break;
        case Down:
            snake.head_X++;
            break;
        case Right:
            snake.head_Y++;
            break;
        case Left:
            snake.head_Y--;
            break;
    }
    // condition check if the head hit the fruit
    if (fruit.fruit_X == snake.head_X && fruit.fruit_Y == snake.head_Y) {
        genratFruit();
        player.score++;
        cout << "\a";
        snake.tailLength++;
    }
    // condition check if the head hit the map
    if (snake.head_X == 0) snake.head_X = map.height - 1;
    if (snake.head_X == map.height) snake.head_X = 1;
    if (snake.head_Y == 0) snake.head_Y = map.width - 1;
    if (snake.head_Y == map.width) snake.head_Y = 1;

    // condition check if the head hit the tail
    // for (int i = 0; i < snake.tailLength; i++) {
    //   if (snake.head_X == snake.tail_X[i] && snake.head_Y == snake.tail_Y[i])
    //        exit(-1);
    //}
}

// function to gernat fruit in random postion
void genratFruit() {
    srand(time(0));
    fruit.fruit_X = ((rand() % map.height - 1) + 1);
    fruit.fruit_Y = ((rand() % map.width - 1) + 1);
    fruit.gender = '*';
    if (player.score % 5 == 0) fruit.gender = '@';
}

// function to setup th game
void setup() {
    // setup height and widht
    getTermSize(map.height, map.width);
    genratFruit();
    snake.head_X = map.height / 2;
    snake.head_Y = map.width / 2;
    snake.tailLength = 0;
    snake.headShap = '^';
    player.score = 0;
    player.lose = false;
}

void draw() {
    system("clear -x");
    for (int i = 0; i < map.height; i++) {
        cout << "\t";
        for (int y = 0; y < map.width; y++) {
            if (i == 0)
                cout << "_";
            else if (i == map.height - 1)
                cout << "-";
            else if (y == 0 || y == map.width - 1)
                cout << "|";
            else if (i == snake.head_X && y == snake.head_Y)
                cout << snake.headShap;
            else if (i == fruit.fruit_X && y == fruit.fruit_Y)
                cout << fruit.gender;
            else {
                bool printBlank = false;
                for (int z = 0; z < snake.tailLength; z++) {
                    if (snake.tail_X[z] == i && snake.tail_Y[z] == y) {
                        cout << "O";
                        printBlank = true;
                    }
                }
                if (!printBlank) cout << " ";
            }
        }
        cout << endl;
    }
    cout << "\tScore	:" << player.score << endl;
}
void howToSleep() { system("sleep .1"); }

int main() {
    setup();
    while (!player.lose) {
        draw();
        getDirecion();
        move();
        howToSleep();
    }

    return 0;
}

void getTermSize(int &height, int &width) {
    struct winsize termsize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &termsize);
    map.height = termsize.ws_row - 10;
    map.width = termsize.ws_col - 10;
}

