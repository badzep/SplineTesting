#pragma once


#include <vector>


#include "Constants.hpp"
#include "Vector.hpp"
#include "raylib.h"


class CubicBezier {
public:
	const Vec2<float> t0_coefficient;
	const Vec2<float> t1_coefficient;
	const Vec2<float> t2_coefficient;
	const Vec2<float> t3_coefficient;

	CubicBezier(const Vec2<float> start_point, const Vec2<float> control_point1, const Vec2<float> control_point2, const Vec2<float> end_point): 
	t0_coefficient(start_point),
	t1_coefficient(start_point * -3 + control_point1 * 3),
	t2_coefficient(start_point * 3 + control_point1 * -6 + control_point2 * 3),
	t3_coefficient(start_point * -1 + control_point1 * 3 + control_point2 * -3 + end_point)
	{}

	Vec2<float> get_position_at(const float index) {
		return t0_coefficient + t1_coefficient * index + t2_coefficient * index * index + t3_coefficient * index * index * index;
	}

	Vec2<float> get_tangent_at(const float index) {
		return t1_coefficient + t2_coefficient * index * 2 + t3_coefficient * index * index * 3;
	}
};


class Hermite {
public:
	Vec2<float> t0_coefficient;
	Vec2<float> t1_coefficient;
	Vec2<float> t2_coefficient;
	Vec2<float> t3_coefficient;

	Hermite(const Vec2<float> start_point, const Vec2<float> start_velocity, const Vec2<float> end_point, const Vec2<float> end_velocity):
	t0_coefficient(start_point),
	t1_coefficient(start_velocity),
	t2_coefficient(start_point * -3 + start_velocity * -2 + end_point * 3 + end_velocity * -1),
	t3_coefficient(start_point * 2 + start_velocity + end_point * -2 + end_velocity)
	{}

	CubicBezier to_cubic_bezier() {
		return CubicBezier(this->t0_coefficient, this->t0_coefficient + this->t1_coefficient / 3.0f, this->t2_coefficient + this->t3_coefficient / 3.0f, this->t2_coefficient);
	}

	Vec2<float> get_position_at(const float index) {
		return this->t0_coefficient + this->t1_coefficient * index + this->t2_coefficient * index * index + this->t3_coefficient * index * index * index;
	}

	Vec2<float> get_velocity_at(const float index) {
		return this->t1_coefficient + this->t2_coefficient * index * 2 + this->t3_coefficient * index * index * 3;
	}

	Vec2<float> get_acceleration_at(const float index) {
		return this->t2_coefficient * 2 + this->t3_coefficient * index * 6;
	}
};

class Point {
public:
	Vec2<float> position;
	Vec2<float> velocity;
};

class HermiteSpline {
protected:
	unsigned int length = 0;
	float duration = 5.0f;
	std::vector<Point> points;
	std::vector<Hermite> splines;

public:
	void update_point(const unsigned int index, const Vec2<float> new_position, const Vec2<float> new_velocity) {
		this->points[index].position = new_position;
		this->points[index].velocity = new_velocity;
		if (index > 0) {
			Hermite temp_hermite = Hermite(this->splines[index - 1].get_position_at(0), this->splines[index - 1].get_velocity_at(0), new_position, new_velocity);
			this->splines[index - 1].t0_coefficient = temp_hermite.t0_coefficient;
			this->splines[index - 1].t1_coefficient = temp_hermite.t1_coefficient;
			this->splines[index - 1].t2_coefficient = temp_hermite.t2_coefficient;
			this->splines[index - 1].t3_coefficient = temp_hermite.t3_coefficient;
		}
		if (index < this->length) {
			Hermite temp_hermite = Hermite(new_position, new_velocity, this->splines[index].get_position_at(1), this->splines[index].get_velocity_at(1));
			this->splines[index].t0_coefficient = temp_hermite.t0_coefficient;
			this->splines[index].t1_coefficient = temp_hermite.t1_coefficient;
			this->splines[index].t2_coefficient = temp_hermite.t2_coefficient;
			this->splines[index].t3_coefficient = temp_hermite.t3_coefficient;
		}
	}

	unsigned int get_length() const {
		return this->length;
	}

	float get_duration() const {
		return this->duration;
	}

	void set_duration(const float _duration) {
		this->duration = _duration;
	}

	float get_time_scale() const {
		return (this->length - 1.0f) / this->duration;
	}

	void add_point(const Vec2<float> position, const Vec2<float> velocity) {
		this->points.push_back({position, velocity});
		this->length++;

		if (this->length <= 1) { // cannot add spline with just one point
			return;
		}

		this->splines.push_back(Hermite(this->points[this->length-2].position, this->points[this->length-2].velocity, position, velocity));
	}

	float time_to_index(const float time) {
        return (time / this->duration) * (this->length - 1);
    }
    float index_to_time(const float index) {
        return (index / (this->length - 1)) * this->duration;
    }

	Vec2<float> get_position_at(const float time) {
		const float index = this->time_to_index(time);
		int spline_index;
		if (index == this->length - 1) {
			spline_index = this->length - 2;
		} else {
			spline_index = (int) std::floor(index);
		}
		return this->splines[spline_index].get_position_at(index - spline_index);
	}

	Vec2<float> get_velocity_at(const float time) {
		const float index = this->time_to_index(time);
		int spline_index;
		if (index == this->length - 1) {
			spline_index = this->length - 2;
		} else {
			spline_index = (int) std::floor(index);
		}
		return this->splines[spline_index].get_velocity_at(index - spline_index) * this->get_time_scale();
	}

	Vec2<float> get_acceleration_at(const float time) {
		const float index = this->time_to_index(time);
		int spline_index;
		if (index == this->length - 1) {
			spline_index = this->length - 2;
		} else {
			spline_index = (int) std::floor(index);
		}
		return this->splines[spline_index].get_acceleration_at(index - spline_index) * this->get_time_scale();
	}
};
