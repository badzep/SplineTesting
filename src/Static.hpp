#pragma once

#include "Hermite.hpp"

static bool paused = false;
static float current_time = 0;
static float current_position = 0;
static HermiteSpline spline = {};

static void reset() {
	current_time = 0;
	current_position = 0;
}