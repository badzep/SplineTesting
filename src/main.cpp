#include <cstdio>
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <format>
#include <filesystem>
#include <stdexcept>


#include "Constants.hpp"
#include "Static.hpp"
#include "Vector.hpp"
#include "Field.hpp"
#include "Splines.hpp"


enum class Mode {
    TWO_DIMENSIONAL,
    THREE_DIMENSIONAL
};

Mode mode = Mode::TWO_DIMENSIONAL;

void setup_spline() {
    builder = SplineBuilder({4.15, 2.9625}, {-4.16667, 7.94729e-07});
    builder.add_point({4.76837e-07, 3.025}, {-3.95, 0.0500002}, 1.00022);
    builder.add_point({-4.00399, 2.99458}, {-4.43404, -0.678319}, 0.89988);
    builder.add_point({-4.99096, 0}, {0, -4.14979}, 0.801776);
    builder.add_point({-3.97034, -2.98336}, {4.11241, -0.560783}, 1);
    builder.add_point({0.179451, -3.01701}, {4.14979, 0.186927}, 1);
    builder.add_point({3.84697, -2.80391}, {2.75, 0.625}, 1);
    builder.build(spline);
}

void input(Field2d &field2d, Field3d &field3d) {
    if (IsKeyPressed(KEY_TAB)) {
        if (mode == Mode::TWO_DIMENSIONAL) {
            mode = Mode::THREE_DIMENSIONAL;
            HideCursor();
            DisableCursor();
        } else {
            mode = Mode::TWO_DIMENSIONAL;
            ShowCursor();
            EnableCursor();
        }

        printf("mode %i\n", mode);
    }

    if (IsKeyPressed(KEY_SPACE)) {
        paused = !paused;
    }

    if (IsKeyPressed(KEY_ONE)) {
        SetTargetFPS(10);
    }

    if (IsKeyPressed(KEY_TWO)) {
        SetTargetFPS(60);
    }

    if (IsKeyPressed(KEY_THREE)) {
        SetTargetFPS(200);
    }

    if (IsKeyPressed(KEY_FOUR)) {
        SetTargetFPS(10000);
    }

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        if (mode == Mode::TWO_DIMENSIONAL) {
            field2d.drag_points();
        }
    }
    
    if (IsKeyPressed(KEY_P)) {
        builder.print_parameters();
    }
}

void update(Field2d &field2d, Field3d &field3d) {
    input(field2d, field3d);
    if (mode == Mode::TWO_DIMENSIONAL) {
        field2d.render();
    } else {
        field3d.render();
    }
}

int main() {
    setup_spline();

    if (spline.splines.size() <= 1) {
        throw std::runtime_error("Spilne must have at least 2 points\n");
        return 0;
    }

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow((int) WINDOW.x, (int) WINDOW.y, "Hermite Spline Test");
    SetTargetFPS(200);

    font = LoadFont("../res/jetbrains_mono.ttf"); // literally the best one on god

    Field2d field2d;
    Field3d field3d;

    while (!WindowShouldClose()) {
        update(field2d, field3d);
    }

    CloseWindow();
    UnloadFont(font);
    return 0;
}
