#include <iostream>
#include <cstdlib>
#include <ncurses.h>
#include <deque>

using namespace std;

const int WIDTH = 40;
const int HEIGHT = 20;

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Point {
    int x;
    int y;
    Point(int x, int y) : x(x), y(y) {}
};

deque<Point> snake;

void init() {
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(100);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK); // for snake
    init_pair(2, COLOR_RED, COLOR_BLACK); // for food
    init_pair(3, COLOR_CYAN, COLOR_BLACK); // for border
}

void drawBorder() {
    attron(COLOR_PAIR(3));
    for (int i = 0; i < WIDTH; i++) {
        mvprintw(0, i, "-");
        mvprintw(HEIGHT-1, i, "-");
    }
    for (int i = 0; i < HEIGHT; i++) {
        mvprintw(i, 0, "|");
        mvprintw(i, WIDTH-1, "|");
    }
    attroff(COLOR_PAIR(3));
}

void drawSnake() {
    attron(COLOR_PAIR(1));
    for (auto& p : snake) {
        mvprintw(p.y, p.x, "o");
    }
    attroff(COLOR_PAIR(1));
}

void drawFruit(Point& food) {
    attron(COLOR_PAIR(2));
    mvprintw(food.y, food.x, "@");
    attroff(COLOR_PAIR(2));
}

void generateFood(Point& food) {
    food.x = rand() % (WIDTH-2) + 1;
    food.y = rand() % (HEIGHT-2) + 1;
}

bool moveSnake(Direction dir, Point& food, int& score) {
    Point head = snake.front();
    switch (dir) {
        case UP:
            head.y--;
            break;
        case DOWN:
            head.y++;
            break;
        case LEFT:
            head.x--;
            break;
        case RIGHT:
            head.x++;
            break;
    }
    if (head.x == 0 || head.x == WIDTH-1 || head.y == 0 || head.y == HEIGHT-1) {
        return false; // Game over: hit wall
    }
    if (head.x == food.x && head.y == food.y) {
        generateFood(food);
        score++;
    } else {
        snake.pop_back();
    }
    for (auto& p : snake) {
        if (p.x == head.x && p.y == head.y) {
            return false; // Game over: hit self
        }
    }
    snake.push_front(head);
    return true;
}

int main() {
    init();
    int ch;
    Direction dir = UP;
    int score = 0; // Initialize score to 0
    Point food(0, 0);
    bool startGame = false;
    while (!startGame) {
        clear();
        mvprintw(5, 10, "Welcome to Snake!");
        mvprintw(7, 2, "Use arrow keys to move the snake.");
        mvprintw(8, 2, "Eat the food (*) to increase your score.");
        mvprintw(9, 2, "Don't hit the wall or the snake's body!");
        mvprintw(11, 10, "Press SPACE to start");
        refresh();
        ch = getch();
        if (ch == ' ') {
            startGame = true;
        }
    }
    drawBorder();
    generateFood(food);
    snake.push_front(Point(WIDTH/2, HEIGHT/2));
    snake.push_front(Point(WIDTH/2, HEIGHT/2-1));
    snake.push_front(Point(WIDTH/2, HEIGHT/2-2));
    drawSnake();
    while ((ch = getch()) != 'q') {
        switch (ch) {
            case KEY_UP:
                dir = UP;
                break;
            case KEY_DOWN:
                dir = DOWN;
                break;
            case KEY_LEFT:
                dir = LEFT;
                break;
            case KEY_RIGHT:
                dir = RIGHT;
                break;
        }
        if (!moveSnake(dir, food, score)) {
            break;
        }
        clear();
        drawBorder();
        drawSnake();
	drawFruit(food);
        mvprintw(HEIGHT, 0, "Score: %d", score); // Display score at the bottom of the map
        refresh();
    }
    endwin();
    cout << "Game over! Score: " << score << endl;
    return 0;
}
