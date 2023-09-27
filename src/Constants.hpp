#pragma once


#include "Vec.hpp"
#include <raylib.h>
#include <string>


constexpr float mm_to_feet(const float feet) {
	return feet * 0.0032808f;
}


constexpr float BASE_DURATION_EDIT_MULTIPLIER = 0.25f;
constexpr float SPRINT_DURATION_EDIT_MULTIPLIER = BASE_DURATION_EDIT_MULTIPLIER * 10;

constexpr Vector2 WINDOW = {1000, 1000};
constexpr float DURATION_DISPLAY_MULTIPLIER = 0.6f;
constexpr float VELOCITY_DISPLAY_MULTIPLIER = 0.25f;
constexpr float ACCELERATION_DISPLAY_MULTIPLIER = 0.05f;

constexpr float MINIMUM_SPLINE_DURATION = 0.25f;

constexpr Color PATH_COLOR = {10, 200, 100, 200};

constexpr float PATH_INDEX_DELTA = 1e-3; // time step size when drawing path

constexpr float GRAB_DISTANCE = 15.0f;

constexpr float BASE_CAMERA_MOVEMENT_SPEED = 100;
constexpr float SPRINT_CAMERA_MOVEMENT_SPEED = BASE_CAMERA_MOVEMENT_SPEED * 2.5f;
constexpr float BASE_CAMERA_ZOOM_SPEED = 1.0f;
constexpr float SPRINT_CAMERA_ZOOM_SPEED = BASE_CAMERA_ZOOM_SPEED * 5.0f;

constexpr float ROBOT_SIZE = 15.0f / 12.0f;
constexpr float GRID_SIZE = 6.0f;

constexpr float MAX_VELOCITY = 5.0f;
constexpr float MAX_ACCELERATION = 20.0f;

constexpr float ONE_FOOT = 1.0f;
constexpr float TWO_FEET = 2.0f;

constexpr float ONE_INCH = 1.0f / 12.0f;
constexpr float TWO_INCHES = 1.0f / 6.0f;

constexpr float NODE_SIZE = 0.07f;
constexpr float PATH_HEIGHT = 0.1f;
constexpr float VELOCITY_NODE_HEIGHT = PATH_HEIGHT + NODE_SIZE;
constexpr float ROBOT_HEIGHT = 0.6f;
constexpr float ROBOT_NODE_HEIGHT = ROBOT_HEIGHT + NODE_SIZE;

constexpr float WALL_HEIGHT = mm_to_feet(300.0f);
constexpr float GOAL_ELEVATION = 0.5f;

constexpr float NET_SPACING_X = 4.0f / 20.0f;
const float NET_SPACING_Y = std::hypot(2.0f, WALL_HEIGHT - 0.5f) / 10.0f;
constexpr Vec2<unsigned char> NET_SIZE = {20, 10};
const static float NET_ANGLE = std::atan((WALL_HEIGHT - 0.5f) / 2.0f);
