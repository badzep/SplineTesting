#pragma once


#include <algorithm>
#include <complex>
#include <iostream>
#include <raylib.h>
#include <thread>


#include "Constants.hpp"
#include "Vector.hpp"
#include "Static.hpp"
#include "Splines.hpp"


class Field2d {
public:
	Camera2D camera = {};

	Field2d() {
		camera.zoom = 80.0f;
    	camera.offset = {(float) WINDOW.x / 2.0f, (float) WINDOW.y / 2.0f};
	}

	void render() {
		BeginDrawing();
	        BeginMode2D(camera);
				ClearBackground({100,100,100,255});
				this->draw_grid();
				this->draw_field();
				float path_time = 0;
				float path_length = 0;
	            bool does_overspeed = false;
	            bool does_over_acceleration = false;
	            while (true) {
	                Vec2<float> position = spline.get_position_at(path_time);
	                Vec2<float> velocity = spline.get_velocity_at(path_time);
	                Vec2<float> acceleration = spline.get_acceleration_at(path_time);
	                path_length += position.get_distance_to(spline.get_position_at(path_time + PATH_INDEX_DELTA));
	                bool overspeed = velocity.magnitude() > MAX_VELOCITY;
	                if (overspeed) {
	                    does_overspeed = true;
	                }
	                bool over_acceleration = acceleration.magnitude() > MAX_ACCELERATION;
	                if (over_acceleration) {
	                    does_over_acceleration = true;
	                }
	                if (overspeed and over_acceleration) {
	                    DrawLineField(position, spline.get_position_at(path_time + PATH_INDEX_DELTA), 0.05f, RED);
	                } else if (over_acceleration) {
	                    DrawLineField(position, spline.get_position_at(path_time + PATH_INDEX_DELTA), 0.04f, ORANGE);
	                } else if (overspeed) {
	                    DrawLineField(position, spline.get_position_at(path_time + PATH_INDEX_DELTA), 0.04f, YELLOW);
	                } else {
	                    DrawLineField(position, spline.get_position_at(path_time + PATH_INDEX_DELTA), 0.03f, PATH_COLOR);
	                }

	                path_time += PATH_INDEX_DELTA; 
	                if (path_time + PATH_INDEX_DELTA >= spline.total_duration) {
	                    break;
	                }
	            }
	            
	            int index = -1;
	            for (Point &point: builder.points) {
	            	const float size = .2f;
	                const float spacing = .01f;

	                if (index >=0) {
	                	DrawTextField(std::format("Travel time: {0:.2f} sec", builder.durations[index]).c_str(), point.position.add({-0.6f, -0.1f}), size, spacing, BLACK);
	                	const float angle = point.velocity.atan2() + PI / 2.0f;
	                	const Vec2<float> duration_position = point.position.add({builder.durations[index] * DURATION_DISPLAY_MULTIPLIER * std::cos(angle), builder.durations[index] * DURATION_DISPLAY_MULTIPLIER * std::sin(angle)});
	                	DrawCircleField(duration_position, NODE_SIZE, PURPLE);
	                	DrawLineField(point.position, duration_position, 0.015f, PURPLE);
	                }
	                index++;

	                DrawTextField(("Pos: " + point.position.to_string_2f()).c_str(), point.position.add({0.3f, -0.1f}), size, spacing, BLACK);
	                DrawTextField(std::format("Vel: {0:.2f} ft/s", point.velocity.magnitude()).c_str(), point.position.add({-0.2f, -0.1f}), size, spacing, BLACK);
	                DrawTextField(point.velocity.to_string_2f().c_str(), point.position.add({-0.4f, -0.1f}), size, spacing, BLACK);

	                Vec2<float> velocity_position = point.position.add(point.velocity * VELOCITY_DISPLAY_MULTIPLIER);
	                DrawLineField(point.position, velocity_position, 0.015f, {10, 200, 200, 240});
	                DrawCircleField(velocity_position, NODE_SIZE, {10, 200, 200, 240});
	                DrawCircleField(point.position, NODE_SIZE, PATH_COLOR);
	            }
	          
		        Vec2<float> position = spline.get_position_at(current_time);
	            Vec2<float> velocity = spline.get_velocity_at(current_time);
	            Vec2<float> acceleration = spline.get_acceleration_at(current_time);

	            DrawPolyField(position, 4, ROBOT_SIZE / 2.0f, (velocity.atan2() / PI) * 180 + 45, {200, 10, 200, 175});
	            DrawLineField(position, position.add(velocity.multiply(VELOCITY_DISPLAY_MULTIPLIER)), 0.025f, BLUE);
	            DrawLineField(position.add(velocity * VELOCITY_DISPLAY_MULTIPLIER), position.add(velocity  * VELOCITY_DISPLAY_MULTIPLIER).add(acceleration * ACCELERATION_DISPLAY_MULTIPLIER), 0.025, GREEN);
	            

			EndMode2D();
			DrawFPS(WINDOW.x * 0.9f, WINDOW.y * 0.05f);
	        DrawText(std::format("Time {0:.2f} / {1:.2f}", current_time, spline.total_duration).c_str(), 25, 20, 20, BLACK);
	        DrawText(std::format("Position {0:.2f} / {1:.2f}", current_position, path_length).c_str(), 25, 40, 20, BLACK);
	        DrawText(std::format("Velocity {0:.2f} / {1:.2f}", velocity.magnitude(), MAX_VELOCITY).c_str(), 25, 60, 20, BLACK);
	        DrawText(std::format("Acceleration {0:.2f} / {1:.2f}", acceleration.magnitude(), MAX_ACCELERATION).c_str(), 25, 80, 20, BLACK);

	        if (does_overspeed) {
	            DrawText("Warning: Path Exceeds Speed Constraint", 30, WINDOW.y - 60, 20, YELLOW);
	        }
	        if (does_over_acceleration) {
	            DrawText("Warning: Path Exceeds Acceleration Constraint", 30, WINDOW.y - 40, 20, ORANGE);
	        }
	        
		EndDrawing();

		if (!paused) {
	        current_time += GetFrameTime();
	        current_position += velocity.magnitude() * GetFrameTime();

	        if (current_time > spline.total_duration) {
	            reset();
	        }
    	}
	}

	void draw_grid() {
	    for (int i = -GRID_SIZE; i <= GRID_SIZE; i++) {
	        if (i % 2 == 0) {
	            DrawLineEx({-(float) GRID_SIZE, (float) i}, {(float)GRID_SIZE, (float) i}, .025f, {255,255,255,200});
	            DrawLineEx({(float) i, -(float) GRID_SIZE}, {(float) i, (float) GRID_SIZE}, .025f, {255,255,255,200});
	        } else {  
	            DrawLineV({-(float) GRID_SIZE, (float) i}, {(float)GRID_SIZE, (float) i}, {255,255,255,100});
	            DrawLineV({(float) i, -(float) GRID_SIZE}, {(float) i, (float) GRID_SIZE}, {255,255,255,100});
	        }
	    }

	    Vec2<float> start = {-GRID_SIZE, -GRID_SIZE};
	    Vec2<float> x_up = start + Vec2<float>{1,0};
	    Vec2<float> y_up = start + Vec2<float>{0,1};
	    DrawLineField(start, x_up, 0.025f, RED);
	    DrawLineField(start, y_up, 0.025f, GREEN);
	}

	void draw_field() {
	    // loading zones
	    DrawLineField({-6,-4}, {-4,-6}, TWO_INCHES, BLUE);
	    DrawLineField({6,-4}, {4,-6}, TWO_INCHES, BLUE);

	    DrawLineField({-6,4}, {-4,6}, TWO_INCHES, RED);
	    DrawLineField({6,4}, {4,6}, TWO_INCHES, RED);

	    // center black I thing
	    DrawLineField({4,-0}, {-4,0}, TWO_INCHES, BLACK);
	    DrawLineField({4,2}, {4,-2}, TWO_INCHES, BLACK);
	    DrawLineField({-4,2}, {-4,-2}, TWO_INCHES, BLACK);

	    // goal net things
	    DrawCircleField({2,4}, mm_to_feet(100.0), BLUE);
	    DrawCircleField({-2,4}, mm_to_feet(100.0), BLUE);
	    DrawLineField({2,4}, {-2,4}, ONE_INCH, BLUE);
	    DrawLineField({2,4}, {2,6}, ONE_INCH, BLUE);
	    DrawLineField({-2,4}, {-2,6}, ONE_INCH, BLUE);

	    DrawCircleField({2,-4}, mm_to_feet(100.0), RED);
	    DrawCircleField({-2,-4}, mm_to_feet(100.0), RED);
	    DrawLineField({2,-4}, {-2,-4}, ONE_INCH, RED);
	    DrawLineField({2,-4}, {2,-6}, ONE_INCH, RED);
	    DrawLineField({-2,-4}, {-2,-6}, ONE_INCH, RED);
	}

	void drag_points() {
	    Vec2<float> clicked_position = Vec2<float>(GetScreenToWorld2D(GetMousePosition(), this->camera)).screen_to_field();
	    int index = -1;
	    for (Point& point: builder.points) {
	    	if (point.position.add(point.velocity * VELOCITY_DISPLAY_MULTIPLIER).get_distance_to(clicked_position - Vec2<float>(GetMouseDelta()).screen_to_field() / this->camera.zoom) <= GRAB_DISTANCE / this->camera.zoom) {
	            point.velocity = (clicked_position - point.position) / (VELOCITY_DISPLAY_MULTIPLIER);
	        	builder.build(spline);
	            reset();
	        } else if (point.position.get_distance_to(clicked_position - Vec2<float>(GetMouseDelta()).screen_to_field() / this->camera.zoom) <= GRAB_DISTANCE / this->camera.zoom) { 
	        	point.position = clicked_position;
	        	builder.build(spline);
	            reset();
	        } else if (index >= 0) {
	        	const float angle = point.velocity.atan2() + PI / 2.0f;
        		const Vec2<float> duration_position = point.position.add({builder.durations[index] * DURATION_DISPLAY_MULTIPLIER * std::cos(angle), builder.durations[index] * DURATION_DISPLAY_MULTIPLIER * std::sin(angle)});
        		const float duration_distance = point.position.get_distance_to(clicked_position);
        		
        		if (duration_position.get_distance_to(clicked_position) <= GRAB_DISTANCE / this->camera.zoom) {
        			builder.durations[index] = duration_distance / DURATION_DISPLAY_MULTIPLIER;
        			builder.build(spline);
	            	reset();
        		}
	        }
        	index++;

	    }
	}
};






class Field3d {
public:
	Camera3D camera = {};
	Model field_model;
	Model robot_model;
	Model thick_pipe;
	Model thin_pipe;

	explicit Field3d() {
		camera.position = {0, 0, 3};
		camera.target = {1, 0, 1};
		camera.fovy = 60;
		camera.up = {0.0f, 0.0f, 1.0f};
		camera.projection = CAMERA_PERSPECTIVE;

    	this->field_model = LoadModelFromMesh(GenMeshCube(GRID_SIZE * 2.0f, GRID_SIZE * 2.0f, 1.0f));
		this->robot_model = LoadModelFromMesh(GenMeshCube(ROBOT_SIZE, ROBOT_SIZE, ROBOT_HEIGHT));
		this->thick_pipe = LoadModelFromMesh(GenMeshCylinder(mm_to_feet(30.0f), 1.0f, 10));
		this->thin_pipe = LoadModelFromMesh(GenMeshCylinder(mm_to_feet(10.5f), 1.0f, 10));
	}

	~Field3d() {
		UnloadModel(this->field_model);
		UnloadModel(this->robot_model);
		UnloadModel(this->thick_pipe);
		UnloadModel(this->thin_pipe);
	}

	void draw_grid() {
		const float GRID_HEIGHT = 0.0f;
		for (int i = -GRID_SIZE; i <= GRID_SIZE; i++) {
	        if (i % 2 == 0) {
	        	DrawLine3D({-(float) GRID_SIZE, (float) i, GRID_HEIGHT}, {(float)GRID_SIZE, (float) i, GRID_HEIGHT}, {255,255,255,200});
	        	DrawLine3D({(float) i, -(float) GRID_SIZE, GRID_HEIGHT}, {(float) i, (float) GRID_SIZE, GRID_HEIGHT}, {255,255,255,200});
	        } else {
	        	DrawLine3D({-(float) GRID_SIZE, (float) i, GRID_HEIGHT}, {(float)GRID_SIZE, (float) i, GRID_HEIGHT}, {255,255,255,125});
	            DrawLine3D({(float) i, -(float) GRID_SIZE, GRID_HEIGHT}, {(float) i, (float) GRID_SIZE, GRID_HEIGHT}, {255,255,255,125});
	        }
	    }

	    Vec3<float> start = {-GRID_SIZE, -GRID_SIZE, .25};
	    Vec3<float> x_up = start.add({1,0,0});
	    Vec3<float> y_up = start.add({0,1,0});
	    Vec3<float> z_up = start.add({0,0,1});
	    DrawLine3D(start.to_raylib(), x_up.to_raylib(), RED);
	    DrawLine3D(start.to_raylib(), y_up.to_raylib(), GREEN);
	    DrawLine3D(start.to_raylib(), z_up.to_raylib(), BLUE);
	}

	void draw_field() {
		DrawModel(this->field_model, {0,0,-0.5f}, 1.0f, {100,100,100,255});
		DrawModelEx(this->thick_pipe, {-4, -6, ONE_INCH}, {0,0,1}, 45.0f, {1, Vec2<float>(2,2).magnitude(), 1}, BLUE);
		DrawModelEx(this->thick_pipe, {6, -4, ONE_INCH}, {0,0,1}, 135.0f, {1, Vec2<float>(2,2).magnitude(), 1}, BLUE);

		DrawModelEx(this->thick_pipe, {6, 4, ONE_INCH}, {0,0,1}, 45.0f, {1, Vec2<float>(2,2).magnitude(), 1}, RED);
		DrawModelEx(this->thick_pipe, {-4, 6, ONE_INCH}, {0,0,1}, 135.0f, {1, Vec2<float>(2,2).magnitude(), 1}, RED);

		DrawModelEx(this->thick_pipe, {4, 0, ONE_INCH}, {0,0,1}, 90.0f, {1,8,1}, BLACK);
		DrawModelEx(this->thick_pipe, {4, -2, ONE_INCH}, {0,0,1}, 0.0f, {1,4,1}, BLACK);
		DrawModelEx(this->thick_pipe, {-4, -2, ONE_INCH}, {0,0,1}, 0.0f, {1,4,1}, BLACK);

		DrawModelEx(this->thick_pipe, {4, 0, ONE_INCH}, {1,0,0}, 90.0f, {1,2,1}, BLUE);
		DrawModelEx(this->thick_pipe, {4, 0, 2 + ONE_INCH}, {0,0,1}, -90.0f, {1,2,1}, BLUE);

		DrawModelEx(this->thick_pipe, {-4, 0, ONE_INCH}, {1,0,0}, 90.0f, {1,2,1}, RED);
		DrawModelEx(this->thick_pipe, {-4, 0, 2 + ONE_INCH}, {0,0,1}, 90.0f, {1,2,1}, RED);

		DrawModelEx(this->thin_pipe, {-2, -4, 0}, {1,0,0}, 90.0f, {1,0.5f,1}, RED);
		DrawModelEx(this->thin_pipe, {2, -4, 0}, {1,0,0}, 90.0f, {1,0.5f,1}, RED);
		DrawModelEx(this->thin_pipe, {-2, -6, 0}, {1,0,0}, 90.0f, {1,WALL_HEIGHT,1}, BLACK);
		DrawModelEx(this->thin_pipe, {2, -6, 0}, {1,0,0}, 90.0f, {1,WALL_HEIGHT,1}, BLACK);
		DrawModelEx(this->thin_pipe, {-2, -4, 0.5f}, {0,0,1}, 180.0f, {1,2,1}, RED);
		DrawModelEx(this->thin_pipe, {2, -4, 0.5f}, {0,0,1}, 180.0f, {1,2,1}, RED);
		DrawModelEx(this->thin_pipe, {2, -4, 0.5f}, {0,0,1}, 90.0f, {1,4,1}, BLACK);
		DrawModelEx(this->thin_pipe, {2, -6, WALL_HEIGHT}, {0,0,1}, 90.0f, {1,4,1}, BLACK);
		DrawModelEx(this->thin_pipe, {-2, -6, WALL_HEIGHT}, {1,0,0}, -std::atan((WALL_HEIGHT - 0.5f) / 2.0f) * 180.0f / PI, {1,std::hypot(2.0f, WALL_HEIGHT - 0.5f),1}, BLACK);
		DrawModelEx(this->thin_pipe, {2, -6, WALL_HEIGHT}, {1,0,0}, -std::atan((WALL_HEIGHT - 0.5f) / 2.0f) * 180.0f / PI, {1,std::hypot(2.0f, WALL_HEIGHT - 0.5f),1}, BLACK);

		DrawModelEx(this->thin_pipe, {-2, 4, 0}, {1,0,0}, 90.0f, {1,0.5f,1}, BLUE);
		DrawModelEx(this->thin_pipe, {2, 4, 0}, {1,0,0}, 90.0f, {1,0.5f,1}, BLUE);
		DrawModelEx(this->thin_pipe, {-2, 6, 0}, {1,0,0}, 90.0f, {1,WALL_HEIGHT,1}, BLACK);
		DrawModelEx(this->thin_pipe, {2, 6, 0}, {1,0,0}, 90.0f, {1,WALL_HEIGHT,1}, BLACK);
		DrawModelEx(this->thin_pipe, {-2, 4, 0.5f}, {0,0,1}, 0.0f, {1,2,1}, BLUE);
		DrawModelEx(this->thin_pipe, {2, 4, 0.5f}, {0,0,1}, 0.0f, {1,2,1}, BLUE);
		DrawModelEx(this->thin_pipe, {2, 4, 0.5f}, {0,0,1}, 90.0f, {1,4,1}, BLACK);
		DrawModelEx(this->thin_pipe, {2, 6, WALL_HEIGHT}, {0,0,1}, 90.0f, {1,4,1}, BLACK);
		DrawModelEx(this->thin_pipe, {-2, 6, WALL_HEIGHT}, {1,0,0}, std::atan((WALL_HEIGHT - 0.5f) / 2.0f) * 180.0f / PI + 180, {1,std::hypot(2.0f, WALL_HEIGHT - 0.5f),1}, BLACK);
		DrawModelEx(this->thin_pipe, {2, 6, WALL_HEIGHT}, {1,0,0}, std::atan((WALL_HEIGHT - 0.5f) / 2.0f) * 180.0f / PI + 180, {1,std::hypot(2.0f, WALL_HEIGHT - 0.5f),1}, BLACK);
	}

	void render() {
		BeginDrawing();
			BeginMode3D(camera);
				UpdateCamera(&this->camera, camera.projection);

				ClearBackground({50,50,50});
				
				this->draw_grid();
				this->draw_field();
				float path_time = 0;
				float path_length = 0;
	            bool does_overspeed = false;
	            bool does_over_acceleration = false;

	            while (true) {
	                Vec2<float> position = spline.get_position_at(path_time);
	                Vec2<float> velocity = spline.get_velocity_at(path_time);
	                Vec2<float> acceleration = spline.get_acceleration_at(path_time);
	                path_length += position.get_distance_to(spline.get_position_at(path_time + PATH_INDEX_DELTA));
	                bool overspeed = velocity.magnitude() > MAX_VELOCITY;
	                if (overspeed) {
	                    does_overspeed = true;
	                }
	                bool over_acceleration = acceleration.magnitude() > MAX_ACCELERATION;
	                if (over_acceleration) {
	                    does_over_acceleration = true;
	                }
	                if (overspeed and over_acceleration) {
	                    DrawLine3D(position.to_3d(PATH_HEIGHT).to_raylib(), spline.get_position_at(path_time + PATH_INDEX_DELTA).to_3d(PATH_HEIGHT).to_raylib(), RED);
	                } else if (over_acceleration) {
	                    DrawLine3D(position.to_3d(PATH_HEIGHT).to_raylib(), spline.get_position_at(path_time + PATH_INDEX_DELTA).to_3d(PATH_HEIGHT).to_raylib(), ORANGE);
	                } else if (overspeed) {
	                    DrawLine3D(position.to_3d(PATH_HEIGHT).to_raylib(), spline.get_position_at(path_time + PATH_INDEX_DELTA).to_3d(PATH_HEIGHT).to_raylib(), YELLOW);
	                } else {
	                    DrawLine3D(position.to_3d(PATH_HEIGHT).to_raylib(), spline.get_position_at(path_time + PATH_INDEX_DELTA).to_3d(PATH_HEIGHT).to_raylib(), PATH_COLOR);
	                }

	                path_time += PATH_INDEX_DELTA; 
	                if (path_time + PATH_INDEX_DELTA >= spline.total_duration) {
	                    break;
	                } 
	            }

	            for (Point &point: builder.points) {
	            	DrawLine3D(point.position.to_3d(VELOCITY_NODE_HEIGHT).to_raylib(), point.position.add(point.velocity * VELOCITY_DISPLAY_MULTIPLIER).to_3d(VELOCITY_NODE_HEIGHT).to_raylib(), {10, 200, 200, 240});
	            	DrawSphere(point.position.add(point.velocity * VELOCITY_DISPLAY_MULTIPLIER).to_3d(VELOCITY_NODE_HEIGHT).to_raylib(), NODE_SIZE, {10, 200, 200, 240});
	                DrawLine3D(point.position.to_3d(PATH_HEIGHT).to_raylib(), point.position.to_3d(VELOCITY_NODE_HEIGHT).to_raylib(), PATH_COLOR);
	                DrawSphere(point.position.to_3d(PATH_HEIGHT).to_raylib(), NODE_SIZE, PATH_COLOR);
	            }

	            Vec2<float> position = spline.get_position_at(current_time);
                Vec2<float> velocity = spline.get_velocity_at(current_time);
                Vec2<float> acceleration = spline.get_acceleration_at(current_time);

	            DrawModelEx(this->robot_model, position.to_3d(ROBOT_HEIGHT / 2.0f).to_raylib(),{0,0,1}, (velocity.atan2() / PI) * 180, {1,1,1}, {200, 10, 200, 175});

	            DrawLine3D(position.to_3d(ROBOT_NODE_HEIGHT).to_raylib(), position.add(velocity.multiply(VELOCITY_DISPLAY_MULTIPLIER)).to_3d(ROBOT_NODE_HEIGHT).to_raylib(), BLUE);
	            DrawLine3D(position.add(velocity * VELOCITY_DISPLAY_MULTIPLIER).to_3d(ROBOT_NODE_HEIGHT).to_raylib(), position.add(velocity * VELOCITY_DISPLAY_MULTIPLIER).add(acceleration * ACCELERATION_DISPLAY_MULTIPLIER).to_3d(ROBOT_NODE_HEIGHT).to_raylib(), GREEN);

			EndMode3D();
			DrawFPS(WINDOW.x * 0.9f, WINDOW.y * 0.05f);
	        DrawText(std::format("Time {0:.2f} / {1:.2f}", current_time, spline.total_duration).c_str(), 25, 20, 20, WHITE);
	        DrawText(std::format("Position {0:.2f} / {1:.2f}", current_position, path_length).c_str(), 25, 40, 20, WHITE);
	        DrawText(std::format("Velocity {0:.2f} / {1:.2f}", velocity.magnitude(), MAX_VELOCITY).c_str(), 25, 60, 20, WHITE);
	        DrawText(std::format("Acceleration {0:.2f} / {1:.2f}", acceleration.magnitude(), MAX_ACCELERATION).c_str(), 25, 80, 20, WHITE);

	        if (does_overspeed) {
	            DrawText("Warning: Path Exceeds Speed Constraint", 30, WINDOW.y - 60, 20, YELLOW);
	        }
	        if (does_over_acceleration) {
	            DrawText("Warning: Path Exceeds Acceleration Constraint", 30, WINDOW.y - 40, 20, ORANGE);
	        }
		EndDrawing();

		if (!paused) {
	        current_time += GetFrameTime();
	        current_position += velocity.magnitude() * GetFrameTime();

	        if (current_time > spline.total_duration) {
	            reset();
	        }
    	}
	}
};
