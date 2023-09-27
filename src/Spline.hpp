#pragma once


#include <chrono>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <raylib.h>


#include "Vec.hpp"



enum class SplineMethod {
	HERMITE,
	BEZIER,
	CARDINAL,
	CATMULL_ROM,
	BASIS
};

/**
 * More efficient if only using spline once
 */
template<const SplineMethod SPLINE, const unsigned char DIMENSIONS, typename T>
class BernsteinSpline {

};

template<const unsigned char DIMENSIONS, typename T>
class BernsteinSpline<SplineMethod::HERMITE, DIMENSIONS, T> {
public:
	T start_time;
	T end_time;
	Vec<DIMENSIONS, T> start_point;
	Vec<DIMENSIONS, T> start_tangent;
	Vec<DIMENSIONS, T> end_point;
	Vec<DIMENSIONS, T> end_tangent;

	BernsteinSpline(const T start_time, const T end_time, const Vec<DIMENSIONS, T> start_point, const Vec<DIMENSIONS, T> start_tangent, const Vec<DIMENSIONS, T> end_point, const Vec<DIMENSIONS, T> end_tangent) {}
	static Vec<DIMENSIONS, T> get_point_at(const T time) {
   		
	}
	static Vec<DIMENSIONS, T> get_tangent_at(const T time) {

	}
	static Vec<DIMENSIONS, T> get_tangent_slope_at(const T time) {

	}
};


template<const unsigned char DIMENSIONS, typename T>
class SplinePolynomial {
public:
	T start_time;
	T end_time;
	T duration;
	Vec<DIMENSIONS, T> t0_coefficient; // coefficient of t^0
	Vec<DIMENSIONS, T> t1_coefficient; // coefficient of t^1
	Vec<DIMENSIONS, T> t2_coefficient; // coefficient of t^2
	Vec<DIMENSIONS, T> t3_coefficient; // coefficient of t^3

	SplinePolynomial(const T start_time, const T end_time, const Vec<DIMENSIONS, T> t0_coefficient, const Vec<DIMENSIONS, T> t1_coefficient, const Vec<DIMENSIONS, T> t2_coefficient, const Vec<DIMENSIONS, T> t3_coefficient):
	start_time(start_time),
	end_time(end_time),
	duration(end_time - start_time),
	t0_coefficient(t0_coefficient),
	t1_coefficient(t1_coefficient / duration),
	t2_coefficient(t2_coefficient / (duration * duration)),
	t3_coefficient(t3_coefficient / (duration * duration * duration))
	{	}

	static SplinePolynomial<DIMENSIONS, T> hermite_factory(const T start_time, const T end_time, const Vec<DIMENSIONS, T> start_point, const Vec<DIMENSIONS, T> start_tangent, const Vec<DIMENSIONS, T> end_point, const Vec<DIMENSIONS, T> end_tangent) {
		return SplinePolynomial<DIMENSIONS, T>(start_time, end_time, start_point, start_tangent, start_point * -3 + start_tangent * -2 + end_point * 3 + end_tangent * -1, start_point * 2 + start_tangent + end_point * -2 + end_tangent);
	}

	static SplinePolynomial<DIMENSIONS, T> cardinal_factory(const T start_time, const T end_time, const T scale, const Vec<DIMENSIONS, T> point0, const Vec<DIMENSIONS, T> point1, const Vec<DIMENSIONS, T> point2, const Vec<DIMENSIONS, T> point3) {
		return SplinePolynomial<DIMENSIONS, T>(start_time, end_time, point1, point0 * -scale + point2 * scale, point0 * 2 * scale + point1 * (scale-3) + point2 * (3 - 2 * scale) + point3 * -scale, point0 * -scale + point1 * (2 - scale) + point2 * (scale - 2) + point3 * scale);
	}

	static SplinePolynomial<DIMENSIONS, T> catmull_rom_factory(const T start_time, const T end_time, const Vec<DIMENSIONS, T> point0, const Vec<DIMENSIONS, T> point1, const Vec<DIMENSIONS, T> point2, const Vec<DIMENSIONS, T> point3) {
		return SplinePolynomial<DIMENSIONS, T>(start_time, end_time, point1, point0 * -0.5 + point2 * 0.5, point0 + point1 * -2.5 + point2 * 2 + point3 * -0.5, point0 * -0.5 + point1 * 1.5 + point2 * -1.5 + point3 * 0.5);
	}

    [[nodiscard]] Vec<DIMENSIONS, T> get_point_at(T time) const {
		time -= this->start_time;
		return this->t0_coefficient + this->t1_coefficient * time + this->t2_coefficient * time * time + this->t3_coefficient * time * time * time;
	}

    [[nodiscard]] Vec<DIMENSIONS, T> get_tangent_at(T time) const {
		time -= this->start_time;
		return this->t1_coefficient + this->t2_coefficient * time * 2 + this->t3_coefficient * time * time * 3;
	}

	[[nodiscard]] Vec<DIMENSIONS, T> get_tangent_slope_at(T time) const {
		time -= this->start_time;
		return this->t2_coefficient * 2 + this->t3_coefficient * time * 6;
	}
};



using Spline2f = SplinePolynomial<2, float>;
using Spline2d = SplinePolynomial<2, double>;

using Spline3f = SplinePolynomial<3, float>;
using Spline3d = SplinePolynomial<3, double>;


template<const unsigned char DIMENSIONS, typename T>
class Chain {
public:
    T total_duration = 0;
	std::vector<SplinePolynomial<DIMENSIONS, T>> splines;

    Chain() {   }

	void add_spline(const SplinePolynomial<DIMENSIONS, T> spline) {
		this->splines.push_back(spline);
		this->total_duration += spline.duration;
	}

	[[nodiscard]] SplinePolynomial<DIMENSIONS, T> get_spline_at(const float time) const {
        for (SplinePolynomial<DIMENSIONS, T> spline: splines) {
            if (spline.start_time <= time and spline.end_time >= time) {
                return spline;
            }
        }
        throw std::runtime_error("Invalid Time\n");
    }

    [[nodiscard]] Vec<DIMENSIONS, T> get_point_at(const float time) const {
        return this->get_spline_at(time).get_point_at(time);
    }

    [[nodiscard]] Vec<DIMENSIONS, T> get_tangent_at(const float time) const {
	 	return this->get_spline_at(time).get_tangent_at(time);
	 }

    [[nodiscard]] Vec<DIMENSIONS, T> get_tangent_slope_at(const float time) const {
	 	return this->get_spline_at(time).get_tangent_slope_at(time);
	 }

	virtual void print_parameters() {
		printf("Not yet implemented\n");
	}
};

static std::string get_data_type(const int size) {
    if (size == 4) {
        return "float";
    }

    if (size == 8) {
        return "double";
    }

    return "idk man";
}


template<const SplineMethod SPLINE, const unsigned char DIMENSIONS, typename T>
class SplineChain;

template<const unsigned char DIMENSIONS, typename T>
class SplineChain<SplineMethod::HERMITE, DIMENSIONS, T>: public Chain<DIMENSIONS, T> {
public:
	std::vector<Vec<DIMENSIONS, T>> points;
	std::vector<Vec<DIMENSIONS, T>> tangents;
    std::vector<T> durations;

    SplineChain() {}

    SplineChain(const Vec<DIMENSIONS, T> point, const Vec<DIMENSIONS, T> tangent) {
        this->points.push_back(point);
        this->tangents.push_back(tangent);
    }

	void add_point(const Vec<DIMENSIONS, T> point, const Vec<DIMENSIONS, T> tangent, const T travel_time) {
		this->points.push_back(point);
        this->tangents.push_back(tangent);
		this->durations.push_back(travel_time);
	}

	void build() {
		this->total_duration = 0;
		this->splines.clear();
		for (unsigned int i = 0; i < this->points.size() - 1; i++) {
			const Vec<DIMENSIONS, T> start_point = this->points[i];
			Vec<DIMENSIONS, T> start_tangent = this->tangents[i];
			const Vec<DIMENSIONS, T> end_point = this->points[i + 1];
			Vec<DIMENSIONS, T> end_tangent = this->tangents[i + 1];
			start_tangent *= this->durations[i];
			end_tangent *= this->durations[i];
			this->add_spline(SplinePolynomial<DIMENSIONS, T>::hermite_factory(this->total_duration, this->total_duration + this->durations[i], start_point, start_tangent, end_point, end_tangent));
		}
	}

	void print_parameters() {
		std::cout << "SplineChain<SplineMethod::HERMITE, " << (int) DIMENSIONS << ", " << get_data_type(sizeof(T)) << "> spline = SplineChain<SplineMethod::HERMITE, " << (int) DIMENSIONS << ", " << get_data_type(sizeof(T)) << ">({" << this->points[0].x << ", "<< this->points[0].y << "}, {"<< this->tangents[0].x << ", "<< this->tangents[0].y << "});" << "\n";
        for (unsigned int index = 1; index < this->points.size(); index++) {
        	std::cout << "spline.add_point({" << this->points[index].x << ", "<< this->points[index].y << "}, {"<< this->tangents[index].x << ", "<< this->tangents[index].y << "}, " << this->durations[index - 1] << ");\n";
        }
        std::cout << "spline.build();\n";
	}
};

using Hermite2f = SplineChain<SplineMethod::HERMITE, 2, float>;
using Hermite3f = SplineChain<SplineMethod::HERMITE, 3, float>;

using Hermite2d = SplineChain<SplineMethod::HERMITE, 2, double>;
using Hermite3d = SplineChain<SplineMethod::HERMITE, 3, double>;


template<const unsigned char DIMENSIONS, typename T>
class SplineChain<SplineMethod::CARDINAL, DIMENSIONS, T>: public Chain<DIMENSIONS, T> {
public:
	std::vector<Vec<DIMENSIONS, T>> points;
    std::vector<T> durations;
    std::vector<T> scales;

    SplineChain() {}

    SplineChain(const Vec<DIMENSIONS, T> point) {
        this->points.push_back(point);
    }

	void add_point(const Vec<DIMENSIONS, T> point, const T travel_time, const T scale) {
		this->points.push_back(point);
		this->durations.push_back(travel_time);
        this->scales.push_back(scale);
	}

	void build() {
		this->total_duration = 0;
		this->splines.clear();
		{
			const Vec<DIMENSIONS, T> point0 = this->points[0] - (this->points[1] - this->points[0]);
			const Vec<DIMENSIONS, T> point1 = this->points[0];
			const Vec<DIMENSIONS, T> point2 = this->points[1];
			const Vec<DIMENSIONS, T> point3 = this->points[2];
            this->add_spline(SplinePolynomial<DIMENSIONS, T>::cardinal_factory(this->total_duration, this->total_duration + this->durations[0], this->scales[0], point0, point1, point2, point3));
		}
		for (unsigned int i = 1; i < this->points.size() - 2; i++) {
			const Vec<DIMENSIONS, T> point0 = this->points[i - 1];
			const Vec<DIMENSIONS, T> point1 = this->points[i];
			const Vec<DIMENSIONS, T> point2 = this->points[i + 1];
			const Vec<DIMENSIONS, T> point3 = this->points[i + 2];
            this->add_spline(SplinePolynomial<DIMENSIONS, T>::cardinal_factory(this->total_duration, this->total_duration + this->durations[i], this->scales[i], point0, point1, point2, point3));
		}

		{
			const unsigned int last_index = this->points.size() - 1;
			const Vec<DIMENSIONS, T> point0 = this->points[last_index - 2];
			const Vec<DIMENSIONS, T> point1 = this->points[last_index - 1];
			const Vec<DIMENSIONS, T> point2 = this->points[last_index];
			const Vec<DIMENSIONS, T> point3 = this->points[last_index] - (this->points[last_index] - this->points[last_index - 1]);
            this->add_spline(SplinePolynomial<DIMENSIONS, T>::cardinal_factory(this->total_duration, this->total_duration + this->durations[last_index - 1], this->scales[last_index - 1], point0, point1, point2, point3));
		}
	}

	void print_parameters() {
		std::cout << "SplineChain<SplineMethod::CARDINAL, " << (int) DIMENSIONS << ", " << get_data_type(sizeof(T)) << "> spline = SplineChain<SplineMethod::CARDINAL, " << (int) DIMENSIONS << ", " << get_data_type(sizeof(T)) << ">({" << this->points[0].x << ", "<< this->points[0].y << "});" << "\n";
        for (unsigned int index = 1; index < this->points.size(); index++) {
        	std::cout << "spline.add_point({" << this->points[index].x << ", "<< this->points[index].y << "}, " << this->durations[index - 1] << ", "<< this->scales[index] << ");\n";
        }
        std::cout << "spline.build();\n";
	}
};

using Cardinal2f = SplineChain<SplineMethod::CARDINAL, 2, float>;
using Cardinal3f = SplineChain<SplineMethod::CARDINAL, 3, float>;

using Cardinal2d = SplineChain<SplineMethod::CARDINAL, 2, double>;
using Cardinal3d = SplineChain<SplineMethod::CARDINAL, 3, double>;


template<const unsigned char DIMENSIONS, typename T>
class SplineChain<SplineMethod::CATMULL_ROM, DIMENSIONS, T>: public Chain<DIMENSIONS, T> {
public:
	std::vector<Vec<DIMENSIONS, T>> points;
    std::vector<T> durations;

    SplineChain() {}
    
    SplineChain(const Vec<DIMENSIONS, T> point) {
        this->points.push_back(point);
    }

	void add_point(const Vec<DIMENSIONS, T> point, const T travel_time) {
		this->points.push_back(point);
		this->durations.push_back(travel_time);
	}

	void build() {
		this->total_duration = 0;
		this->splines.clear();
		{
			const Vec<DIMENSIONS, T> point0 = this->points[0] - (this->points[1] - this->points[0]);
			const Vec<DIMENSIONS, T> point1 = this->points[0];
			const Vec<DIMENSIONS, T> point2 = this->points[1];
			const Vec<DIMENSIONS, T> point3 = this->points[2];
            this->add_spline(SplinePolynomial<DIMENSIONS, T>::catmull_rom_factory(this->total_duration, this->total_duration + this->durations[0], point0, point1, point2, point3));
		}
		for (unsigned int i = 1; i < this->points.size() - 2; i++) {
			const Vec<DIMENSIONS, T> point0 = this->points[i - 1];
			const Vec<DIMENSIONS, T> point1 = this->points[i];
			const Vec<DIMENSIONS, T> point2 = this->points[i + 1];
			const Vec<DIMENSIONS, T> point3 = this->points[i + 2];
            this->add_spline(SplinePolynomial<DIMENSIONS, T>::catmull_rom_factory(this->total_duration, this->total_duration + this->durations[i], point0, point1, point2, point3));
		}

		{
			const unsigned int last_index = this->points.size() - 1;
			const Vec<DIMENSIONS, T> point0 = this->points[last_index - 2];
			const Vec<DIMENSIONS, T> point1 = this->points[last_index - 1];
			const Vec<DIMENSIONS, T> point2 = this->points[last_index];
			const Vec<DIMENSIONS, T> point3 = this->points[last_index] - (this->points[last_index] - this->points[last_index - 1]);
            this->add_spline(SplinePolynomial<DIMENSIONS, T>::catmull_rom_factory(this->total_duration, this->total_duration + this->durations[last_index - 1], point0, point1, point2, point3));
		}
	}

	void print_parameters() {
		std::cout << "SplineChain<SplineMethod::CATMULL_ROM, " << (int) DIMENSIONS << ", " << get_data_type(sizeof(T)) << "> spline = SplineChain<SplineMethod::CATMULL_ROM, " << (int) DIMENSIONS << ", " << get_data_type(sizeof(T)) << ">({" << this->points[0].x << ", "<< this->points[0].y << "});" << "\n";
        for (unsigned int index = 1; index < this->points.size(); index++) {
        	std::cout << "spline.add_point({" << this->points[index].x << ", "<< this->points[index].y << "}, " << this->durations[index - 1] << ");\n";
        }
        std::cout << "spline.build();\n";
	}
};

using CatmullRom2f = SplineChain<SplineMethod::CATMULL_ROM, 2, float>;
using CatmullRom3f = SplineChain<SplineMethod::CATMULL_ROM, 3, float>;

using CatmullRom2d = SplineChain<SplineMethod::CATMULL_ROM, 2, double>;
using CatmullRom3d = SplineChain<SplineMethod::CATMULL_ROM, 3, double>;

class TestRobot {
public:
	Vec2f position;
	Vec2f velocity;
	Vec2f acceleration;
	Vec3f size;
	float max_velocity;
	float max_acceleration;

	TestRobot(const float max_velocity, const float max_acceleration): max_velocity(max_velocity), max_acceleration(max_acceleration) {

	}
};
