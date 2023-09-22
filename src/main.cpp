#include <fstream>
#include <iostream>
#include <raylib.h>
#include <format>
#include <filesystem>


#include "Hermite.hpp"
#include "Constants.hpp"
#include "MotionProfile.hpp"
#include "Vector.hpp"


Camera2D camera = {};
bool paused = false;
float simulation_time = 0;
float current_position = 0;

void reset() {
    simulation_time = 0;
    current_position = 0;
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
}

void draw_field() {
    // loading zones
    DrawLineEx({4,6}, {6,4}, TWO_INCHES, BLUE);
    DrawLineEx({4,-6}, {6,-4}, TWO_INCHES, BLUE);

    DrawLineEx({-4,6}, {-6,4}, TWO_INCHES, RED);
    DrawLineEx({-4,-6}, {-6,-4}, TWO_INCHES, RED);

    // center black I thing
    DrawLineEx({0,-4}, {0,4}, TWO_INCHES, BLACK);
    DrawLineEx({-2,-4}, {2,-4}, TWO_INCHES, BLACK);
    DrawLineEx({-2,4}, {2,4}, TWO_INCHES, BLACK);

    // goal net things
    DrawCircleV({-4,-2}, 5.0f * ONE_INCH, BLUE);
    DrawCircleV({-4,2}, 5.0f * ONE_INCH, BLUE);
    DrawLineEx({-4,-2}, {-4,2}, ONE_INCH, BLUE);
    DrawLineEx({-4,-2}, {-6,-2}, ONE_INCH, BLUE);
    DrawLineEx({-4,2}, {-6,2}, ONE_INCH, BLUE);

    DrawCircleV({4,-2}, 5.0f * ONE_INCH, RED);
    DrawCircleV({4,2}, 5.0f * ONE_INCH, RED);
    DrawLineEx({4,-2}, {4,2}, ONE_INCH, RED);
    DrawLineEx({4,-2}, {6,-2}, ONE_INCH, RED);
    DrawLineEx({4,2}, {6,2}, ONE_INCH, RED);
}

void drag_points(HermiteSpline &spline, Camera2D &camera) {
    Vec<float> clicked_position = Vec<float>(GetScreenToWorld2D(GetMousePosition(), camera));
    for (HermitePoint &spline_point: spline.get_points()) {
        if (spline_point.position.add(spline_point.velocity.multiply(spline.get_time_scale()).multiply(VELOCITY_DISPLAY_MULTIPLIER)).get_distance_to(clicked_position.subtract(Vec<float>(GetMouseDelta()).divide(camera.zoom))) <= GRAB_DISTANCE / camera.zoom) {
            spline_point.velocity = clicked_position.subtract(spline_point.position).divide(VELOCITY_DISPLAY_MULTIPLIER).divide(spline.get_time_scale());
            reset();
            return;
        }
    }

    for (HermitePoint &spline_point: spline.get_points()) {
        if (spline_point.position.get_distance_to(clicked_position.subtract(Vec<float>(GetMouseDelta()).divide(camera.zoom))) <= GRAB_DISTANCE / camera.zoom) { // 
            spline_point.position.x = clicked_position.x;
            spline_point.position.y = clicked_position.y;
            reset();
            return;
        }
    }
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow((int) WINDOW.x, (int) WINDOW.y, "Hermite Spline Test");
    
    SetTargetFPS(60);

    HermiteSpline spline;

    // spline.total_duration = 5.0f;
    // spline.add_point(HermitePoint{Vec<float>{-3,-3}, Vec<float>{0,0}});
    // spline.add_point(HermitePoint{Vec<float>{-2,0}, Vec<float>{0,4}});
    // spline.add_point(HermitePoint{Vec<float>{-3,4}, Vec<float>{2,3}});
    // spline.add_point(HermitePoint{Vec<float>{0,5}, Vec<float>{4,0}});
    // spline.add_point(HermitePoint{Vec<float>{3,4}, Vec<float>{0.3,-4}});
    // spline.add_point(HermitePoint{Vec<float>{2,1}, Vec<float>{0,-4}});
    // spline.add_point(HermitePoint{Vec<float>{2,-2}, Vec<float>{0,0}});
    
    spline.total_duration = 4.0f;
    spline.add_point(HermitePoint{Vec<float>{-3,-3}, Vec<float>{0,0}});
    spline.add_point(HermitePoint{Vec<float>{-2.6875,-0.7}, Vec<float>{-0.233821,3.3069}});
    spline.add_point(HermitePoint{Vec<float>{-2.9625,2.5125}, Vec<float>{0.601254,3.20669}});
    spline.add_point(HermitePoint{Vec<float>{-1.7125,4.8375}, Vec<float>{3.3069,0.066805}});
    spline.add_point(HermitePoint{Vec<float>{1.575,4.925}, Vec<float>{3.3,-0.0333328}});
    spline.add_point(HermitePoint{Vec<float>{2.6375,2.65}, Vec<float>{-0.167015,-3.3069}});
    spline.add_point(HermitePoint{Vec<float>{2.175,-0.625}, Vec<float>{-0.601252,-3.27349}});

    camera.zoom = 80.0f;
    camera.offset = {(float) WINDOW.x / 2.0f, (float) WINDOW.y / 2.0f}; // center camera

    
    while (!WindowShouldClose()) {
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

        float camera_speed = BASE_CAMERA_MOVEMENT_SPEED * GetFrameTime();
        float zoom_speed = BASE_CAMERA_ZOOM_SPEED * GetFrameTime();
        float duration_edit_speed = BASE_DURATION_EDIT_MULTIPLIER * GetFrameTime();
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            camera_speed = SPRINT_CAMERA_MOVEMENT_SPEED * GetFrameTime();
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
            camera.target.y -= camera_speed / camera.zoom;
        }

        if (IsKeyDown(KEY_A)) {
            camera.target.x -= camera_speed / camera.zoom;
        }

        if (IsKeyDown(KEY_S)) {
            camera.target.y += camera_speed / camera.zoom;
        }

        if (IsKeyDown(KEY_D)) {
            camera.target.x += camera_speed / camera.zoom;
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
                    Motion path_motion = spline.get_point_at(path_index);
                    path_motion.velocity.multiply_in_place(spline.get_time_scale());
                    path_motion.acceleration.multiply_in_place(spline.get_time_scale());
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
                        DrawLineEx(spline.get_position_at(path_index).to_raylib(), spline.get_position_at(path_index + PATH_INDEX_DELTA).to_raylib(), 0.05f, RED);
                    } else if (over_acceleration) {
                        DrawLineEx(spline.get_position_at(path_index).to_raylib(), spline.get_position_at(path_index + PATH_INDEX_DELTA).to_raylib(), 0.04f, ORANGE);
                    } else if (overspeed) {
                        DrawLineEx(spline.get_position_at(path_index).to_raylib(), spline.get_position_at(path_index + PATH_INDEX_DELTA).to_raylib(), 0.04f, YELLOW);
                    } else {
                        DrawLineEx(spline.get_position_at(path_index).to_raylib(), spline.get_position_at(path_index + PATH_INDEX_DELTA).to_raylib(), 0.03f, PATH_COLOR);
                    }

                    if (path_index > spline.get_index_total() - PATH_INDEX_DELTA) {
                        break;
                    }
                    path_index += PATH_INDEX_DELTA;  
                }

                for (HermitePoint &spline_point: spline.get_points()) {
                    DrawLineEx(spline_point.position.to_raylib(), spline_point.position.add(spline_point.velocity.multiply(spline.get_time_scale()).multiply(VELOCITY_DISPLAY_MULTIPLIER)).to_raylib(), 0.015f, {10, 200, 200, 240});
                    DrawCircleV(spline_point.position.add(spline_point.velocity.multiply(spline.get_time_scale()).multiply(VELOCITY_DISPLAY_MULTIPLIER)).to_raylib(), 0.07f, {10, 200, 200, 240});
                    DrawCircleV(spline_point.position.to_raylib(), 0.07f, PATH_COLOR);
                }
                Motion motion = spline.get_point_at(simulation_time * spline.get_time_scale());
                motion.velocity.multiply_in_place(spline.get_time_scale());
                motion.acceleration.multiply_in_place(spline.get_time_scale());
                DrawPoly(motion.position.to_raylib(), 4, ROBOT_SIZE / 2.0f, (motion.velocity.atan2() / PI) * 180 + 45, {200, 10, 200, 175});

                DrawLineEx(motion.position.to_raylib(), motion.position.add(motion.velocity.multiply(spline.get_time_scale()).multiply(VELOCITY_DISPLAY_MULTIPLIER)).to_raylib(), 0.025f, BLUE);
                DrawLineEx(motion.position.add(motion.velocity.multiply(spline.get_time_scale()).multiply(VELOCITY_DISPLAY_MULTIPLIER)).to_raylib(),motion.position.add(motion.velocity.multiply(spline.get_time_scale()).multiply(VELOCITY_DISPLAY_MULTIPLIER)).add(motion.acceleration.multiply(spline.get_time_scale()).multiply(ACCELERATION_DISPLAY_MULTIPLIER)).to_raylib(), 0.025, GREEN);
            EndMode2D();

            for (HermitePoint &spline_point: spline.get_points()) {
                Vector2 position_text_position = GetWorldToScreen2D(spline_point.position.to_raylib(), camera);
                position_text_position.x += 10;
                position_text_position.y -= 20;
                DrawTextEx(GetFontDefault(), spline_point.position.to_string_2f().c_str(), position_text_position, 17.0f, 1.0f, GREEN);
                Vector2 velocity_text_position = GetWorldToScreen2D(spline_point.position.to_raylib(), camera);
                velocity_text_position.x += 10;
                velocity_text_position.y += 20;
                DrawTextEx(GetFontDefault(), std::format("{0:.2f} ft/s", spline_point.velocity.multiply(spline.get_time_scale()).magnitude()).c_str(), velocity_text_position, 17.0f, 1.0f, {10, 200, 200, 240});
                velocity_text_position.y += 20;
                DrawTextEx(GetFontDefault(), spline_point.velocity.to_string_2f().c_str(), velocity_text_position, 17.0f, 1.0f, {10, 200, 200, 240});
            }

            DrawFPS(WINDOW.x * 0.9f, WINDOW.y * 0.05f);
            DrawText(std::format("Time {0:.2f} / {1:.2f}", simulation_time, spline.index_to_time(spline.get_index_total())).c_str(), 25, 20, 20, WHITE);
            DrawText(std::format("Position {0:.2f} / {1:.2f}", current_position, path_length).c_str(), 25, 40, 20, WHITE);
            DrawText(std::format("Velocity {0:.2f} / {1:.2f}", motion.velocity.magnitude(), MAX_VELOCITY).c_str(), 25, 60, 20, WHITE);
            DrawText(std::format("Acceleration {0:.2f} / {1:.2f}", motion.acceleration.magnitude(), MAX_ACCELERATION).c_str(), 25, 80, 20, WHITE);

            if (does_overspeed) {
                DrawText("Warning: Path Exceeds Speed Constraint", 20, WINDOW.y - 60, 20, YELLOW);
            }
            if (does_over_acceleration) {
                DrawText("Warning: Path Exceeds Acceleration Constraint", 20, WINDOW.y - 40, 20, ORANGE);
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
    return 0;
}
