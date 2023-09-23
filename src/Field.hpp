#pragma once


#include <raylib.h>
#include <thread>


#include "Constants.hpp"
#include "Vector.hpp"
#include "Hermite.hpp"
#include "Static.hpp"



class Field2d {
public:
	Camera2D camera = {};

	explicit Field2d() {
		camera.zoom = 80.0f;
    	camera.offset = {(float) WINDOW.x / 2.0f, (float) WINDOW.y / 2.0f};
	}

	void render() {
		BeginDrawing();
	        BeginMode2D(camera);
				ClearBackground({100,100,100,255});
				this->draw_grid();
				this->draw_field();
				float path_index = 0;
				float path_length = 0;
	            bool does_overspeed = false;
	            bool does_over_acceleration = false;
	            while (true) {
	                Motion path_motion = spline.get_motion_at(path_index);
	                path_motion.velocity *= spline.get_time_scale();
	                path_motion.acceleration *= spline.get_time_scale();
	                path_length += path_motion.position.get_distance_to(spline.get_position_at(path_index + PATH_INDEX_DELTA));
	                bool overspeed = path_motion.velocity.magnitude() > MAX_VELOCITY;
	                if (overspeed) {
	                    does_overspeed = true;
	                }
	                bool over_acceleration = path_motion.acceleration.magnitude() > MAX_ACCELERATION;
	                if (over_acceleration) {
	                    does_over_acceleration = true;
	                }
	                if (overspeed and over_acceleration) {
	                    DrawLineField(path_motion.position, spline.get_position_at(path_index + PATH_INDEX_DELTA), 0.05f, RED);
	                } else if (over_acceleration) {
	                    DrawLineField(path_motion.position, spline.get_position_at(path_index + PATH_INDEX_DELTA), 0.04f, ORANGE);
	                } else if (overspeed) {
	                    DrawLineField(path_motion.position, spline.get_position_at(path_index + PATH_INDEX_DELTA), 0.04f, YELLOW);
	                } else {
	                    DrawLineField(path_motion.position, spline.get_position_at(path_index + PATH_INDEX_DELTA), 0.03f, PATH_COLOR);
	                }

	                if (path_index > spline.get_index_total() - PATH_INDEX_DELTA) {
	                    break;
	                }
	                path_index += PATH_INDEX_DELTA;  
	            }

	            for (Node &spline_point: spline.get_nodes()) {
	                DrawLineField(spline_point.position, spline_point.position.add(spline_point.velocity * spline.get_time_scale() * VELOCITY_DISPLAY_MULTIPLIER), 0.015f, {10, 200, 200, 240});
	                DrawCircleField(spline_point.position.add(spline_point.velocity * spline.get_time_scale() * VELOCITY_DISPLAY_MULTIPLIER), NODE_SIZE, {10, 200, 200, 240});
	                DrawCircleField(spline_point.position, NODE_SIZE, PATH_COLOR);
	            }
	            Motion motion = spline.get_motion_at(current_time * spline.get_time_scale());
	            motion.velocity.multiply_in_place(spline.get_time_scale());
	            motion.acceleration.multiply_in_place(spline.get_time_scale());

	            DrawPolyField(motion.position, 4, ROBOT_SIZE / 2.0f, (motion.velocity.atan2() / PI) * 180 + 45, {200, 10, 200, 175});

	            DrawLineField(motion.position, motion.position.add(motion.velocity.multiply(spline.get_time_scale()).multiply(VELOCITY_DISPLAY_MULTIPLIER)), 0.025f, BLUE);
	            DrawLineField(motion.position.add(motion.velocity * spline.get_time_scale() * VELOCITY_DISPLAY_MULTIPLIER), motion.position.add(motion.velocity * spline.get_time_scale() * VELOCITY_DISPLAY_MULTIPLIER).add(motion.acceleration * spline.get_time_scale() * ACCELERATION_DISPLAY_MULTIPLIER), 0.025, GREEN);

	            for (Node &spline_point: spline.get_nodes()) {  
	                const float size = .2f;
	                const float spacing = .01f;
	                DrawTextField(("Pos: " + spline_point.position.to_string_2f()).c_str(), spline_point.position.add({0.3f, -0.1f}), size, spacing, BLACK);
	                DrawTextField(std::format("Vel: {0:.2f} ft/s", spline_point.velocity.multiply(spline.get_time_scale()).magnitude()).c_str(), spline_point.position.add({-0.2f, -0.1f}), size, spacing, BLACK);
	                DrawTextField(spline_point.velocity.to_string_2f().c_str(), spline_point.position.add({-0.4f, -0.1f}), size, spacing, BLACK);
	            }

			EndMode2D();
			DrawFPS(WINDOW.x * 0.9f, WINDOW.y * 0.05f);
	        DrawText(std::format("Time {0:.2f} / {1:.2f}", current_time, spline.index_to_time(spline.get_index_total())).c_str(), 25, 20, 20, BLACK);
	        DrawText(std::format("Position {0:.2f} / {1:.2f}", current_position, path_length).c_str(), 25, 40, 20, BLACK);
	        DrawText(std::format("Velocity {0:.2f} / {1:.2f}", motion.velocity.magnitude(), MAX_VELOCITY).c_str(), 25, 60, 20, BLACK);
	        DrawText(std::format("Acceleration {0:.2f} / {1:.2f}", motion.acceleration.magnitude(), MAX_ACCELERATION).c_str(), 25, 80, 20, BLACK);

	        if (does_overspeed) {
	            DrawText("Warning: Path Exceeds Speed Constraint", 30, WINDOW.y - 60, 20, YELLOW);
	        }
	        if (does_over_acceleration) {
	            DrawText("Warning: Path Exceeds Acceleration Constraint", 30, WINDOW.y - 40, 20, ORANGE);
	        }
		EndDrawing();

		if (!paused) {
	        current_time += GetFrameTime();
	        current_position += motion.velocity.magnitude() * GetFrameTime();

	        if (current_position > path_length) {
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
	    DrawCircleField({2,4}, 5.0f * ONE_INCH, BLUE);
	    DrawCircleField({-2,4}, 5.0f * ONE_INCH, BLUE);
	    DrawLineField({2,4}, {-2,4}, ONE_INCH, BLUE);
	    DrawLineField({2,4}, {2,6}, ONE_INCH, BLUE);
	    DrawLineField({-2,4}, {-2,6}, ONE_INCH, BLUE);

	    DrawCircleField({2,-4}, 5.0f * ONE_INCH, RED);
	    DrawCircleField({-2,-4}, 5.0f * ONE_INCH, RED);
	    DrawLineField({2,-4}, {-2,-4}, ONE_INCH, RED);
	    DrawLineField({2,-4}, {2,-6}, ONE_INCH, RED);
	    DrawLineField({-2,-4}, {-2,-6}, ONE_INCH, RED);
	}

	void drag_points() {
	    Vec2<float> clicked_position = Vec2<float>(GetScreenToWorld2D(GetMousePosition(), this->camera)).screen_to_field();
	    for (Node &spline_point: spline.get_nodes()) {
	        if (spline_point.position.add(spline_point.velocity * spline.get_time_scale() * VELOCITY_DISPLAY_MULTIPLIER).get_distance_to(clicked_position - Vec2<float>(GetMouseDelta()).screen_to_field() / this->camera.zoom) <= GRAB_DISTANCE / this->camera.zoom) {
	            spline_point.velocity = (clicked_position - spline_point.position) / (VELOCITY_DISPLAY_MULTIPLIER * spline.get_time_scale());
	            reset();
	            return;
	        }

	        if (spline_point.position.get_distance_to(clicked_position - Vec2<float>(GetMouseDelta()).screen_to_field() / this->camera.zoom) <= GRAB_DISTANCE / this->camera.zoom) { 
	            spline_point.position.x = clicked_position.x;
	            spline_point.position.y = clicked_position.y;
	            reset();
	            return;
	        }
	    }
	}
};

static void make_light(Shader &lighting_shader, Vector3 position, Color color) {
	static int lights_count = 0;
	int enabledLoc = GetShaderLocation(lighting_shader, std::format("lights[{}].enabled", lights_count).c_str());
    int typeLoc = GetShaderLocation(lighting_shader, std::format("lights[{}].type", lights_count).c_str());
    int positionLoc = GetShaderLocation(lighting_shader, std::format("lights[{}].position", lights_count).c_str());
    int targetLoc = GetShaderLocation(lighting_shader, std::format("lights[{}].target", lights_count).c_str());
    int colorLoc = GetShaderLocation(lighting_shader, std::format("lights[{}].color", lights_count).c_str());

    SetShaderValue(lighting_shader, enabledLoc, &LIGHT_ENABLED, SHADER_UNIFORM_INT);
    SetShaderValue(lighting_shader, typeLoc, &LIGHT_TYPE, SHADER_UNIFORM_INT);
    float _position[3] = {position.x, position.y, position.z};
    SetShaderValue(lighting_shader, positionLoc, _position, SHADER_UNIFORM_VEC3);
    float target[3] = {0,0,0};
    SetShaderValue(lighting_shader, targetLoc, target, SHADER_UNIFORM_VEC3);
    float _color[4] = {(float)color.r/(float)255, (float)color.g/(float)255,
                       (float)color.b/(float)255, (float)color.a/(float)255};
    SetShaderValue(lighting_shader, colorLoc, _color, SHADER_UNIFORM_VEC4);
    lights_count++;
}

class Field3d {
public:
	Camera3D camera = {};
	float fog_density;
	Shader lighting_shader;
	Model field_model;
	Model robot_model;
	Model thick_pipe;
	Model thin_pipe;

	explicit Field3d() {
		camera.position = {0, 0, 1};
		camera.target = {1, 0, 1};
		camera.fovy = 60;
		camera.up = {0.0f, 0.0f, 1.0f};
		camera.projection = CAMERA_PERSPECTIVE;

		this->lighting_shader = this->lighting_shader = LoadShader("../res/lighting.vs", "../res/fog.fs");
		this->lighting_shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(this->lighting_shader, "viewPos");

		int fogDensityLoc = GetShaderLocation(this->lighting_shader, "fog_density");
		this->fog_density = 0.0f;
    	SetShaderValue(this->lighting_shader, fogDensityLoc, &this->fog_density, SHADER_UNIFORM_FLOAT);
    	// make_light(this->lighting_shader, {0, 0, 3}, LIGHT);

    	make_light(this->lighting_shader, {GRID_SIZE / 2.0f, GRID_SIZE / 2.0f, 3}, LIGHT);
    	make_light(this->lighting_shader, {-GRID_SIZE / 2.0f, GRID_SIZE / 2.0f, 3}, LIGHT);
    	make_light(this->lighting_shader, {GRID_SIZE / 2.0f, -GRID_SIZE / 2.0f, 3}, LIGHT);
    	make_light(this->lighting_shader, {-GRID_SIZE / 2.0f, -GRID_SIZE / 2.0f, 3}, LIGHT);


    	this->field_model = LoadModelFromMesh(GenMeshCube(GRID_SIZE * 2.0f, GRID_SIZE * 2.0f, 1.0f));
		this->robot_model = LoadModelFromMesh(GenMeshCube(ROBOT_SIZE, ROBOT_SIZE, ROBOT_HEIGHT));
		this->thick_pipe = LoadModelFromMesh(GenMeshCylinder(ONE_INCH, 1.0f, 10));
		this->thin_pipe = LoadModelFromMesh(GenMeshCylinder(ONE_INCH / 2.0f, 1.0f, 10));

		this->field_model.materials[0].shader = this->lighting_shader;
		this->robot_model.materials[0].shader = this->lighting_shader;
		this->thin_pipe.materials[0].shader = this->lighting_shader;
		this->thick_pipe.materials[0].shader = this->lighting_shader;
	}

	~Field3d() {
		UnloadModel(this->field_model);
		UnloadModel(this->robot_model);
		UnloadModel(this->thick_pipe);
		UnloadModel(this->thin_pipe);
		UnloadShader(this->lighting_shader);
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
	}

	void render() {
		BeginDrawing();
			BeginMode3D(camera);
				UpdateCamera(&this->camera, camera.projection);

				ClearBackground(BLACK);
				
				this->draw_grid();
				this->draw_field();
				float path_index = 0;
				float path_length = 0;
	            bool does_overspeed = false;
	            bool does_over_acceleration = false;
	            while (true) {
	                Motion path_motion = spline.get_motion_at(path_index);
	                path_motion.velocity *= spline.get_time_scale();
	                path_motion.acceleration *= spline.get_time_scale();
	                path_length += path_motion.position.get_distance_to(spline.get_position_at(path_index + PATH_INDEX_DELTA));
	                bool overspeed = path_motion.velocity.magnitude() > MAX_VELOCITY;
	                if (overspeed) {
	                    does_overspeed = true;
	                }
	                bool over_acceleration = path_motion.acceleration.magnitude() > MAX_ACCELERATION;
	                if (over_acceleration) {
	                    does_over_acceleration = true;
	                }
	                if (overspeed and over_acceleration) {
	                    DrawLine3D(path_motion.position.to_3d(PATH_HEIGHT).to_raylib(), spline.get_position_at(path_index + PATH_INDEX_DELTA).to_3d(PATH_HEIGHT).to_raylib(), RED);
	                } else if (over_acceleration) {
	                    DrawLine3D(path_motion.position.to_3d(PATH_HEIGHT).to_raylib(), spline.get_position_at(path_index + PATH_INDEX_DELTA).to_3d(PATH_HEIGHT).to_raylib(), ORANGE);
	                } else if (overspeed) {
	                    DrawLine3D(path_motion.position.to_3d(PATH_HEIGHT).to_raylib(), spline.get_position_at(path_index + PATH_INDEX_DELTA).to_3d(PATH_HEIGHT).to_raylib(), YELLOW);
	                } else {
	                    DrawLine3D(path_motion.position.to_3d(PATH_HEIGHT).to_raylib(), spline.get_position_at(path_index + PATH_INDEX_DELTA).to_3d(PATH_HEIGHT).to_raylib(), PATH_COLOR);
	                }

	                if (path_index > spline.get_index_total() - PATH_INDEX_DELTA) {
	                    break;
	                }
	                path_index += PATH_INDEX_DELTA;  
	            }

	            for (Node &spline_point: spline.get_nodes()) {
	                DrawLine3D(spline_point.position.to_3d(VELOCITY_NODE_HEIGHT).to_raylib(), spline_point.position.add(spline_point.velocity * spline.get_time_scale() * VELOCITY_DISPLAY_MULTIPLIER).to_3d(VELOCITY_NODE_HEIGHT).to_raylib(), {10, 200, 200, 240});
	                DrawSphere(spline_point.position.add(spline_point.velocity * spline.get_time_scale() * VELOCITY_DISPLAY_MULTIPLIER).to_3d(VELOCITY_NODE_HEIGHT).to_raylib(), NODE_SIZE, {10, 200, 200, 240});
	                DrawLine3D(spline_point.position.to_3d(PATH_HEIGHT).to_raylib(), spline_point.position.to_3d(VELOCITY_NODE_HEIGHT).to_raylib(), PATH_COLOR);
	                DrawSphere(spline_point.position.to_3d(PATH_HEIGHT).to_raylib(), NODE_SIZE, PATH_COLOR);
	            }
	            Motion motion = spline.get_motion_at(current_time * spline.get_time_scale());
	            motion.velocity.multiply_in_place(spline.get_time_scale());
	            motion.acceleration.multiply_in_place(spline.get_time_scale());

	            DrawModelEx(this->robot_model, motion.position.to_3d(ROBOT_HEIGHT / 2.0f).to_raylib(),{0,0,1}, (motion.velocity.atan2() / PI) * 180, {1,1,1}, {200, 10, 200, 175});

	            DrawLine3D(motion.position.to_3d(ROBOT_NODE_HEIGHT).to_raylib(), motion.position.add(motion.velocity.multiply(spline.get_time_scale()).multiply(VELOCITY_DISPLAY_MULTIPLIER)).to_3d(ROBOT_NODE_HEIGHT).to_raylib(), BLUE);
	            DrawLine3D(motion.position.add(motion.velocity * spline.get_time_scale() * VELOCITY_DISPLAY_MULTIPLIER).to_3d(ROBOT_NODE_HEIGHT).to_raylib(), motion.position.add(motion.velocity * spline.get_time_scale() * VELOCITY_DISPLAY_MULTIPLIER).add(motion.acceleration * spline.get_time_scale() * ACCELERATION_DISPLAY_MULTIPLIER).to_3d(ROBOT_NODE_HEIGHT).to_raylib(), GREEN);

	            for (Node &spline_point: spline.get_nodes()) {  
	                const float size = .2f;
	                const float spacing = .01f;
	                DrawTextField(("Pos: " + spline_point.position.to_string_2f()).c_str(), spline_point.position.add({0.3f, -0.1f}), size, spacing, BLACK);
	                DrawTextField(std::format("Vel: {0:.2f} ft/s", spline_point.velocity.multiply(spline.get_time_scale()).magnitude()).c_str(), spline_point.position.add({-0.2f, -0.1f}), size, spacing, BLACK);
	                DrawTextField(spline_point.velocity.to_string_2f().c_str(), spline_point.position.add({-0.4f, -0.1f}), size, spacing, BLACK);
	            }

			EndMode3D();
			DrawFPS(WINDOW.x * 0.9f, WINDOW.y * 0.05f);
	        DrawText(std::format("Time {0:.2f} / {1:.2f}", current_time, spline.index_to_time(spline.get_index_total())).c_str(), 25, 20, 20, WHITE);
	        DrawText(std::format("Position {0:.2f} / {1:.2f}", current_position, path_length).c_str(), 25, 40, 20, WHITE);
	        DrawText(std::format("Velocity {0:.2f} / {1:.2f}", motion.velocity.magnitude(), MAX_VELOCITY).c_str(), 25, 60, 20, WHITE);
	        DrawText(std::format("Acceleration {0:.2f} / {1:.2f}", motion.acceleration.magnitude(), MAX_ACCELERATION).c_str(), 25, 80, 20, WHITE);

	        if (does_overspeed) {
	            DrawText("Warning: Path Exceeds Speed Constraint", 30, WINDOW.y - 60, 20, YELLOW);
	        }
	        if (does_over_acceleration) {
	            DrawText("Warning: Path Exceeds Acceleration Constraint", 30, WINDOW.y - 40, 20, ORANGE);
	        }
		EndDrawing();

		if (!paused) {
	        current_time += GetFrameTime();
	        current_position += motion.velocity.magnitude() * GetFrameTime();

	        if (current_position > path_length) {
	            reset();
	        }
    	}
	}
};
