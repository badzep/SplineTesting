#include <fstream>
#include <iostream>
#include <raylib.h>
#include <format>
#include <filesystem>


#include "Hermite.hpp"
#include "Constants.hpp"
#include "Vector.hpp"


HermiteSpline spline;
Camera2D camera = {};
bool paused = false;
float simulation_time = 0;
float current_position = 0;
Vec<float> test_position = {0,0};

void setup_spline() {
    spline.total_duration = 5;
    spline.add_point(HermitePoint{Vec<float>{4.15, 2.9625}, Vec<float>{-4.16667, 7.94729e-07}});
    spline.add_point(HermitePoint{Vec<float>{4.76837e-07, 3.025}, Vec<float>{-4.125, 0.333333}});
    spline.add_point(HermitePoint{Vec<float>{-4.00399, 2.99458}, Vec<float>{-3.73855, -1.7945}});
    spline.add_point(HermitePoint{Vec<float>{-4.99096, 0}, Vec<float>{0, -4.14979}});
    spline.add_point(HermitePoint{Vec<float>{-3.97034, -2.98336}, Vec<float>{4.11241, -0.560783}});
    spline.add_point(HermitePoint{Vec<float>{0.179451, -3.01701}, Vec<float>{4.14979, 0.186927}});
    spline.add_point(HermitePoint{Vec<float>{3.84697, -2.80391}, Vec<float>{2.75, 0.625}});
}

void reset() {
    simulation_time = 0;
    current_position = 0;
    test_position = spline.get_position_at(0);
}

void draw_grid() {
    for (int i = -GRID_SIZE; i <= GRID_SIZE; i++) {
        if (i % 2 == 0) {
            DrawLineEx({-(float) GRID_SIZE, (float) i}, {(float)GRID_SIZE, (float) i}, .025f, {255,255,255,200});
            DrawLineEx({(float) i, -(float) GRID_SIZE}, {(float) i, (float) GRID_SIZE}, .025f, {255,255,255,200});
        } else {  
            DrawLineV({-(float) GRID_SIZE, (float) i}, {(float)GRID_SIZE, (float) i}, {255,255,255,100});
            DrawLineV({(float) i, -(float) GRID_SIZE}, {(float) i, (float) GRID_SIZE}, {255,255,255,100});
        }
    }

    Vec<float> start = {-GRID_SIZE, -GRID_SIZE};
    Vec<float> x_up = start + Vec<float>{1,0};
    Vec<float> y_up = start + Vec<float>{0,1};
    DrawLineField(start, x_up, 0.025f, RED);
    DrawLineField(start, y_up, 0.025f, GREEN);
}

void draw_field() {
    // loading zones
    DrawLineField({-6,-4}, {-4,-6}, TWO_INCHES, BLUE);
    DrawLineField({6,-4}, {4,-6}, TWO_INCHES, BLUE);

    DrawLineField({-6,4}, {-4,6}, TWO_INCHES, RED);
    DrawLineField({6,4}, {4,6}, TWO_INCHES, RED);

    // center black I thing
    DrawLineField({4,-0}, {-4,0}, TWO_INCHES, BLACK);
    DrawLineField({4,2}, {4,-2}, TWO_INCHES, BLACK);
    DrawLineField({-4,2}, {-4,-2}, TWO_INCHES, BLACK);

    // goal net things
    DrawCircleField({2,4}, 5.0f * ONE_INCH, BLUE);
    DrawCircleField({-2,4}, 5.0f * ONE_INCH, BLUE);
    DrawLineField({2,4}, {-2,4}, ONE_INCH, BLUE);
    DrawLineField({2,4}, {2,6}, ONE_INCH, BLUE);
    DrawLineField({-2,4}, {-2,6}, ONE_INCH, BLUE);

    DrawCircleField({2,-4}, 5.0f * ONE_INCH, RED);
    DrawCircleField({-2,-4}, 5.0f * ONE_INCH, RED);
    DrawLineField({2,-4}, {-2,-4}, ONE_INCH, RED);
    DrawLineField({2,-4}, {2,-6}, ONE_INCH, RED);
    DrawLineField({-2,-4}, {-2,-6}, ONE_INCH, RED);
}

void drag_points(HermiteSpline &spline, Camera2D &camera) {
    Vec<float> clicked_position = Vec<float>(GetScreenToWorld2D(GetMousePosition(), camera)).screen_to_field();
    for (HermitePoint &spline_point: spline.get_points()) {
        if (spline_point.position.add(spline_point.velocity * spline.get_time_scale() * VELOCITY_DISPLAY_MULTIPLIER).get_distance_to(clicked_position - Vec<float>(GetMouseDelta()).screen_to_field() / camera.zoom) <= GRAB_DISTANCE / camera.zoom) {
            spline_point.velocity = (clicked_position - spline_point.position) / (VELOCITY_DISPLAY_MULTIPLIER * spline.get_time_scale());
            reset();
            return;
        }

        if (spline_point.position.get_distance_to(clicked_position - Vec<float>(GetMouseDelta()).screen_to_field() / camera.zoom) <= GRAB_DISTANCE / camera.zoom) { 
            spline_point.position.x = clicked_position.x;
            spline_point.position.y = clicked_position.y;
            reset();
            return;
        }
    }
}

void input() {
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

        float camera_speed = BASE_CAMERA_MOVEMENT_SPEED * GetFrameTime() / camera.zoom;
        float zoom_speed = BASE_CAMERA_ZOOM_SPEED * GetFrameTime();
        float duration_edit_speed = BASE_DURATION_EDIT_MULTIPLIER * GetFrameTime();

        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            camera_speed = SPRINT_CAMERA_MOVEMENT_SPEED * GetFrameTime() / camera.zoom;
            zoom_speed = SPRINT_CAMERA_ZOOM_SPEED * GetFrameTime();
            duration_edit_speed = SPRINT_DURATION_EDIT_MULTIPLIER * GetFrameTime();
        }

        if (IsKeyDown(KEY_E)) {
            camera.zoom += zoom_speed;
        }

        if (IsKeyDown(KEY_Q)) {
            camera.zoom -= zoom_speed;
            camera.zoom = std::max(0.01f, camera.zoom);
        }

        if (IsKeyDown(KEY_W)) {
            camera.target.y -= camera_speed;
        }

        if (IsKeyDown(KEY_A)) {
            camera.target.x -= camera_speed;
        }

        if (IsKeyDown(KEY_S)) {
            camera.target.y += camera_speed;
        }

        if (IsKeyDown(KEY_D)) {
            camera.target.x += camera_speed;
        }

        if (IsKeyDown(KEY_UP)) {
            spline.total_duration += duration_edit_speed;
            reset();
        }

        if (IsKeyDown(KEY_DOWN)) {
            spline.total_duration -= duration_edit_speed;
            reset();
        }

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            drag_points(spline,camera);
        }

        if (IsKeyPressed(KEY_P)) {
            spline.print_parameters();
        }
}

void update() {
    input();
    
    float path_length = 0;
    BeginDrawing();
        BeginMode2D(camera);
            ClearBackground({100,100,100,255});
            draw_grid();
            draw_field();
            
            float path_index = 0;
            bool does_overspeed = false;
            bool does_over_acceleration = false;
            while (true) {
                Motion path_motion = spline.get_motion_at(path_index);
                path_motion.velocity *= spline.get_time_scale();
                path_motion.acceleration *= spline.get_time_scale();
                path_length += path_motion.position.get_distance_to(spline.get_position_at(path_index + PATH_INDEX_DELTA));
                bool overspeed = path_motion.velocity.magnitude() > MAX_VELOCITY;
                if (overspeed) {
                    does_overspeed = true;
                }
                bool over_acceleration = path_motion.acceleration.magnitude() > MAX_ACCELERATION;
                if (over_acceleration) {
                    does_over_acceleration = true;
                }
                if (overspeed and over_acceleration) {
                    DrawLineField(path_motion.position, spline.get_position_at(path_index + PATH_INDEX_DELTA), 0.05f, RED);
                } else if (over_acceleration) {
                    DrawLineField(path_motion.position, spline.get_position_at(path_index + PATH_INDEX_DELTA), 0.04f, ORANGE);
                } else if (overspeed) {
                    DrawLineField(path_motion.position, spline.get_position_at(path_index + PATH_INDEX_DELTA), 0.04f, YELLOW);
                } else {
                    DrawLineField(path_motion.position, spline.get_position_at(path_index + PATH_INDEX_DELTA), 0.03f, PATH_COLOR);
                }

                if (path_index > spline.get_index_total() - PATH_INDEX_DELTA) {
                    break;
                }
                path_index += PATH_INDEX_DELTA;  
            }

            for (HermitePoint &spline_point: spline.get_points()) {
                DrawLineField(spline_point.position, spline_point.position.add(spline_point.velocity * spline.get_time_scale() * VELOCITY_DISPLAY_MULTIPLIER), 0.015f, {10, 200, 200, 240});
                DrawCircleField(spline_point.position.add(spline_point.velocity * spline.get_time_scale() * VELOCITY_DISPLAY_MULTIPLIER), 0.07f, {10, 200, 200, 240});
                DrawCircleField(spline_point.position, 0.07f, PATH_COLOR);
            }
            Motion motion = spline.get_motion_at(simulation_time * spline.get_time_scale());
            motion.velocity.multiply_in_place(spline.get_time_scale());
            motion.acceleration.multiply_in_place(spline.get_time_scale());

            DrawPolyField(motion.position, 4, ROBOT_SIZE / 2.0f, (motion.velocity.atan2() / PI) * 180 + 45, {200, 10, 200, 175});

            DrawLineField(motion.position, motion.position.add(motion.velocity.multiply(spline.get_time_scale()).multiply(VELOCITY_DISPLAY_MULTIPLIER)), 0.025f, BLUE);
            DrawLineField(motion.position.add(motion.velocity * spline.get_time_scale() * VELOCITY_DISPLAY_MULTIPLIER), motion.position.add(motion.velocity * spline.get_time_scale() * VELOCITY_DISPLAY_MULTIPLIER).add(motion.acceleration * spline.get_time_scale() * ACCELERATION_DISPLAY_MULTIPLIER), 0.025, GREEN);

            if (!paused) {
                test_position += motion.velocity * GetFrameTime();
            }
            DrawCircleField(test_position, .1f, PURPLE);

            for (HermitePoint &spline_point: spline.get_points()) {  
                const float size = .2f;
                const float spacing = .01f;
                DrawTextField(spline_point.position.to_string_2f().c_str(), spline_point.position.add({0.3f, -0.1f}), size, spacing, GREEN);
                DrawTextField(std::format("{0:.2f} ft/s", spline_point.velocity.multiply(spline.get_time_scale()).magnitude()).c_str(), spline_point.position.add({-0.2f, -0.1f}), size, spacing, {10, 200, 200, 240});
                DrawTextField(spline_point.velocity.to_string_2f().c_str(), spline_point.position.add({-0.4f, -0.1f}), size, spacing, {10, 200, 200, 240});
            }
        EndMode2D();

        

        DrawFPS(WINDOW.x * 0.9f, WINDOW.y * 0.05f);
        DrawText(std::format("Time {0:.2f} / {1:.2f}", simulation_time, spline.index_to_time(spline.get_index_total())).c_str(), 25, 20, 20, WHITE);
        DrawText(std::format("Position {0:.2f} / {1:.2f}", current_position, path_length).c_str(), 25, 40, 20, WHITE);
        DrawText(std::format("Velocity {0:.2f} / {1:.2f}", motion.velocity.magnitude(), MAX_VELOCITY).c_str(), 25, 60, 20, WHITE);
        DrawText(std::format("Acceleration {0:.2f} / {1:.2f}", motion.acceleration.magnitude(), MAX_ACCELERATION).c_str(), 25, 80, 20, WHITE);

        if (does_overspeed) {
            DrawText("Warning: Path Exceeds Speed Constraint", 30, WINDOW.y - 60, 20, YELLOW);
        }
        if (does_over_acceleration) {
            DrawText("Warning: Path Exceeds Acceleration Constraint", 30, WINDOW.y - 40, 20, ORANGE);
        }
    EndDrawing();
    
    if (!paused) {
        simulation_time += GetFrameTime();
        current_position += motion.velocity.magnitude() * GetFrameTime();

        if (current_position > path_length) {
            reset();
        }
    }
}

int main() {
    setup_spline(); 
    test_position = spline.get_position_at(0); 

    if (spline.get_point_count() <= 1) {
        printf("Spling must have at least 2 points\n");
        return 0;
    }  

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow((int) WINDOW.x, (int) WINDOW.y, "Hermite Spline Test");
    
    SetTargetFPS(200);

    camera.zoom = 80.0f;
    camera.offset = {(float) WINDOW.x / 2.0f, (float) WINDOW.y / 2.0f};

    while (!WindowShouldClose()) {
        update();
    }
    CloseWindow();
    return 0;
}


// coordinate system: +x is up -y is right