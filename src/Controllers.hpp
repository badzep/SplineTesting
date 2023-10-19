#pragma once


#include "PID.hpp"


constexpr static inline PID follower_angle_controller() { // TODO
	return {
		5.0,
		0,
		0
	};
}

constexpr static inline PID follower_speed_controller() { // TODO
	return {
		0.1,
		0,
		0
	};
}
