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

Hermite2f spline;

void setup_spline() {
    spline = SplineChain<SplineMethod::HERMITE, 2, float>({4.025, 3}, {-4.1, 0.0999994});
    spline.add_point({-0.0124998, 2.8}, {-3.95, 0.0500002}, 1.00022);
    spline.add_point({-4.00399, 2.99458}, {-3.93404, -0.0283194}, 0.89988);
    spline.add_point({-4.99096, 0}, {0, -4.14979}, 0.902563);
    spline.add_point({-3.97034, -2.98336}, {4.08136, -0.116559}, 1);
    spline.add_point({4.76837e-07, -2.8375}, {4.1, 0.0499992}, 1);
    spline.add_point({4, -2.9875}, {3.85, -0.100002}, 1);
    spline.build();

    // Cardinal2f spline = Cardinal2f({4.15, 2.9625});
    // spline.add_point({4.76837e-07, 3.025}, 1.00022, 0.5f);
    // spline.add_point({-4.00399, 2.99458}, 0.89988, 0.5f);
    // spline.add_point({-4.99096, 0}, 0.801776, 0.5f);
    // spline.add_point({-3.97034, -2.98336}, 1, 0.5f);
    // spline.add_point({0.179451, -3.01701}, 1, 0.5f);
    // spline.add_point({3.84697, -2.80391}, 1, 0.5f);
    // spline.build();
}

void input() {
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
    
    if (IsKeyPressed(KEY_P)) {
       spline.print_parameters();
    }
}


int main() {
    setup_spline();

    if (spline.splines.size() <= 1) {
        throw std::runtime_error("Spline must have at least 2 points\n");
        return 0;
    }

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow((int) WINDOW.x, (int) WINDOW.y, "Hermite Spline Test");
    SetTargetFPS(60);

    font = LoadFont("../res/jetbrains_mono.ttf"); // literally the best one on god

    Field<2> field2d;
    Field<3> field3d;
    TestRobot robot = TestRobot({ROBOT_SIZE, ROBOT_SIZE, ROBOT_HEIGHT}, 5, 20);
    while (!WindowShouldClose()) {
        if (current_time == 0 or current_time > spline.total_duration) {
            current_time = 0;
            robot.position = spline.get_point_at(0);
            robot.velocity = spline.get_tangent_at(0);
        }

        field2d.update(current_time, paused, spline, robot);
        field3d.update(current_time, paused, spline, robot);

        input();
        switch (mode) {
            case Mode::TWO_DIMENSIONAL:
                field2d.render(current_time, spline, robot);
                break;
            case Mode::THREE_DIMENSIONAL:
                field3d.render(current_time, spline, robot);
                break;
        }        

        if (!paused) {
            current_time += GetFrameTime();
        }
    }

    CloseWindow();
    UnloadFont(font);
    return 0;
}
