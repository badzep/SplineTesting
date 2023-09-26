#include <cstdio>
#include <raylib.h>
#include <stdexcept>


#include "Constants.hpp"
#include "Static.hpp"
#include "Field.hpp"
#include "Spline.hpp"


enum class Mode {
    TWO_DIMENSIONAL,
    THREE_DIMENSIONAL
};

Mode mode = Mode::TWO_DIMENSIONAL;

void setup_spline() {
    spline = SplineChain<SplineMethod::HERMITE, 2, float>({4.025, 3}, {-4.1, 0.0999994});
    spline.add_point({-0.0124998, 2.8}, {-3.95, 0.0500002}, 1.00022);
    spline.add_point({-4.00399, 2.99458}, {-3.93404, -0.0283194}, 0.89988);
    spline.add_point({-4.99096, 0}, {0, -4.14979}, 0.902563);
    spline.add_point({-3.97034, -2.98336}, {4.08136, -0.116559}, 1);
    spline.add_point({4.76837e-07, -2.8375}, {4.1, 0.0499992}, 1);
    spline.add_point({4, -2.9875}, {3.85, -0.100002}, 1);
    spline.build();

    test_position = spline.points[0];
    test_velocity = spline.tangents[0];

//    spline = Cardinal2f({4.15, 2.9625});
//    spline.add_point({4.76837e-07, 3.025}, 1.00022, 0.5f);
//    spline.add_point({-4.00399, 2.99458}, 0.89988, 0.5f);
//    spline.add_point({-4.99096, 0}, 0.801776, 0.5f);
//    spline.add_point({-3.97034, -2.98336}, 1, 0.5f);
//    spline.add_point({0.179451, -3.01701}, 1, 0.5f);
//    spline.add_point({3.84697, -2.80391}, 1, 0.5f);
//    spline.build();
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
       spline.print_parameters();
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
