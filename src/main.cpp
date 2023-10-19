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
//    spline = SplineChain<SplineMethod::HERMITE, 2, float>({0,0}, {0,0});
//    spline.add_point({40.0f,20.0f}, {10.0f, 5.0f}, 3.0f);
//    spline.build();

    spline = SplineChain<SplineMethod::HERMITE, 2, float>({4.025 * 12.0, 3 * 12.0}, {-4.1 * 12.0, 0.0999994 * 12.0});
    spline.add_point({-0.0124998 * 12.0, 2.8 * 12.0},
                     {-3.95 * 12.0, 0.0500002 * 12.0},
                     1.00022);
    spline.add_point({-4.00399 * 12.0, 2.99458 * 12.0},
                     {-3.93404 * 12.0, -0.0283194 * 12.0},
                     0.89988);
    spline.add_point({-4.99096 * 12.0, 0 * 12.0},
                     {0 * 12.0, -4.14979 * 12.0},
                     0.902563);
    spline.add_point({-3.97034 * 12.0, -2.98336 * 12.0},
                     {4.08136 * 12.0, -0.116559 * 12.0},
                     1);
    spline.add_point({4.76837e-07 * 12.0, -2.8375 * 12.0},
                     {4.1 * 12.0, 0.0499992 * 12.0},
                     1);
    spline.add_point({4 * 12.0, -2.9875 * 12.0},
                     {3.8 * 12.05, -0.100002 * 12.0},
                     1);
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

    if (spline.splines.empty()) {
        throw std::runtime_error("Spline must have at least 2 points\n");
    }

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow((int) WINDOW.x, (int) WINDOW.y, "Hermite Spline Test");
    SetTargetFPS(60);

    font = LoadFont("../res/jetbrains_mono.ttf"); // literally the best one on god

    Field<2> field2d;
    Field<3> field3d;
    TestRobot robot = TestRobot({ROBOT_SIZE, ROBOT_SIZE, ROBOT_HEIGHT}, MAX_VELOCITY, MAX_ACCELERATION);
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
