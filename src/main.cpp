#include <cstdio>
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <format>
#include <filesystem>


#include "Hermite.hpp"
#include "Constants.hpp"
#include "Static.hpp"
#include "Vector.hpp"
#include "Field.hpp"


enum class Mode {
    TWO_DIMENSIONAL,
    THREE_DIMENSIONAL
};

Mode mode = Mode::TWO_DIMENSIONAL;

void setup_spline() {
    spline.total_duration = 5;
    spline.add_point(Node{Vec2<float>{4.15, 2.9625}, Vec2<float>{-4.16667, 7.94729e-07}});
    spline.add_point(Node{Vec2<float>{4.76837e-07, 3.025}, Vec2<float>{-4.125, 0.333333}});
    spline.add_point(Node{Vec2<float>{-4.00399, 2.99458}, Vec2<float>{-3.73855, -1.7945}});
    spline.add_point(Node{Vec2<float>{-4.99096, 0}, Vec2<float>{0, -4.14979}});
    spline.add_point(Node{Vec2<float>{-3.97034, -2.98336}, Vec2<float>{4.11241, -0.560783}});
    spline.add_point(Node{Vec2<float>{0.179451, -3.01701}, Vec2<float>{4.14979, 0.186927}});
    spline.add_point(Node{Vec2<float>{3.84697, -2.80391}, Vec2<float>{2.75, 0.625}});
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

    // float camera_speed = BASE_CAMERA_MOVEMENT_SPEED * GetFrameTime() / camera.zoom;
    // float zoom_speed = BASE_CAMERA_ZOOM_SPEED * GetFrameTime();
    float duration_edit_speed = BASE_DURATION_EDIT_MULTIPLIER * GetFrameTime();

    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        // camera_speed = SPRINT_CAMERA_MOVEMENT_SPEED * GetFrameTime() / camera.zoom;
        // zoom_speed = SPRINT_CAMERA_ZOOM_SPEED * GetFrameTime();
        duration_edit_speed = SPRINT_DURATION_EDIT_MULTIPLIER * GetFrameTime();
    }

    // if (IsKeyDown(KEY_E)) {
    //     camera.zoom += zoom_speed;
    // }

    // if (IsKeyDown(KEY_Q)) {
    //     camera.zoom -= zoom_speed;
    //     camera.zoom = std::max(0.01f, camera.zoom);
    // }

    // if (IsKeyDown(KEY_W)) {
    //     camera.target.y -= camera_speed;
    // }

    // if (IsKeyDown(KEY_A)) {
    //     camera.target.x -= camera_speed;
    // }

    // if (IsKeyDown(KEY_S)) {
    //     camera.target.y += camera_speed;
    // }

    // if (IsKeyDown(KEY_D)) {
    //     camera.target.x += camera_speed;
    // }

    if (IsKeyDown(KEY_X)) {
        spline.total_duration += duration_edit_speed;
        reset();
    }

    if (IsKeyDown(KEY_Z)) {
        spline.total_duration -= duration_edit_speed;
        if (spline.total_duration < MINIMUM_SPLINE_DURATION) {
            spline.total_duration = MINIMUM_SPLINE_DURATION;
        }
        reset();
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

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow((int) WINDOW.x, (int) WINDOW.y, "Hermite Spline Test");
    SetTargetFPS(200);

    if (spline.get_point_count() <= 1) {
        printf("Spling must have at least 2 points\n");
        return 0;
    }  

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
