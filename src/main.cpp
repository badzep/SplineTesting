#include <fstream>
#include <iostream>
#include <raylib.h>


#include <format>
#include <filesystem>


#include "Hermite.hpp"
#include "Constants.hpp"
#include "MotionProfile.hpp"


void drag_points(HermiteSpline &spline, Camera2D &camera) {
    Vec<float> clicked_position = Vec<float>(GetScreenToWorld2D(GetMousePosition(), camera));
    for (HermitePoint &spline_point: spline.get_points()) {
        if (spline_point.get_velocity_end_point().get_distance_to(clicked_position.subtract(Vec<float>(GetMouseDelta()).divide(camera.zoom))) <= DRAG_DISTANCE) { // camera.zoom
            spline_point.velocity = clicked_position.subtract(spline_point.position).divide(VELOCITY_DISPLAY_MULTIPLIER);
            return;
        }
    }

    for (HermitePoint &spline_point: spline.get_points()) {
        if (spline_point.position.get_distance_to(clicked_position.subtract(Vec<float>(GetMouseDelta()).divide(camera.zoom))) <= DRAG_DISTANCE) { // / camera.zoom
            spline_point.position.x = clicked_position.x;
            spline_point.position.y = clicked_position.y;
            return;
        }
    }
}


int main() {
    InitWindow((int) WINDOW.x, (int) WINDOW.y, "Hermite Spline Test");
    SetTargetFPS(200);

    Camera2D camera = {};
    bool paused = false;

    HermiteSpline spline;
    
    if (!std::filesystem::exists(POINTS_PATH)) {
        std::ofstream _file = std::ofstream();
        _file.open(POINTS_PATH, std::ios::out);
        _file << 0.0f;
        _file << "\n";
        _file << 0.0f;
        _file << "\n";
        _file << 0.0f;
        _file << "\n";
        _file << 50.0f;
        _file << "\n";
        _file << "\n";
        _file << 5.0f;
        _file << "\n";
        _file << 5.0f;
        _file << "\n";
        _file << 50.0f;
        _file << "\n";
        _file << 50.0f;
        _file.close();
    }

    std::ifstream file = std::ifstream();
    file.open(POINTS_PATH, std::ios::in);
    int x = 0;
    while (true) {
        float position_x;
        float position_y;
        float velocity_x;
        float velocity_y;
        file >> position_x;
        file >> position_y;
        file >> velocity_x;
        file >> velocity_y;
        std::cout << position_x << "\n";
        std::cout << position_y << "\n";
        std::cout << velocity_x << "\n";
        std::cout << velocity_y << "\n";
        spline.add_point({{position_x, position_y},{velocity_x, velocity_y}});
        if (file.eof()) {
            break;
        }
    }

    file.close();
    
    camera.zoom = 10.0f;
    camera.offset = {(float) WINDOW.x / 2.0f, (float) WINDOW.y / 2.0f}; // center camera

    float time = 0;
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

        if (IsKeyDown(KEY_E)) {
            camera.zoom += zoom_speed;
        }

        if (IsKeyDown(KEY_Q)) {
            camera.zoom -= zoom_speed;
            camera.zoom = std::max(0.01f, camera.zoom);
        }

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            drag_points(spline,camera);
        }
        
        float path_length = 0;
        BeginDrawing();
            BeginMode2D(camera);
                ClearBackground(BLACK);
                float path_time = 0;
                while (true) {
                    path_length += spline.get_position_at(path_time).get_distance_to(spline.get_position_at(path_time + PATH_DELTA));
                    DrawLineV(spline.get_position_at(path_time).to_raylib(), spline.get_position_at(path_time + PATH_DELTA).to_raylib(), PATH_COLOR);
                    path_time += PATH_DELTA;
                    if (path_time > ((float) spline.get_point_count() - 1.0f) - PATH_DELTA) {
                        break;
                    }
                }
                for (HermitePoint &spline_point: spline.get_points()) {
                    DrawLineV(spline_point.position.to_raylib(), spline_point.position.add(spline_point.velocity.multiply(VELOCITY_DISPLAY_MULTIPLIER)).to_raylib(), RED);
                    DrawCircleV(spline_point.get_velocity_end_point().to_raylib(), 2.0f / camera.zoom, RED);
                    DrawCircleV(spline_point.position.to_raylib(), 2.5f / camera.zoom, PATH_COLOR);
                }

                AsymmetricalProfile profile = AsymmetricalProfile(MAX_ACCELERATION, MAX_DECELERATION, MAX_VELOCITY, path_length);
                MotionState state = profile.getState(time);
                
                float current_index = 0;
                float current_position = 0;
                while (true) {
                    current_position += spline.get_position_at(current_index).get_distance_to(spline.get_position_at(current_index + INDEX_DELTA));
                    current_index += INDEX_DELTA;
                    if (current_position >= state.x) {
                        break;
                    }
                }
                Vec<float> position = spline.get_position_at(current_index);
                Motion point = spline.get_point_at(current_index);
                const float velocity_angle = point.velocity.atan2();
                Vec<float> velocity = {std::cos(velocity_angle) * state.v, std::sin(velocity_angle) * state.v};
                Vec<float> velocity_position = position.add(velocity);
                const float acceleration_angle = point.acceleration.atan2();
                Vec<float> acceleration = {std::cos(acceleration_angle) * state.a, std::sin(acceleration_angle) * state.a};
                Vec<float> acceleration_position = velocity_position.add(acceleration); 
                DrawLineV(point.position.to_raylib(), velocity_position.to_raylib(), RED);
                DrawLineV(velocity_position.to_raylib(), acceleration_position.to_raylib(), ORANGE);
                DrawCircleV(position.to_raylib(), 5.0f / camera.zoom, PURPLE);

            EndMode2D();
            DrawText(std::format("Position {0:.2f} / {1:.2f}", state.x, path_length).c_str(), 20, 20, 20, WHITE);
            DrawText(std::format("Velocity {0:.2f} / {1:.2f}", state.v, MAX_VELOCITY).c_str(), 20, 40, 20, WHITE);
            DrawText(std::format("Acceleration {0:.2f} / [{1:.2f}, {2:.2f}]", state.a, -MAX_DECELERATION, MAX_ACCELERATION).c_str(), 20, 60, 20, WHITE);
        EndDrawing();
        
        if (!paused) {
            time += TIME_DELTA;
            if (current_position > path_length) {
                time = 0;
            }
        }
    }
    return 0;
}
