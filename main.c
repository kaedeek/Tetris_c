#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define WIDTH 10
#define HEIGHT 20

int field[HEIGHT][WIDTH] = {0};
int block[4][4] = {0};
int blockX = WIDTH / 2 - 2, blockY = 0;
int score = 0;
int level = 1;
int speed = 500;

int blocks[7][4][4] = {
    {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // I字型
    {{1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // L字型
    {{1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // 逆L字型
    {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // 四角形
    {{1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // T字型
    {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // Z字型
    {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}  // 逆Z字型
};

void drawField() {
    system("cls");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (field[y][x] == 0)
                printf(" .");
            else
                printf(" #");
        }
        printf("\n");
    }
    printf("Score: %d\n", score);
    printf("Level: %d\n", level);
}

void drawBlock() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (block[y][x] == 1) {
                COORD coord = {blockX + x, blockY + y};
                SetConsoleCursorPosition(hConsole, coord);
                printf("#");
            }
        }
    }
}

void clearBlock() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (block[y][x] == 1) {
                COORD coord = {blockX + x, blockY + y};
                SetConsoleCursorPosition(hConsole, coord);
                printf(" ");
            }
        }
    }
}

int canMove(int dx, int dy) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (block[y][x] == 1) {
                int newX = blockX + x + dx;
                int newY = blockY + y + dy;
                if (newX < 0 || newX >= WIDTH || newY < 0 || newY >= HEIGHT || field[newY][newX] == 1) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

void dropBlock() {
    if (canMove(0, 1)) {
        clearBlock();
        blockY++;
        drawBlock();
    } else {
        // ブロックをフィールドに固定
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                if (block[y][x] == 1) {
                    field[blockY + y][blockX + x] = 1;
                }
            }
        }
        // ラインの消去
        for (int y = 0; y < HEIGHT; y++) {
            int fullLine = 1;
            for (int x = 0; x < WIDTH; x++) {
                if (field[y][x] == 0) {
                    fullLine = 0;
                    break;
                }
            }
            if (fullLine) {
                score += 100;
                for (int yy = y; yy > 0; yy--) {
                    for (int xx = 0; xx < WIDTH; xx++) {
                        field[yy][xx] = field[yy - 1][xx];
                    }
                }
                for (int xx = 0; xx < WIDTH; xx++) {
                    field[0][xx] = 0;
                }
            }
        }
        // レベルアップ
        if (score >= level * 500) {
            level++;
            speed -= 50;
        }
        // 新しいブロックを生成
        blockY = 0;
        blockX = WIDTH / 2 - 2;
        int newBlock = rand() % 7;
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                block[y][x] = blocks[newBlock][y][x];
            }
        }
        // ゲームオーバー判定
        if (!canMove(0, 0)) {
            printf("Game Over! Final Score: %d\n", score);
            exit(0);
        }
    }
}

void rotateBlock(int block[4][4]) {
    int temp[4][4] = {0};
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            temp[x][3 - y] = block[y][x];
        }
    }
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            block[y][x] = temp[y][x];
        }
    }
}

void handleInput() {
    if (_kbhit()) {
        char key = _getch();
        if (key == 'a' && canMove(-1, 0)) {
            clearBlock();
            blockX--;
            drawBlock();
        } else if (key == 'd' && canMove(1, 0)) {
            clearBlock();
            blockX++;
            drawBlock();
        } else if (key == 's' && canMove(0, 1)) {
            clearBlock();
            blockY++;
            drawBlock();
        } else if (key == 'w') {
            clearBlock();
            rotateBlock(block);
            drawBlock();
        }
    }
}

int main() {
    srand(time(NULL));
    int newBlock = rand() % 7;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            block[y][x] = blocks[newBlock][y][x];
        }
    }

    while (1) {
        drawField();
        drawBlock();
        handleInput();
        Sleep(speed / 10); // 入力処理の頻度を増やす
        dropBlock();
    }

    return 0;
}
