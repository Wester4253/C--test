#include <cstdint>
#include <raylib.h>
#include <vector>
#include <algorithm>
#include <utility>

// Variables
constexpr int width = 500;
constexpr int height = 500;
constexpr int cell_size = 10;
constexpr int cols = width / cell_size;
constexpr int rows = height / cell_size;

const char *title = "GameOfLife";

// CODE
// grid stuff?
using Grid = std::vector<std::uint8_t>;
int index(int x, int y) {
    return y * cols + x;
}

int countNeighbors(const Grid &cells, int x, int y) {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) {
                continue;
            }
            int nx = (x + dx + cols) % cols;
            int ny = (y + dy + rows) % rows;
            count += cells[index(nx, ny)];
        }
    }
    return count;
}

void step(const Grid &cells, Grid &next) {
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            int n = countNeighbors(cells, x, y);
            bool alive = cells[index(x, y)] != 0;
            next[index(x, y)] = alive ? (n == 2 || n == 3) : (n == 3);
        }
    }
}

void drawBoard(const Grid &grid) {
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x){
            if (grid[index(x, y)] == 1) {
                DrawRectangle(x * cell_size, y * cell_size, cell_size, cell_size, RAYWHITE);
            }
        }
    }
    for (int x = 0; x <= cols; ++x) {
        DrawLine(x * cell_size, 0, x * cell_size, height, Fade(DARKGRAY, 0.4f));
    }
    for (int y = 0; y <= rows; ++y) {
        DrawLine(0, y * cell_size, width, y * cell_size, Fade(DARKGRAY, 0.4f));
    }
}



int main() {
    InitWindow(width, height, title);
    SetTargetFPS(60);
    Grid grid(cols * rows, 0);
    Grid nextGen(cols * rows, 0);
    bool running = false;
    float timer = 0.0f;
    constexpr float step_delay = 0.1f;

    while (!WindowShouldClose()) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 mouse = GetMousePosition();
            int x = static_cast<int>(mouse.x) / cell_size;
            int y = static_cast<int>(mouse.y) / cell_size;
            if (x >= 0 && x < cols && y >= 0 && y < rows) {
                grid[index(x, y)] = IsMouseButtonDown(MOUSE_BUTTON_LEFT) ? 1 : 0;
            }
        }
        
        if (IsKeyPressed(KEY_SPACE)) {
            running = !running;
        }
        if (IsKeyPressed(KEY_C)) {
            std::fill(grid.begin(), grid.end(), 0);
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            step(grid, nextGen);
            std::swap(grid, nextGen);
        }
        
        if (running) {
            timer += GetFrameTime();
            if (timer >= step_delay) {
                timer = 0.0f;
                step(grid, nextGen);
                std::swap(grid, nextGen);
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);
        drawBoard(grid);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}