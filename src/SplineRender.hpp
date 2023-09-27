#pragma once

#include <raylib.h>

#include "Constants.hpp"
#include "Static.hpp"
#include "Spline.hpp"


template<const unsigned char DIMENSIONS>
static void draw_spline_polynomial(const SplinePolynomial<2, float>& test_spline, const float max_velocity, const float max_acceleration);


template<>
void draw_spline_polynomial<2>(const SplinePolynomial<2, float>& test_spline, const float max_velocity, const float max_acceleration) {
	float test_path_time = test_spline.start_time;
	while (true) {
        Vec2f test_path_position = test_spline.get_point_at(test_path_time);
        Vec2f test_path_velocity = test_spline.get_tangent_at(test_path_time);
        Vec2f test_path_acceleation = test_spline.get_tangent_slope_at(test_path_time);
        bool overspeed = test_path_velocity.magnitude() > max_velocity;
        bool over_acceleration = test_path_acceleation.magnitude() > max_acceleration;
        if (overspeed and over_acceleration) {
            DrawLineField(test_path_position, test_spline.get_point_at(test_path_time + PATH_INDEX_DELTA), 0.05f, RED);
        } else if (over_acceleration) {
            DrawLineField(test_path_position, test_spline.get_point_at(test_path_time + PATH_INDEX_DELTA), 0.04f, ORANGE);
        } else if (overspeed) {
            DrawLineField(test_path_position, test_spline.get_point_at(test_path_time + PATH_INDEX_DELTA), 0.04f, YELLOW);
        } else {
            DrawLineField(test_path_position, test_spline.get_point_at(test_path_time + PATH_INDEX_DELTA), 0.03f, PATH_COLOR);
        }
        test_path_time += PATH_INDEX_DELTA;
        if (test_path_time + PATH_INDEX_DELTA >= test_spline.end_time) {
            break;
        }
    }
}

template<>
void draw_spline_polynomial<3>(const SplinePolynomial<2, float>& test_spline, const float max_velocity, const float max_acceleration) {
	float test_path_time = test_spline.start_time;
	while (true) {
        Vec2f test_path_position = test_spline.get_point_at(test_path_time);
        Vec2f test_path_velocity = test_spline.get_tangent_at(test_path_time);
        Vec2f test_path_acceleation = test_spline.get_tangent_slope_at(test_path_time);
        bool overspeed = test_path_velocity.magnitude() > MAX_VELOCITY;
        bool over_acceleration = test_path_acceleation.magnitude() > MAX_ACCELERATION;
        if (overspeed and over_acceleration) {
            DrawLine3D(test_path_position.to_3d(PATH_HEIGHT).to_raylib(), test_spline.get_point_at(test_path_time + PATH_INDEX_DELTA).to_3d(PATH_HEIGHT).to_raylib(), RED);
        } else if (over_acceleration) {
            DrawLine3D(test_path_position.to_3d(PATH_HEIGHT).to_raylib(), test_spline.get_point_at(test_path_time + PATH_INDEX_DELTA).to_3d(PATH_HEIGHT).to_raylib(), ORANGE);
        } else if (overspeed) {
            DrawLine3D(test_path_position.to_3d(PATH_HEIGHT).to_raylib(), test_spline.get_point_at(test_path_time + PATH_INDEX_DELTA).to_3d(PATH_HEIGHT).to_raylib(), YELLOW);
        } else {
            DrawLine3D(test_path_position.to_3d(PATH_HEIGHT).to_raylib(), test_spline.get_point_at(test_path_time + PATH_INDEX_DELTA).to_3d(PATH_HEIGHT).to_raylib(), PATH_COLOR);
        }
        test_path_time += PATH_INDEX_DELTA;
        if (test_path_time + PATH_INDEX_DELTA >= test_spline.end_time) {
            break;
        }
    }
}

template<const unsigned char DIMENSIONS>
static void draw_spline_chain(const Chain<2, float>& test_spline, const float max_velocity, const float max_acceleration) {
	for (const SplinePolynomial<2, float>& spline_polynomial: test_spline.splines) {
		draw_spline_polynomial<DIMENSIONS>(spline_polynomial, max_velocity, max_acceleration);
	}
}
