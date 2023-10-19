#pragma once


#include <algorithm>
#include <complex>
#include <cstdio>
#include <iostream>
#include <raylib.h>
#include <thread>


#include "Constants.hpp"
#include "Vec.hpp"
#include "Static.hpp"
#include "Spline.hpp"
#include "SplineRender.hpp"


template<const unsigned char DIMENSIONS>
class Field {};

template<> 
class Field<2> {
public:
	Camera2D camera = {};

	Field() {
		camera.zoom = 6.5f;
    	camera.offset = {(float) WINDOW.x / 2.0f, (float) WINDOW.y / 2.0f};
	}

	~Field() {}

	void reset(float& time, Chain<2, float> spline_chain,TestRobot& robot) {
		time = 0;
		robot.position = spline_chain.get_point_at(0);
		robot.velocity = spline_chain.get_tangent_at(0);
	}

	void drag_points(float& time, SplineChain<SplineMethod::HERMITE, 2, float>& spline_chain, TestRobot& robot) {
		Vec2f clicked_position = Vec2f(GetScreenToWorld2D(GetMousePosition(), this->camera)).screen_to_field();

        int duration_index = -1;
        for (unsigned int point_index = 0; point_index < spline_chain.points.size(); point_index++) {
            Vec2f position = spline_chain.points[point_index];
            Vec2f velocity = spline_chain.tangents[point_index];
            if (position.add(velocity * VELOCITY_DISPLAY_MULTIPLIER).get_distance_to(clicked_position - Vec2f(GetMouseDelta()).screen_to_field() / this->camera.zoom) <= GRAB_DISTANCE / this->camera.zoom) {
                spline_chain.tangents[point_index] = (clicked_position - position) / (VELOCITY_DISPLAY_MULTIPLIER);
                spline_chain.build();
                this->reset(time, spline_chain, robot);
                return;
            }

            if (position.get_distance_to(clicked_position - Vec2f(GetMouseDelta()).screen_to_field() / this->camera.zoom) <= GRAB_DISTANCE / this->camera.zoom) {
                spline_chain.points[point_index] = clicked_position;
                spline_chain.build();
                this->reset(time, spline_chain, robot);
                return;
            }

            if (duration_index >= 0) {
                const float angle = velocity.atan2() + PI / 2.0f;
                const Vec2f duration_position = position.add({spline_chain.durations[duration_index] * DURATION_DISPLAY_MULTIPLIER * std::cos(angle), spline_chain.durations[duration_index] * DURATION_DISPLAY_MULTIPLIER * std::sin(angle)});
                const float duration_distance = position.get_distance_to(clicked_position);

                if (duration_position.get_distance_to(clicked_position) <= GRAB_DISTANCE / this->camera.zoom) {
                    spline_chain.durations[duration_index] = duration_distance / DURATION_DISPLAY_MULTIPLIER;
                    spline_chain.build();
                    this->reset(time, spline_chain, robot);
                    return;
                }
            }
            duration_index++;
        }
	}

	void drag_points(float& time, SplineChain<SplineMethod::CARDINAL, 2, float>& spline_chain, TestRobot& robot) {
		Vec2f clicked_position = Vec2f(GetScreenToWorld2D(GetMousePosition(), this->camera)).screen_to_field();

        int duration_index = -1;
        for (unsigned int point_index = 0; point_index < spline_chain.points.size(); point_index++) {
            Vec2f position = spline_chain.points[point_index];
            if (position.get_distance_to(clicked_position - Vec2f(GetMouseDelta()).screen_to_field() / this->camera.zoom) <= GRAB_DISTANCE / this->camera.zoom) {
                spline_chain.points[point_index] = clicked_position;
                spline_chain.build();
                this->reset(time, spline_chain, robot);
                return;
            }

            if (duration_index >= 0) {
                const float angle = 0;
                const Vec2f duration_position = position.add({spline_chain.durations[duration_index] * DURATION_DISPLAY_MULTIPLIER * std::cos(angle), spline_chain.durations[duration_index] * DURATION_DISPLAY_MULTIPLIER * std::sin(angle)});
                const float duration_distance = position.get_distance_to(clicked_position);

                if (duration_position.get_distance_to(clicked_position) <= GRAB_DISTANCE / this->camera.zoom) {
                    spline_chain.durations[duration_index] = duration_distance / DURATION_DISPLAY_MULTIPLIER;
                    spline_chain.build();
                    this->reset(time, spline_chain, robot);
                    return;
                }
            }
            duration_index++;
        }
	}

	void drag_points(float& time, SplineChain<SplineMethod::CATMULL_ROM, 2, float>& spline_chain, TestRobot& robot) {
		Vec2f clicked_position = Vec2f(GetScreenToWorld2D(GetMousePosition(), this->camera)).screen_to_field();

        int duration_index = -1;
        for (unsigned int point_index = 0; point_index < spline_chain.points.size(); point_index++) {
            Vec2f position = spline_chain.points[point_index];

            if (position.get_distance_to(clicked_position - Vec2f(GetMouseDelta()).screen_to_field() / this->camera.zoom) <= GRAB_DISTANCE / this->camera.zoom) {
                spline_chain.points[point_index] = clicked_position;
                spline_chain.build();
                this->reset(time, spline_chain, robot);
                return;
            }

            if (duration_index >= 0) {
                const float angle = 0;
                const Vec2f duration_position = position.add({spline_chain.durations[duration_index] * DURATION_DISPLAY_MULTIPLIER * std::cos(angle), spline_chain.durations[duration_index] * DURATION_DISPLAY_MULTIPLIER * std::sin(angle)});
                const float duration_distance = position.get_distance_to(clicked_position);

                if (duration_position.get_distance_to(clicked_position) <= GRAB_DISTANCE / this->camera.zoom) {
                    spline_chain.durations[duration_index] = duration_distance / DURATION_DISPLAY_MULTIPLIER;
                    spline_chain.build();
                    this->reset(time, spline_chain, robot);
                    return;
                }
            }
            duration_index++;
        }
	}

	template<SplineMethod SPLINE>
	void update(float& time, const bool paused, SplineChain<SPLINE, 2, float>& spline_chain, TestRobot& robot) {
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			this->drag_points(time, spline_chain, robot);
		}
	}

	void render_grid() {
		for (int i = -GRID_SIZE; i <= GRID_SIZE; i++) {
	        if (i % 2 == 0) {
	            DrawLineEx({-(float) GRID_SIZE * 12.0f, (float) i * 12.0f}, {(float)GRID_SIZE * 12.0f, (float) i * 12.0f}, .025f * 12.0f, {255,255,255,200});
	            DrawLineEx({(float) i * 12.0f, -(float) GRID_SIZE * 12.0f}, {(float) i * 12.0f, (float) GRID_SIZE * 12.0f}, .025f * 12.0f, {255,255,255,200});
	        } else {  
	            DrawLineV({-(float) GRID_SIZE * 12.0f, (float) i * 12.0f}, {(float)GRID_SIZE * 12.0f, (float) i * 12.0f}, {255,255,255,100});
	            DrawLineV({(float) i * 12.0f, -(float) GRID_SIZE * 12.0f}, {(float) i * 12.0f, (float) GRID_SIZE * 12.0f}, {255,255,255,100});
	        }
	    }

	    Vec2f start = {-GRID_SIZE * 12.0f, -GRID_SIZE * 12.0f};
	    Vec2f x_up = start + Vec2f{1 * 12.0f,0};
	    Vec2f y_up = start + Vec2f{0,1 * 12.0f};
	    DrawLineField(start, x_up, 0.025f * 12.0f, RED);
	    DrawLineField(start, y_up, 0.025f * 12.0f, GREEN);
	}

	void render_field() {
		// loading zones
	    DrawLineField({-6 * 12.0f,-4 * 12.0f}, {-4 * 12.0f,-6 * 12.0f}, TWO_INCHES, BLUE);
	    DrawLineField({6 * 12.0f,-4 * 12.0f}, {4 * 12.0f,-6 * 12.0f}, TWO_INCHES, BLUE);

	    DrawLineField({-6 * 12.0f,4 * 12.0f}, {-4 * 12.0f,6 * 12.0f}, TWO_INCHES, RED);
	    DrawLineField({6 * 12.0f,4 * 12.0f}, {4 * 12.0f,6 * 12.0f}, TWO_INCHES, RED);

	    // center black I thing
	    DrawLineField({4 * 12.0f,-0 * 12.0f}, {-4 * 12.0f,0 * 12.0f}, TWO_INCHES, BLACK);
	    DrawLineField({4 * 12.0f,2 * 12.0f}, {4 * 12.0f,-2 * 12.0f}, TWO_INCHES, BLACK);
	    DrawLineField({-4 * 12.0f,2 * 12.0f}, {-4 * 12.0f,-2 * 12.0f}, TWO_INCHES, BLACK);
	    

	    // goal net things
	    DrawCircleField({2 * 12.0f,4 * 12.0f}, mm_to_feet(100.0) * 12.0f, BLUE);
	    DrawCircleField({-2 * 12.0f,4 * 12.0f}, mm_to_feet(100.0) * 12.0f, BLUE);
	    for (unsigned char net_x_index = 0; net_x_index < NET_SIZE.x; net_x_index++) {
			const float x_position = -2 * 12.0f + (NET_SPACING_X * 12.0f * (net_x_index + 1));
			DrawLineField({x_position, 4 * 12.0f}, {x_position, 6 * 12.0f}, 0.01f * 12.0f, BLUE);
		}
		for (unsigned char net_y_index = 0; net_y_index < NET_SIZE.y; net_y_index++) {
			const float y_position = 6 * 12.0f - NET_SPACING_X * 12.0f * (net_y_index + 1);
			DrawLineField({-2 * 12.0f, y_position}, {2 * 12.0f, y_position}, 0.01f * 12.0f, BLUE);
		}
	    DrawLineField({2 * 12.0f,4 * 12.0f}, {-2 * 12.0f,4 * 12.0f}, ONE_INCH, BLACK);
	    DrawLineField({2 * 12.0f,6 * 12.0f}, {-2 * 12.0f,6 * 12.0f}, ONE_INCH, BLACK);
	    DrawLineField({2 * 12.0f,4 * 12.0f}, {2 * 12.0f,6 * 12.0f}, ONE_INCH, BLACK);
	    DrawLineField({-2 * 12.0f,4 * 12.0f}, {-2 * 12.0f,6 * 12.0f}, ONE_INCH, BLACK);

	    DrawCircleField({2 * 12.0f,-4 * 12.0f}, mm_to_feet(100.0) * 12.0f, RED);
	    DrawCircleField({-2 * 12.0f,-4 * 12.0f}, mm_to_feet(100.0) * 12.0f, RED);
	    for (unsigned char net_x_index = 0; net_x_index < NET_SIZE.x; net_x_index++) {
			const float x_position = 2 * 12.0f - NET_SPACING_X * 12.0f * (net_x_index + 1);
			DrawLineField({x_position, -4 * 12.0f}, {x_position, -6 * 12.0f}, 0.01f * 12.0f, RED);
		}
		for (unsigned char net_y_index = 0; net_y_index < NET_SIZE.y; net_y_index++) {
			const float y_position = -6 * 12.0f + NET_SPACING_X * 12.0f * (net_y_index + 1);
			DrawLineField({-2 * 12.0f, y_position}, {2 * 12.0f, y_position}, 0.01f * 12.0f, RED);
		}
	    DrawLineField({2 * 12.0f,-4 * 12.0f}, {-2 * 12.0f,-4 * 12.0f}, ONE_INCH, BLACK);
	    DrawLineField({2 * 12.0f,-6 * 12.0f}, {-2 * 12.0f,-6 * 12.0f}, ONE_INCH, BLACK);
	    DrawLineField({2 * 12.0f,-4 * 12.0f}, {2 * 12.0f,-6 * 12.0f}, ONE_INCH, BLACK);
	    DrawLineField({-2 * 12.0f,-4 * 12.0f}, {-2 * 12.0f,-6 * 12.0f}, ONE_INCH, BLACK);
	}

	void render_points(const SplineChain<SplineMethod::HERMITE, 2, float>& spline_chain) {
		int duration_index = -1;
        for (unsigned int point_index = 0; point_index < spline_chain.points.size(); point_index++) {
            Vec2f position = spline_chain.points[point_index];
            Vec2f velocity = spline_chain.tangents[point_index];
            const float size = .2f * 12.0f;
            const float spacing = .01f * 12.0f;

            if (duration_index >= 0) {
                DrawTextField(std::format("Travel time: {0:.2f} sec", spline_chain.durations[duration_index]).c_str(), position.add({-0.6f * 12.0f, -0.1f * 12.0f}), size, spacing, BLACK);
                const float angle = velocity.atan2() + PI / 2.0f;
                const Vec2f duration_position = position.add({spline_chain.durations[duration_index] * DURATION_DISPLAY_MULTIPLIER * std::cos(angle), spline_chain.durations[duration_index] * DURATION_DISPLAY_MULTIPLIER * std::sin(angle)});
                DrawCircleField(duration_position, NODE_SIZE, PURPLE);
                DrawLineField(position, duration_position, 0.015f * 12.0f, PURPLE);
            }
            duration_index++;

            DrawTextField(std::format("Vel: {0:.2f} in/s", velocity.magnitude()).c_str(), position.add({-0.2f * 12.0f, -0.1f * 12.0f}), size, spacing, BLACK);
            DrawTextField(velocity.to_string_2f().c_str(), position.add({-0.4f * 12.0f, -0.1f * 12.0f}), size, spacing, BLACK);

            Vec2f velocity_position = position.add(velocity * VELOCITY_DISPLAY_MULTIPLIER);
            DrawLineField(position, velocity_position, 0.015f * 12.0f, {10, 200, 200, 240});
            DrawCircleField(velocity_position, NODE_SIZE, {10, 200, 200, 240});
            DrawCircleField(position, NODE_SIZE, PATH_COLOR);
            DrawTextField(("Pos: " + position.to_string_2f()).c_str(), position.add({0.3f * 12.0f, -0.1f * 12.0f}), size, spacing, BLACK);
        }
	}

	void render_points(const SplineChain<SplineMethod::CARDINAL, 2, float>& spline_chain) {

	}

	void render_points(const SplineChain<SplineMethod::CATMULL_ROM, 2, float>& spline_chain) {

	}

	template<SplineMethod SPLINE>
	void render(const float time, const SplineChain<SPLINE, 2, float>& spline_chain, const TestRobot& robot) {
		BeginDrawing();
	        BeginMode2D(camera);
	        	ClearBackground({100,100,100,255});
				this->render_grid();
				this->render_field();
				this->render_points(spline_chain);

				const Vec2f position = spline_chain.get_point_at(time);
		        const Vec2f velocity = spline_chain.get_tangent_at(time);
		        const Vec2f acceleration = spline_chain.get_tangent_slope_at(time);

		        draw_spline_chain<2>(spline_chain, robot.max_velocity, robot.max_acceleration);

		        DrawRectangleField(position, {robot.size.x, robot.size.y}, (velocity.atan2() / PI) * 180.0f + 90.0f, ROBOT_COLOR);
		        DrawLineField(position, position.add(velocity.multiply(VELOCITY_DISPLAY_MULTIPLIER)), 0.025f * 12.0f, BLUE);
		        DrawLineField(position.add(velocity * VELOCITY_DISPLAY_MULTIPLIER), position.add(velocity  * VELOCITY_DISPLAY_MULTIPLIER).add(acceleration * ACCELERATION_DISPLAY_MULTIPLIER), 0.025 * 12.0f, GREEN);
			EndMode2D();

			DrawFPS(WINDOW.x * 0.9f, WINDOW.y * 0.05f);
	        DrawText(std::format("Time {0:.2f} / {1:.2f}", time, spline_chain.total_duration).c_str(), 25, 20, 20, BLACK);
	        DrawText(std::format("Velocity {0:.2f} / {1:.2f}", velocity.magnitude(), robot.max_velocity).c_str(), 25, 40, 20, BLACK);
	        DrawText(std::format("Acceleration {0:.2f} / {1:.2f}", acceleration.magnitude(), robot.max_acceleration).c_str(), 25, 60, 20, BLACK);
		EndDrawing();
	}
};



template<> 
class Field<3> {
public:
	Camera3D camera = {};
	Model field_model;
	Model robot_model;
	Model thick_pipe;
	Model thin_pipe;
	Model cone_thing;
	SplinePolynomial<2, float> test_spline = {0,1,{},{},{},{}};

	Field() {
		camera.projection = CAMERA_PERSPECTIVE;
		camera.fovy = 60;
		camera.up = {0.0f, 0.0f, 1.0f};
		camera.position = {0, 0, 8.0 * 12.0f};
		camera.target = {0.1f, 0, 1.0f};

    	this->field_model = LoadModelFromMesh(GenMeshCube(GRID_SIZE * 2.0f * 12.0f, GRID_SIZE * 2.0f * 12.0f, 1.0f * 12.0f));
		this->robot_model = LoadModelFromMesh(GenMeshCube(ROBOT_SIZE, ROBOT_SIZE, ROBOT_HEIGHT));
		this->thick_pipe = LoadModelFromMesh(GenMeshCylinder(mm_to_feet(30.0f) * 12.0f, 1.0f * 12.0f, 10));
		this->thin_pipe = LoadModelFromMesh(GenMeshCylinder(mm_to_feet(10.5f) * 12.0f, 1.0f * 12.0f, 10));
		this->cone_thing = LoadModelFromMesh(GenMeshCone(mm_to_feet(100.0) * 12.0f, 0.25f * 12.0f, 20));
	}

	~Field() {
		UnloadModel(this->field_model);
		UnloadModel(this->robot_model);
		UnloadModel(this->thick_pipe);
		UnloadModel(this->thin_pipe);
		UnloadModel(this->cone_thing);
	}

	template<SplineMethod SPLINE>
	void update(float& time, const bool paused, SplineChain<SPLINE, 2, float>& spline_chain, TestRobot& robot) {
		if (paused) {
			return;
		}

		const float end_time = spline_chain.get_spline_at(time).end_time;
	    const Vec2f start_point = robot.position;
	    const Vec2f start_tangent = robot.velocity * (end_time - time);
	    const Vec2f end_point = spline_chain.get_point_at(end_time);
	    const Vec2f end_tangent = spline_chain.get_tangent_at(end_time) * (end_time - time);
	    this->test_spline = SplinePolynomial<2, float>::hermite_factory(time, end_time, start_point, start_tangent, end_point, end_tangent);

	    robot.acceleration = this->test_spline.get_tangent_slope_at(time);
	   	robot.acceleration.range_in_place(MAX_ACCELERATION, -MAX_ACCELERATION);
        robot.position += robot.velocity * GetFrameTime() + robot.acceleration * GetFrameTime() * GetFrameTime();
        robot.velocity += robot.acceleration * GetFrameTime();
        robot.velocity.range_in_place(robot.max_velocity, -robot.max_velocity);
	}

	void render_grid() {
		const float GRID_HEIGHT = 0.0f;
		for (int i = -GRID_SIZE; i <= GRID_SIZE; i++) {
	        if (i % 2 == 0) {
	        	DrawLine3D({-(float) GRID_SIZE * 12.0f, (float) i * 12.0f, GRID_HEIGHT * 12.0f}, {(float)GRID_SIZE * 12.0f, (float) i * 12.0f, GRID_HEIGHT * 12.0f}, {255,255,255,200});
	        	DrawLine3D({(float) i * 12.0f, -(float) GRID_SIZE * 12.0f, GRID_HEIGHT * 12.0f}, {(float) i * 12.0f, (float) GRID_SIZE * 12.0f, GRID_HEIGHT * 12.0f}, {255,255,255,200});
	        } else {
	        	DrawLine3D({-(float) GRID_SIZE * 12.0f, (float) i * 12.0f, GRID_HEIGHT * 12.0f}, {(float)GRID_SIZE * 12.0f, (float) i * 12.0f, GRID_HEIGHT * 12.0f}, {255,255,255,50});
	            DrawLine3D({(float) i * 12.0f, -(float) GRID_SIZE * 12.0f, GRID_HEIGHT * 12.0f}, {(float) i * 12.0f, (float) GRID_SIZE * 12.0f, GRID_HEIGHT * 12.0f}, {255,255,255,50});
	        }
	    }

	    Vec3f start = {-GRID_SIZE * 12.0f, -GRID_SIZE * 12.0f, 0.25f * 12.0f};
	    Vec3f x_up = start.add({1 * 12.0f,0,0});
	    Vec3f y_up = start.add({0,1 * 12.0f,0});
	    Vec3f z_up = start.add({0,0,1 * 12.0f});
	    DrawLine3D(start.to_raylib(), x_up.to_raylib(), RED);
	    DrawLine3D(start.to_raylib(), y_up.to_raylib(), GREEN);
	    DrawLine3D(start.to_raylib(), z_up.to_raylib(), BLUE);
	}

	void render_field() {
		DrawModel(this->field_model, {0,0,-0.5f * 12.0f}, 1.0f, {100,100,100,255});
		DrawModelEx(this->thick_pipe, {-4 * 12.0f, -6 * 12.0f, ONE_INCH}, {0,0,1}, 45.0f, {1, Vec2f(2,2).magnitude(), 1}, BLUE);
		DrawModelEx(this->thick_pipe, {6 * 12.0f, -4 * 12.0f, ONE_INCH}, {0,0,1}, 135.0f, {1, Vec2f(2,2).magnitude(), 1}, BLUE);

		DrawModelEx(this->thick_pipe, {6 * 12.0f, 4 * 12.0f, ONE_INCH}, {0,0,1}, 45.0f, {1, Vec2f(2,2).magnitude(), 1}, RED);
		DrawModelEx(this->thick_pipe, {-4 * 12.0f, 6 * 12.0f, ONE_INCH}, {0,0,1}, 135.0f, {1, Vec2f(2,2).magnitude(), 1}, RED);

		DrawModelEx(this->thick_pipe, {4 * 12.0f, 0 * 12.0f, ONE_INCH}, {0,0,1}, 90.0f, {1,8,1}, BLACK);
		DrawModelEx(this->thick_pipe, {4 * 12.0f, -2 * 12.0f, ONE_INCH}, {0,0,1}, 0.0f, {1,4,1}, BLACK);
		DrawModelEx(this->thick_pipe, {-4 * 12.0f, -2 * 12.0f, ONE_INCH}, {0,0,1}, 0.0f, {1,4,1}, BLACK);

		DrawModelEx(this->thick_pipe, {4 * 12.0f, 0 * 12.0f, ONE_INCH}, {1,0,0}, 90.0f, {1,2,1}, BLUE);
		DrawModelEx(this->thick_pipe, {4 * 12.0f, 0 * 12.0f, 2 * 12.0f + ONE_INCH}, {0,0,1}, -90.0f, {1,2,1}, BLUE);

		DrawModelEx(this->thick_pipe, {-4 * 12.0f, 0 * 12.0f, ONE_INCH}, {1,0,0}, 90.0f, {1,2,1}, RED);
		DrawModelEx(this->thick_pipe, {-4 * 12.0f, 0 * 12.0f, 2 * 12.0f + ONE_INCH}, {0,0,1}, 90.0f, {1,2,1}, RED);

		DrawModelEx(this->thin_pipe, {-2 * 12.0f, -4 * 12.0f, 0}, {1,0,0}, 90.0f, {1,0.5f,1}, RED);
		DrawModelEx(this->thin_pipe, {2 * 12.0f, -4 * 12.0f, 0}, {1,0,0}, 90.0f, {1,0.5f,1}, RED);
		DrawModelEx(this->cone_thing, {-2 * 12.0f, -4 * 12.0f, 0}, {1,0,0}, 90.0f, {1,1,1}, RED);
		DrawModelEx(this->cone_thing, {2 * 12.0f, -4 * 12.0f, 0}, {1,0,0}, 90.0f, {1,1,1}, RED);
		DrawModelEx(this->thin_pipe, {-2 * 12.0f, -6 * 12.0f, 0}, {1,0,0}, 90.0f, {1,WALL_HEIGHT / 12.0f,1}, BLACK);
		DrawModelEx(this->thin_pipe, {2 * 12.0f, -6 * 12.0f, 0}, {1,0,0}, 90.0f, {1,WALL_HEIGHT / 12.0f,1}, BLACK);
		DrawModelEx(this->thin_pipe, {-2 * 12.0f, -4 * 12.0f, 0.5f * 12.0f}, {0,0,1}, 180.0f, {1,2,1}, RED);
		DrawModelEx(this->thin_pipe, {2 * 12.0f, -4 * 12.0f, 0.5f * 12.0f}, {0,0,1}, 180.0f, {1,2,1}, RED);
		DrawModelEx(this->thin_pipe, {2 * 12.0f, -4 * 12.0f, 0.5f * 12.0f}, {0,0,1}, 90.0f, {1,4,1}, BLACK);
		DrawModelEx(this->thin_pipe, {2 * 12.0f, -6 * 12.0f, WALL_HEIGHT}, {0,0,1}, 90.0f, {1,4,1}, BLACK);
		DrawModelEx(this->thin_pipe, {-2 * 12.0f, -6 * 12.0f, WALL_HEIGHT}, {1,0,0}, -NET_ANGLE * 180.0f / PI, {1,std::hypot(2.0f, WALL_HEIGHT  / 12.0f -  0.5f),1}, BLACK);
		DrawModelEx(this->thin_pipe, {2 * 12.0f, -6 * 12.0f, WALL_HEIGHT}, {1,0,0}, -NET_ANGLE * 180.0f / PI, {1,std::hypot(2.0f, WALL_HEIGHT / 12.0f - 0.5f),1}, BLACK);
		for (unsigned char net_x_index = 0; net_x_index < NET_SIZE.x; net_x_index++) {
			const float x_position = -2 + (NET_SPACING_X * (net_x_index + 1));
			DrawLine3D({x_position * 12.0f, -4 * 12.0f, 0.5f * 12.0f}, {x_position * 12.0f, -6 * 12.0f, WALL_HEIGHT}, RED);
		}
		for (unsigned char net_y_index = 0; net_y_index < NET_SIZE.y; net_y_index++) {
			const float y_position = -6.0f * 12.0f + (std::cos(-NET_ANGLE) * NET_SPACING_Y) * (net_y_index + 1);
			const float z_position = WALL_HEIGHT + (std::sin(-NET_ANGLE) * NET_SPACING_Y) * (net_y_index + 1);
			DrawLine3D({-2 * 12.0f, y_position, z_position}, {2 * 12.0f, y_position, z_position}, RED);
		}

		DrawModelEx(this->thin_pipe, {-2 * 12.0f, 4 * 12.0f, 0}, {1,0,0}, 90.0f, {1,0.5f,1}, BLUE);
		DrawModelEx(this->thin_pipe, {2 * 12.0f, 4 * 12.0f, 0}, {1,0,0}, 90.0f, {1,0.5f,1}, BLUE);
		DrawModelEx(this->cone_thing, {-2 * 12.0f, 4 * 12.0f, 0}, {1,0,0}, 90.0f, {1,1,1}, BLUE);
		DrawModelEx(this->cone_thing, {2 * 12.0f, 4 * 12.0f, 0}, {1,0,0}, 90.0f, {1,1,1}, BLUE);
		DrawModelEx(this->thin_pipe, {-2 * 12.0f, 6 * 12.0f, 0}, {1,0,0}, 90.0f, {1,WALL_HEIGHT / 12.0f,1}, BLACK);
		DrawModelEx(this->thin_pipe, {2 * 12.0f, 6 * 12.0f, 0}, {1,0,0}, 90.0f, {1,WALL_HEIGHT / 12.0f,1}, BLACK);
		DrawModelEx(this->thin_pipe, {-2 * 12.0f, 4 * 12.0f, 0.5f * 12.0f}, {0,0,1}, 0.0f, {1,2,1}, BLUE);
		DrawModelEx(this->thin_pipe, {2 * 12.0f, 4 * 12.0f, 0.5f * 12.0f}, {0,0,1}, 0.0f, {1,2,1}, BLUE);
		DrawModelEx(this->thin_pipe, {2 * 12.0f, 4 * 12.0f, 0.5f * 12.0f}, {0,0,1}, 90.0f, {1,4,1}, BLACK);
		DrawModelEx(this->thin_pipe, {2 * 12.0f, 6 * 12.0f, WALL_HEIGHT}, {0,0,1}, 90.0f, {1,4,1}, BLACK);
		DrawModelEx(this->thin_pipe, {-2 * 12.0f, 6 * 12.0f, WALL_HEIGHT}, {1,0,0}, NET_ANGLE * 180.0f / PI + 180, {1,std::hypot(2.0f, WALL_HEIGHT / 12.0f - 0.5f),1}, BLACK);
		DrawModelEx(this->thin_pipe, {2 * 12.0f, 6 * 12.0f, WALL_HEIGHT}, {1,0,0}, NET_ANGLE * 180.0f / PI + 180, {1,std::hypot(2.0f, WALL_HEIGHT / 12.0f - 0.5f),1}, BLACK);
		for (unsigned char net_x_index = 0; net_x_index < NET_SIZE.x; net_x_index++) {
			const float x_position = -2 + (NET_SPACING_X * (net_x_index + 1));
			DrawLine3D({x_position * 12.0f, 4 * 12.0f, 0.5f * 12.0f}, {x_position * 12.0f, 6 * 12.0f, WALL_HEIGHT}, BLUE);
		}
		for (unsigned char net_y_index = 0; net_y_index < NET_SIZE.y; net_y_index++) {
			const float y_position = 6.0f * 12.0f - (std::cos(NET_ANGLE) * NET_SPACING_Y) * (net_y_index + 1);
			const float z_position = WALL_HEIGHT - (std::sin(NET_ANGLE) * NET_SPACING_Y) * (net_y_index + 1);
			DrawLine3D({-2 * 12.0f, y_position, z_position}, {2 * 12.0f, y_position, z_position}, BLUE);
		}
	}

	void render_points(const SplineChain<SplineMethod::HERMITE, 2, float>& spline_chain) {
		int duration_index = -1;
        for (unsigned int point_index = 0; point_index < spline_chain.points.size(); point_index++) {
            Vec2f position = spline_chain.points[point_index];
            Vec2f velocity = spline_chain.tangents[point_index];
            const float size = .2f;
            const float spacing = .01f;

            if (duration_index >= 0) {
                const float angle = velocity.atan2() + PI / 2.0f;
                const Vec2f duration_position = position.add({spline_chain.durations[duration_index] * DURATION_DISPLAY_MULTIPLIER * std::cos(angle), spline_chain.durations[duration_index] * DURATION_DISPLAY_MULTIPLIER * std::sin(angle)});
                DrawSphere(duration_position.to_3d(PATH_HEIGHT).to_raylib(), NODE_SIZE, PURPLE);
                DrawLine3D(position.to_3d(PATH_HEIGHT).to_raylib(), duration_position.to_3d(PATH_HEIGHT).to_raylib(), PURPLE);
            }
            duration_index++;

            DrawLine3D(position.to_3d(VELOCITY_NODE_HEIGHT).to_raylib(), spline_chain.points[point_index].add(velocity * VELOCITY_DISPLAY_MULTIPLIER).to_3d(VELOCITY_NODE_HEIGHT).to_raylib(), {10, 200, 200, 240});
            DrawSphere(position.add(velocity * VELOCITY_DISPLAY_MULTIPLIER).to_3d(VELOCITY_NODE_HEIGHT).to_raylib(), NODE_SIZE, {10, 200, 200, 240});
            DrawLine3D(position.to_3d(PATH_HEIGHT).to_raylib(), position.to_3d(VELOCITY_NODE_HEIGHT).to_raylib(), PATH_COLOR);
            DrawSphere(position.to_3d(PATH_HEIGHT).to_raylib(), NODE_SIZE, PATH_COLOR);
        }
	}

	void render_points(const SplineChain<SplineMethod::CARDINAL, 2, float>& spline_chain) {

	}

	void render_points(const SplineChain<SplineMethod::CATMULL_ROM, 2, float>& spline_chain) {

	}

	template<SplineMethod SPLINE>
	void render(const float time, const SplineChain<SPLINE, 2, float>& spline_chain, const TestRobot& robot) {
		BeginDrawing();
	        BeginMode3D(camera);
	        	ClearBackground({50,50,50});
	        	UpdateCamera(&this->camera, camera.projection);

				this->render_grid();
				this->render_field();
				this->render_points(spline_chain);
                draw_spline_polynomial<3>(this->test_spline, robot.max_velocity, robot.max_acceleration);

                DrawModelEx(this->robot_model, robot.position.to_3d(ROBOT_HEIGHT / 2.0f).to_raylib(),{0,0,1}, (robot.velocity.atan2() / PI) * 180, {1,1,1}, ROBOT_COLOR);
                DrawLine3D(robot.position.to_3d(ROBOT_NODE_HEIGHT).to_raylib(), robot.position.add(robot.velocity.multiply(VELOCITY_DISPLAY_MULTIPLIER)).to_3d(ROBOT_NODE_HEIGHT).to_raylib(), BLUE);
                DrawLine3D(robot.position.add(robot.velocity * VELOCITY_DISPLAY_MULTIPLIER).to_3d(ROBOT_NODE_HEIGHT).to_raylib(), robot.position.add(robot.velocity * VELOCITY_DISPLAY_MULTIPLIER).add(robot.acceleration * ACCELERATION_DISPLAY_MULTIPLIER).to_3d(ROBOT_NODE_HEIGHT).to_raylib(), GREEN);

			EndMode3D();

			DrawFPS(WINDOW.x * 0.9f, WINDOW.y * 0.05f);
	        DrawText(std::format("Time {0:.2f} / {1:.2f}", time, spline_chain.total_duration).c_str(), 25, 20, 20, BLACK);
	        DrawText(std::format("Velocity {0:.2f} / {1:.2f}", robot.velocity.magnitude(), robot.max_velocity).c_str(), 25, 40, 20, BLACK);
	        DrawText(std::format("Acceleration {0:.2f} / {1:.2f}", robot.acceleration.magnitude(), robot.max_acceleration).c_str(), 25, 60, 20, BLACK);
		EndDrawing();
	}
};
