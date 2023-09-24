#pragma once


#include <stdexcept>
#include <vector>


#include "Constants.hpp"
#include "Vector.hpp"
#include "raylib.h"

class UniformCubicSpline {
public:
	Vec2<float> t0_coefficient; // coefficient of t^0
	Vec2<float> t1_coefficient; // coefficient of t^1
	Vec2<float> t2_coefficient; // coefficient of t^2
	Vec2<float> t3_coefficient; // coefficient of t^3

	UniformCubicSpline(const Vec2<float> t0_coefficient, const Vec2<float> t1_coefficient, const Vec2<float> t2_coefficient, const Vec2<float> t3_coefficient):
	t0_coefficient(t0_coefficient),
	t1_coefficient(t1_coefficient),
	t2_coefficient(t2_coefficient),
	t3_coefficient(t3_coefficient)
	{	}

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

class CubicSpline {
public:
	float start_time;
	float end_time;
	float duration;
	Vec2<float> t0_coefficient; // coefficient of t^0
	Vec2<float> t1_coefficient; // coefficient of t^1
	Vec2<float> t2_coefficient; // coefficient of t^2
	Vec2<float> t3_coefficient; // coefficient of t^3

	CubicSpline(const float start_time, const float end_time, const Vec2<float> t0_coefficient, const Vec2<float> t1_coefficient, const Vec2<float> t2_coefficient, const Vec2<float> t3_coefficient):
	start_time(start_time),
	end_time(end_time),
	duration(end_time - start_time),
	// t0_coefficient(t0_coefficient),
	// t1_coefficient(t1_coefficient),
	// t2_coefficient(t2_coefficient),
	// t3_coefficient(t3_coefficient)
	t0_coefficient(t0_coefficient),
	t1_coefficient(t1_coefficient / duration),
	t2_coefficient(t2_coefficient / (duration * duration)),
	t3_coefficient(t3_coefficient / (duration * duration * duration))
	{	}

	Vec2<float> get_position_at(float time) {
		time -= this->start_time;
		// time /= this->duration;
		return this->t0_coefficient + this->t1_coefficient * time + this->t2_coefficient * time * time + this->t3_coefficient * time * time * time;
	}

	Vec2<float> get_velocity_at(float time) {
		time -= this->start_time;
		// time /= this->duration;
		return this->t1_coefficient + this->t2_coefficient * time * 2 + this->t3_coefficient * time * time * 3;
	}

	Vec2<float> get_acceleration_at(float time) {
		time -= this->start_time;
		// time /= this->duration;
		return this->t2_coefficient * 2 + this->t3_coefficient * time * 6;
	}
};

static CubicSpline hermite_factory(const float start_time, const float end_time, const Vec2<float> start_point, Vec2<float> start_velocity, const Vec2<float> end_point, Vec2<float> end_velocity) {
	start_velocity *= end_time - start_time;
	end_velocity *= end_time - start_time;
	return CubicSpline(start_time, end_time, start_point, start_velocity, start_point * -3 + start_velocity * -2 + end_point * 3 + end_velocity * -1, start_point * 2 + start_velocity + end_point * -2 + end_velocity);
}

static UniformCubicSpline uniform_hermite_factory(const Vec2<float> start_point, const Vec2<float> start_velocity, const Vec2<float> end_point, const Vec2<float> end_velocity) {
	return UniformCubicSpline(start_point, start_velocity, start_point * -3 + start_velocity * -2 + end_point * 3 + end_velocity * -1, start_point * 2 + start_velocity + end_point * -2 + end_velocity);
}

static UniformCubicSpline uniform_bezier_factory(const Vec2<float> start_point, const Vec2<float> control_point1, const Vec2<float> control_point2, const Vec2<float> end_point) {
	return UniformCubicSpline(start_point, start_point * -3 + control_point1 * 3, start_point * 3 + control_point1 * -6 + control_point2 * 3, start_point * -1 + control_point1 * 3 + control_point2 * -3 + end_point);
}

static UniformCubicSpline uniform_cardinal_factory(const float scale, const Vec2<float> point1, const Vec2<float> point2, const Vec2<float> point3, const Vec2<float> point4) {
	return UniformCubicSpline(point2, point1 + point3 * scale, point1 * 2 * scale + point2 * (scale-3) + point3 * (3 - 2 * scale) + point4 * -scale, point1 * -scale + point2 * (2 - scale) + point3 * (scale - 2) + point4 * scale);
}

static UniformCubicSpline uniform_catmull_rom_factory(const Vec2<float> point1, const Vec2<float> point2, const Vec2<float> point3, const Vec2<float> point4) {
	return uniform_cardinal_factory(0.5f, point1, point2, point3, point4);
}

// static CubicSpline b_spline_factory(const Vec2<float> point1, const Vec2<float> point2, const Vec2<float> point3, const Vec2<float> point4) {
// 	return CubicSpline();
// }


static UniformCubicSpline uniform_hermite_to_uniform_bezier(const UniformCubicSpline& hermite) {
	return UniformCubicSpline(hermite.t0_coefficient, hermite.t0_coefficient + hermite.t1_coefficient / 3.0f, hermite.t2_coefficient + hermite.t3_coefficient / 3.0f, hermite.t2_coefficient);
}


class Hermite: public UniformCubicSpline {
public:
	Hermite(const Vec2<float> start_point, const Vec2<float> start_velocity, const Vec2<float> end_point, const Vec2<float> end_velocity):
		UniformCubicSpline(start_point,
	            	start_velocity,
	            	start_point * -3 + start_velocity * -2 + end_point * 3 + end_velocity * -1,
	            	start_point * 2 + start_velocity + end_point * -2 + end_velocity) {

	}
	
};

class Point {
public:
	Vec2<float> position;
	Vec2<float> velocity;
};

class Spline {
public:
	float total_duration;
	std::vector<CubicSpline> splines;

	Spline() {	}

	void add_spline(const CubicSpline spline) {
		this->splines.push_back(spline);
		this->total_duration += spline.duration;
	}

	Vec2<float> get_position_at(const float time) {
		for (CubicSpline spline: splines) {
			if (spline.start_time <= time and spline.end_time >= time) {
				return spline.get_position_at(time);
			}
		}
		printf("%f\n", time);
		throw std::runtime_error("Time > Max");
	}

	Vec2<float> get_velocity_at(const float time) {
		for (CubicSpline spline: splines) {
			if (spline.start_time <= time and spline.end_time >= time) {
				return spline.get_velocity_at(time);
			}
		}
		throw std::runtime_error("Time > Max");
	}


	Vec2<float> get_acceleration_at(const float time) {
		for (CubicSpline spline: splines) {
			if (spline.start_time <= time and spline.end_time >= time) {
				return spline.get_acceleration_at(time);
			}
		}
		throw std::runtime_error("Time > Max");
	}
};

class HermiteSpline {
protected:
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
		if (index < this->points.size()) {
			Hermite temp_hermite = Hermite(new_position, new_velocity, this->splines[index].get_position_at(1), this->splines[index].get_velocity_at(1));
			this->splines[index].t0_coefficient = temp_hermite.t0_coefficient;
			this->splines[index].t1_coefficient = temp_hermite.t1_coefficient;
			this->splines[index].t2_coefficient = temp_hermite.t2_coefficient;
			this->splines[index].t3_coefficient = temp_hermite.t3_coefficient;
		}
	}

	unsigned int get_length() const {
		return this->points.size();
	}

	float get_duration() const {
		return this->duration;
	}

	void set_duration(const float _duration) {
		this->duration = _duration;
	}

	float get_time_scale() const {
		return (this->points.size() - 1.0f) / this->duration;
	}

	void add_point(const Vec2<float> position, const Vec2<float> velocity) {
		this->points.push_back({position, velocity});

		if (this->points.size() <= 1) { // cannot add spline with just one point
			return;
		}

		this->splines.push_back(Hermite(this->points[this->points.size() - 2].position, this->points[this->points.size() - 2].velocity, position, velocity));
	}

	float time_to_index(const float time) {
        return (time / this->duration) * (this->points.size() - 1);
    }
    float index_to_time(const float index) {
        return (index / (this->points.size() - 1)) * this->duration;
    }

	Vec2<float> get_position_at(const float time) {
		const float index = this->time_to_index(time);
		int spline_index;
		if (index == this->points.size() - 1) {
			spline_index = this->points.size() - 2;
		} else {
			spline_index = (int) std::floor(index);
		}
		return this->splines[spline_index].get_position_at(index - spline_index);
	}

	Vec2<float> get_velocity_at(const float time) {
		const float index = this->time_to_index(time);
		int spline_index;
		if (index == this->points.size() - 1) {
			spline_index = this->points.size() - 2;
		} else {
			spline_index = (int) std::floor(index);
		}
		return this->splines[spline_index].get_velocity_at(index - spline_index) * this->get_time_scale();
	}

	Vec2<float> get_acceleration_at(const float time) {
		const float index = this->time_to_index(time);
		int spline_index;
		if (index == this->points.size() - 1) {
			spline_index = this->points.size() - 2;
		} else {
			spline_index = (int) std::floor(index);
		}
		return this->splines[spline_index].get_acceleration_at(index - spline_index) * this->get_time_scale();
	}
};

// class Cardinal: public CubicSpline {
// public:
// 	float scale = 0.5f;
// 	Cardinal():
// 	CubicSpline(,
// 	            ,
// 	            ,
// 	            ) {

// 	}
// };
