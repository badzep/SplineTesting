#pragma once


#include <chrono>
#include <iostream>
#include <stdexcept>
#include <vector>


#include "Vec.hpp"
#include "raylib.h"


constexpr float HERMITE_MATRIX[4][4] = {
	// P0, V0, P1, V1
	{1.0f, 0.0f, 0.0f, 0.0f}, // 1
	{0.0f, 1.0f, 0.0f, 0.0f}, // t
	{-3.0f, -2.0f, 3.0f, -1.0f}, // t^2
	{2.0f, 1.0f, -2.0f, 1.0f} // t^3
};

constexpr float BEZIER_MATRIX[4][4] = {
	//	P0, P1, P2, P3
	{1.0f, 0.0f, 0.0f, 0.0f}, // 1
	{-3.0f, 3.0f, 0.0f, 0.0f}, // t
	{-3.0f, -6.0f, 3.0f, 0.0f}, // t^2
	{-1.0f, 3.0f, -3.0f, 1.0f} // t^3
};

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
	static Vec<DIMENSIONS, T> get_point_at(const T start_time, const T end_time, const Vec<DIMENSIONS, T> start_point, const Vec<DIMENSIONS, T> start_tangent, const Vec<DIMENSIONS, T> end_point, const Vec<DIMENSIONS, T> end_tangent) {
   		// TODO
	}
	static Vec<DIMENSIONS, T> get_tangent_at(const T duration, const Vec<DIMENSIONS, T> start_point, const Vec<DIMENSIONS, T> start_tangent, const Vec<DIMENSIONS, T> end_point, const Vec<DIMENSIONS, T> end_tangent) {

	}
	static Vec<DIMENSIONS, T> get_tangent_slope_at(const T duration, const Vec<DIMENSIONS, T> start_point, const Vec<DIMENSIONS, T> start_tangent, const Vec<DIMENSIONS, T> end_point, const Vec<DIMENSIONS, T> end_tangent) {

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

    [[nodiscard]] Vec<DIMENSIONS, T> get_point_at(const float time) const {
        for (SplinePolynomial<DIMENSIONS, T> spline: splines) {
            if (spline.start_time <= time and spline.end_time >= time) {
                return spline.get_point_at(time);
            }
        }
        throw std::runtime_error("Invalid Time\n");
    }

    [[nodiscard]] Vec<DIMENSIONS, T> get_tangent_at(const float time) const {
	 	for (SplinePolynomial<DIMENSIONS, T> spline: splines) {
	 		if (spline.start_time <= time and spline.end_time >= time) {
	 			return spline.get_tangent_at(time);
	 		}
	 	}
	 	throw std::runtime_error("Invalid Time\n");
	 }

    [[nodiscard]] Vec<DIMENSIONS, T> get_tangent_slope_at(const float time) const {
	 	for (SplinePolynomial<DIMENSIONS, T> spline: splines) {
	 		if (spline.start_time <= time and spline.end_time >= time) {
	 			return spline.get_tangent_slope_at(time);
	 		}
	 	}
	 	throw std::runtime_error("Invalid Time\n");
	 }
};

template<const SplineMethod SPLINE, const unsigned char DIMENSIONS, typename T>
class SplineChain;



template<const unsigned char DIMENSIONS, typename T>
class SplineChain<SplineMethod::HERMITE, DIMENSIONS, T>: public Chain<DIMENSIONS, T> {
public:
	std::vector<Vec<DIMENSIONS, T>> points;
	std::vector<Vec<DIMENSIONS, T>> tangents;
    std::vector<T> durations;

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
            this->add_spline(SplinePolynomial<DIMENSIONS, T>(this->total_duration, this->total_duration + this->durations[i], start_point, start_tangent, start_point * -3 + start_tangent * -2 + end_point * 3 + end_tangent * -1, start_point * 2 + start_tangent + end_point * -2 + end_tangent));
		}
	}

	void print_parameters() {
		std::string type_name;
		if (sizeof(T) == 4) {
			type_name = "float";
		} else if (sizeof(T) == 8) {
			type_name = "double";
		} else {
			type_name = "idk man";
		}
		std::cout << "SplineChain<SplineMethod::HERMITE, " << (int) DIMENSIONS << ", " << type_name << "> spline = SplineChain({" << this->points[0].x << ", "<< this->points[0].y << "}, {"<< this->tangents[0].x << ", "<< this->tangents[0].y << "});" << "\n";
        for (unsigned int index = 1; index < this->points.size(); index++) {
        	std::cout << "spline.add_point({" << this->points[index].x << ", "<< this->points[index].y << "}, {"<< this->tangents[index].x << ", "<< this->tangents[index].y << "}, " << this->durations[index - 1] << ");\n";
        }
        std::cout << "spline.build(spline);\n";
	}
};

using Hermite2f = SplineChain<SplineMethod::HERMITE, 2, float>;
using Hermite3f = SplineChain<SplineMethod::HERMITE, 3, float>;

using Hermite2d = SplineChain<SplineMethod::HERMITE, 2, double>;
using Hermite3d = SplineChain<SplineMethod::HERMITE, 3, double>;
