#pragma once

#include <raylib.h>

constexpr Vector2 WINDOW = {1000, 1000};
constexpr float VELOCITY_DISPLAY_MULTIPLIER = 0.1f;
constexpr float ACCELERATION_DISPLAY_MULTIPLIER = 0.01f;

constexpr Color PATH_COLOR = {10, 100, 200, 200};

constexpr float TIME_DELTA = 2e-3; // time step size when moving across spline
constexpr float PATH_DELTA = 1e-2; // time step size when drawing path

constexpr float DRAG_DISTANCE = 7.0f;

constexpr float BASE_CAMERA_MOVEMENT_SPEED = 40;
constexpr float SPRINT_CAMERA_MOVEMENT_SPEED = BASE_CAMERA_MOVEMENT_SPEED * 2.5f;
constexpr float BASE_CAMERA_ZOOM_SPEED = 4.0f;
constexpr float SPRINT_CAMERA_ZOOM_SPEED = BASE_CAMERA_ZOOM_SPEED * 5.0f;