#include <fstream>
#include <iostream>
#include <raylib.h>
#include <format>
#include <filesystem>


#include "Hermite.hpp"
#include "Constants.hpp"
#include "MotionProfile.hpp"
#include "Vector.hpp"



float simulation_time = 0;
float current_position = 0;

void reset() {
    simulation_time = 0;
    current_position = 0;
}

void drag_points(HermiteSpline &spline, Camera2D &camera) {
    Vec<float> clicked_position = Vec<float>(GetScreenToWorld2D(GetMousePosition(), camera));
    for (HermitePoint &spline_point: spline.get_points()) {
        if (spline_point.position.add(spline_point.velocity.multiply(spline.get_time_scale()).multiply(VELOCITY_DISPLAY_MULTIPLIER)).get_distance_to(clicked_position.subtract(Vec<float>(GetMouseDelta()).divide(camera.zoom))) <= DRAG_DISTANCE / camera.zoom) {
            spline_point.velocity = clicked_position.subtract(spline_point.position).divide(VELOCITY_DISPLAY_MULTIPLIER).divide(spline.get_time_scale());
            reset();
            return;
        }
    }

    for (HermitePoint &spline_point: spline.get_points()) {
        if (spline_point.position.get_distance_to(clicked_position.subtract(Vec<float>(GetMouseDelta()).divide(camera.zoom))) <= DRAG_DISTANCE / camera.zoom) { // 
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
    
    SetTargetFPS(200);

    Camera2D camera = {};
    bool paused = false;

    HermiteSpline spline;
    spline.add_point({{0,0},{0,0}});
    spline.add_point({{0,2},{5,5}});
    spline.add_point({{-3,1},{-2,-.1}});
    spline.add_point({{-2,3},{1,-.1}});
    spline.add_point({{4.5,4.5},{0,0}});
    
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

        float camera_speed = BASE_CAMERA_MOVEMENT_SPEED / (float) GetFPS();
        float zoom_speed = BASE_CAMERA_ZOOM_SPEED / (float) GetFPS();
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            camera_speed = SPRINT_CAMERA_MOVEMENT_SPEED / (float) GetFPS();
            zoom_speed = SPRINT_CAMERA_ZOOM_SPEED / (float) GetFPS();
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

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            drag_points(spline,camera);
        }
        
        float path_length = 0;
        BeginDrawing();
            BeginMode2D(camera);
                ClearBackground(BLACK);

                int grid_size = 6; // field is 12 by 12 ft
                for (int i = -grid_size; i <= grid_size; i++) {
                    DrawLineV({-(float) grid_size, (float) i}, {(float)grid_size, (float) i}, {255,255,255,175});
                    DrawLineV({(float) i, -(float) grid_size}, {(float) i, (float) grid_size}, {255,255,255,175});
                }
                
                float path_index = 0;
                while (true) {
                    Motion path_motion = spline.get_point_at(path_index);
                    path_motion.velocity.multiply_in_place(spline.get_time_scale());
                    path_motion.acceleration.multiply_in_place(spline.get_time_scale());
                    path_length += path_motion.position.get_distance_to(spline.get_position_at(path_index + PATH_INDEX_DELTA));
                    
                    bool overspeed = path_motion.velocity.magnitude() > MAX_VELOCITY;
                    bool over_acceleration = path_motion.acceleration.magnitude() > MAX_ACCELERATION;
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
                DrawLineEx(motion.position.to_raylib(), motion.get_velocity_end_point().to_raylib(), 0.025f, BLUE);
                DrawLineEx(motion.get_velocity_end_point().to_raylib(), motion.get_acceleration_end_point().to_raylib(), 0.025, GREEN);

            EndMode2D();
            DrawText(std::format("Time {0:.2f} / {1:.2f}", simulation_time, spline.index_to_time(spline.get_index_total())).c_str(), 20, 20, 20, WHITE);
            DrawText(std::format("Position {0:.2f} / {1:.2f}", current_position, path_length).c_str(), 20, 40, 20, WHITE);
            DrawText(std::format("Velocity {0:.2f} / {1:.2f}", motion.velocity.magnitude(), MAX_VELOCITY).c_str(), 20, 60, 20, WHITE);
            DrawText(std::format("Acceleration {0:.2f} / {1:.2f}", motion.acceleration.magnitude(), MAX_ACCELERATION).c_str(), 20, 80, 20, WHITE);
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
