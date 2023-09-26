#pragma once


#include <chrono>
#include <iostream>
#include <stdexcept>
#include <vector>


#include "Constants.hpp"
#include "Vec.hpp"
#include "raylib.h"


class UniformCubicSpline {
public:
	Vec2f t0_coefficient; // coefficient of t^0
	Vec2f t1_coefficient; // coefficient of t^1
	Vec2f t2_coefficient; // coefficient of t^2
	Vec2f t3_coefficient; // coefficient of t^3

	UniformCubicSpline(const Vec2f t0_coefficient, const Vec2f t1_coefficient, const Vec2f t2_coefficient, const Vec2f t3_coefficient):
	t0_coefficient(t0_coefficient),
	t1_coefficient(t1_coefficient),
	t2_coefficient(t2_coefficient),
	t3_coefficient(t3_coefficient)
	{	}

	Vec2f get_position_at(const float index) {
		return this->t0_coefficient + this->t1_coefficient * index + this->t2_coefficient * index * index + this->t3_coefficient * index * index * index;
	}

	Vec2f get_velocity_at(const float index) {
		return this->t1_coefficient + this->t2_coefficient * index * 2 + this->t3_coefficient * index * index * 3;
	}

	Vec2f get_acceleration_at(const float index) {
		return this->t2_coefficient * 2 + this->t3_coefficient * index * 6;
	}
};

class CubicSpline {
public:
	float start_time;
	float end_time;
	float duration;
	Vec2f t0_coefficient; // coefficient of t^0
	Vec2f t1_coefficient; // coefficient of t^1
	Vec2f t2_coefficient; // coefficient of t^2
	Vec2f t3_coefficient; // coefficient of t^3

	CubicSpline(const float start_time, const float end_time, const Vec2f t0_coefficient, const Vec2f t1_coefficient, const Vec2f t2_coefficient, const Vec2f t3_coefficient):
	start_time(start_time),
	end_time(end_time),
	duration(end_time - start_time),
	t0_coefficient(t0_coefficient),
	t1_coefficient(t1_coefficient / duration),
	t2_coefficient(t2_coefficient / (duration * duration)),
	t3_coefficient(t3_coefficient / (duration * duration * duration))
	{	}

	Vec2f get_position_at(float time) {
		time -= this->start_time;
		return this->t0_coefficient + this->t1_coefficient * time + this->t2_coefficient * time * time + this->t3_coefficient * time * time * time;
	}

	Vec2f get_velocity_at(float time) {
		time -= this->start_time;
		return this->t1_coefficient + this->t2_coefficient * time * 2 + this->t3_coefficient * time * time * 3;
	}

	Vec2f get_acceleration_at(float time) {
		time -= this->start_time;
		return this->t2_coefficient * 2 + this->t3_coefficient * time * 6;
	}
};

static CubicSpline hermite_factory(const float start_time, const float end_time, const Vec2f start_point, Vec2f start_velocity, const Vec2f end_point, Vec2f end_velocity) {
	start_velocity *= end_time - start_time;
	end_velocity *= end_time - start_time;
	return CubicSpline(start_time, end_time, start_point, start_velocity, start_point * -3 + start_velocity * -2 + end_point * 3 + end_velocity * -1, start_point * 2 + start_velocity + end_point * -2 + end_velocity);
}

static CubicSpline bezier_factory(const float start_time, const float end_time, const Vec2f start_point, Vec2f control_point1, Vec2f control_point2, const Vec2f end_point) {
	control_point1 *= end_time - start_time;
	control_point2 *= end_time - start_time;
	return CubicSpline(start_time, end_time, start_point, start_point * -3 + control_point1 * 3, start_point * 3 + control_point1 * -6 + control_point2 * 3, start_point * -1 + control_point1 * 3 + control_point2 * -3 + end_point);
}

static CubicSpline cardinal_factory(const float start_time, const float end_time, const float scale, const Vec2f point1, const Vec2f point2, const Vec2f point3, const Vec2f point4) {
	return CubicSpline(start_time, end_time, point2, point1 * -scale + point3 * scale, point1 * 2 * scale + point2 * (scale-3) + point3 * (3 - 2 * scale) + point4 * -scale, point1 * -scale + point2 * (2 - scale) + point3 * (scale - 2) + point4 * scale);
}

static CubicSpline catmull_rom_factory(const float start_time, const float end_time, const Vec2f point1, const Vec2f point2, const Vec2f point3, const Vec2f point4) {
	return cardinal_factory(start_time, end_time, 0.5f, point1, point2, point3, point4);
}

// static CubicSpline hermite_to_bezier(const CubicSpline& hermite) {
// 	return CubicSpline(hermite.start_time, hermite.end_time, hermite.t0_coefficient, hermite.t0_coefficient + hermite.t1_coefficient / 3.0f, hermite.t2_coefficient + hermite.t3_coefficient / 3.0f, hermite.t2_coefficient);
// }

static UniformCubicSpline uniform_hermite_factory(const Vec2f start_point, const Vec2f start_velocity, const Vec2f end_point, const Vec2f end_velocity) {
	return UniformCubicSpline(start_point, start_velocity, start_point * -3 + start_velocity * -2 + end_point * 3 + end_velocity * -1, start_point * 2 + start_velocity + end_point * -2 + end_velocity);
}

static UniformCubicSpline uniform_bezier_factory(const Vec2f start_point, const Vec2f control_point1, const Vec2f control_point2, const Vec2f end_point) {
	return UniformCubicSpline(start_point, start_point * -3 + control_point1 * 3, start_point * 3 + control_point1 * -6 + control_point2 * 3, start_point * -1 + control_point1 * 3 + control_point2 * -3 + end_point);
}

static UniformCubicSpline uniform_cardinal_factory(const float scale, const Vec2f displacement_from_last_point, const Vec2f start_point, const Vec2f end_point, const Vec2f displacement_to_next_point) {
	return UniformCubicSpline(
	                          start_point,
	                          displacement_from_last_point * -scale + end_point * scale,
	                          displacement_from_last_point * 2 * scale + start_point * (scale - 3) + end_point * (3 - 2 * scale) + displacement_to_next_point * -scale,
	                          displacement_from_last_point * -scale + start_point * (2 - scale) + end_point * (scale - 2) + displacement_to_next_point * scale
	                          );
}

static UniformCubicSpline uniform_catmull_rom_factory(const Vec2f point1, const Vec2f point2, const Vec2f point3, const Vec2f point4) {
	return uniform_cardinal_factory(0.5f, point1, point2, point3, point4);
}

// static CubicSpline b_spline_factory(const Vec2f point1, const Vec2f point2, const Vec2f point3, const Vec2f point4) {
// 	return CubicSpline();
// }


// static UniformCubicSpline uniform_hermite_to_uniform_bezier(const UniformCubicSpline& hermite) {
// 	return UniformCubicSpline(hermite.t0_coefficient, hermite.t0_coefficient + hermite.t1_coefficient / 3.0f, hermite.t2_coefficient + hermite.t3_coefficient / 3.0f, hermite.t2_coefficient);
// }

class Point {
public:
	Vec2f position;
	Vec2f velocity;
};

class Spline {
public:
	float total_duration;
	std::vector<CubicSpline> splines;

	Spline() {	}

	void update_point(const unsigned int index, const Vec2f new_position, const Vec2f new_velocity) {
		if (index > 0) {
			CubicSpline temp_hermite = hermite_factory(this->splines[index - 1].start_time, this->splines[index - 1].end_time, this->splines[index - 1].get_position_at(0), this->splines[index - 1].get_velocity_at(0), new_position, new_velocity);
			this->splines[index - 1].t0_coefficient = temp_hermite.t0_coefficient;
			this->splines[index - 1].t1_coefficient = temp_hermite.t1_coefficient;
			this->splines[index - 1].t2_coefficient = temp_hermite.t2_coefficient;
			this->splines[index - 1].t3_coefficient = temp_hermite.t3_coefficient;
		}
		if (index < this->splines.size()) {
			CubicSpline temp_hermite = hermite_factory(this->splines[index].start_time, this->splines[index].end_time, new_position, new_velocity, this->splines[index].get_position_at(1), this->splines[index].get_velocity_at(1));
			this->splines[index].t0_coefficient = temp_hermite.t0_coefficient;
			this->splines[index].t1_coefficient = temp_hermite.t1_coefficient;
			this->splines[index].t2_coefficient = temp_hermite.t2_coefficient;
			this->splines[index].t3_coefficient = temp_hermite.t3_coefficient;
		}
	}

	void add_spline(const CubicSpline spline) {
		this->splines.push_back(spline);
		this->total_duration += spline.duration;
	}

	Vec2f get_position_at(const float time) {
		for (CubicSpline spline: splines) {
			if (spline.start_time <= time and spline.end_time >= time) {
				return spline.get_position_at(time);
			}
		}
		throw std::runtime_error("Time > Max\n");
	}

	Vec2f get_velocity_at(const float time) {
		for (CubicSpline spline: splines) {
			if (spline.start_time <= time and spline.end_time >= time) {
				return spline.get_velocity_at(time);
			}
		}
		throw std::runtime_error("Time > Max\n");
	}


	Vec2f get_acceleration_at(const float time) {
		for (CubicSpline spline: splines) {
			if (spline.start_time <= time and spline.end_time >= time) {
				return spline.get_acceleration_at(time);
			}
		}
		throw std::runtime_error("Time > Max\n");
	}
};

class SplineBuilder {
public:
	std::vector<Point> points;
	std::vector<float> durations; 

	SplineBuilder(const Vec2f initial_point, const Vec2f initial_control_point) {
		this->points.push_back({initial_point, initial_control_point});
	}

	
	void add_point(const Vec2f point, const Vec2f control_point, const float travel_time) {
		this->points.push_back({point, control_point});
		this->durations.push_back(travel_time);
	}

	virtual void build(Spline& _spline) = 0;
	// {
	// 	_spline.total_duration = 0;
	// 	_spline.splines.clear();
	// 	for (unsigned int i = 0; i < this->points.size() - 1; i++) {
	// 		_spline.add_spline(hermite_factory(_spline.total_duration, _spline.total_duration + this->durations[i], this->points[i].position, this->points[i].velocity, this->points[i + 1].position, this->points[i + 1].velocity));
	// 	}

	// 	// for (unsigned int i = 0; i < this->points.size() - 1; i++) {
	// 	// 	if (i == 0) {
	// 	// 		_spline.add_spline(cardinal_factory(_spline.total_duration, _spline.total_duration + this->durations[i], 0.5f, this->points[i].position + (this->points[i + 1].position - this->points[i].position).rotate(PI), this->points[i].position, this->points[i + 1].position, this->points[i + 2].position));
	// 	// 	} else if (i == this->points.size() - 2) {
	// 	// 		_spline.add_spline(cardinal_factory(_spline.total_duration, _spline.total_duration + this->durations[i], 0.5f, this->points[i - 1].position, this->points[i].position, this->points[i + 1].position, this->points[i + 1].position + (this->points[i + 1].position - this->points[i].position).rotate(PI)));
	// 	// 	} else {
	// 	// 		_spline.add_spline(cardinal_factory(_spline.total_duration, _spline.total_duration + this->durations[i], 0.5f, this->points[i - 1].position, this->points[i].position, this->points[i + 1].position, this->points[i + 2].position));
	// 	// 	}
	// 	// }
	// }

	virtual void print_parameters() = 0;
};

class HermiteBuilder: public SplineBuilder {
public:
	std::vector<Vec2f> positions;
	std::vector<Vec2f> velocities;
	
	HermiteBuilder(const Vec2f initial_position, const Vec2f initial_velocity): SplineBuilder(initial_position, initial_velocity) {

	}

	void build(Spline& _spline) override {
		_spline.total_duration = 0;
		_spline.splines.clear();
		for (unsigned int i = 0; i < this->points.size() - 1; i++) {
			_spline.add_spline(hermite_factory(_spline.total_duration, _spline.total_duration + this->durations[i], this->points[i].position, this->points[i].velocity, this->points[i + 1].position, this->points[i + 1].velocity));
		}
	}

	void print_parameters() override {
		std::cout << "HermiteBuilder builder = HermiteBuilder({" << this->points[0].position.x << ", "<< this->points[0].position.y << "}, {"<< this->points[0].velocity.x << ", "<< this->points[0].velocity.y << "});" << "\n";
        for (unsigned int index = 1; index < this->points.size(); index++) {
        	std::cout << "builder.add_point({" << this->points[index].position.x << ", "<< this->points[index].position.y << "}, {"<< this->points[index].velocity.x << ", "<< this->points[index].velocity.y << "}, " << this->durations[index - 1] << ");\n";
        }
        std::cout << "builder.build(spline);\n";
	}
};
