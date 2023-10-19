#ifndef SPLINETESTING_CAMERA_HPP
#define SPLINETESTING_CAMERA_HPP

#include <raylib.h>
#include <raymath.h>

// this is a modified version of rcamera.h

#define CAMERA_MOVE_SPEED                               0.75f
#define CAMERA_ROTATION_SPEED                           0.03f
#define CAMERA_PAN_SPEED                                0.2f
#define CAMERA_MOUSE_MOVE_SENSITIVITY                   0.003f
#define CAMERA_ORBITAL_SPEED                            0.5f


Vector3 GetCameraForward(Camera *camera) {
    return Vector3Normalize(Vector3Subtract(camera->target, camera->position));
}

Vector3 GetCameraUp(Camera *camera) {
    return Vector3Normalize(camera->up);
}
Vector3 GetCameraRight(Camera *camera) {
    Vector3 forward = GetCameraForward(camera);
    Vector3 up = GetCameraUp(camera);

    return Vector3CrossProduct(forward, up);
}

void CameraMoveForward(Camera *camera, float distance, bool moveInWorldPlane) {
    Vector3 forward = GetCameraForward(camera);

    if (moveInWorldPlane) {
        forward.y = 0;
        forward = Vector3Normalize(forward);
    }

    forward = Vector3Scale(forward, distance);

    camera->position = Vector3Add(camera->position, forward);
    camera->target = Vector3Add(camera->target, forward);
}

void CameraMoveUp(Camera *camera, float distance) {
    Vector3 up = GetCameraUp(camera);

    up = Vector3Scale(up, distance);

    camera->position = Vector3Add(camera->position, up);
    camera->target = Vector3Add(camera->target, up);
}

void CameraMoveRight(Camera *camera, float distance, bool moveInWorldPlane) {
    Vector3 right = GetCameraRight(camera);

    if (moveInWorldPlane) {
        right.y = 0;
        right = Vector3Normalize(right);
    }

    right = Vector3Scale(right, distance);

    camera->position = Vector3Add(camera->position, right);
    camera->target = Vector3Add(camera->target, right);
}

void CameraMoveToTarget(Camera *camera, float delta) {
    float distance = Vector3Distance(camera->position, camera->target);
    distance += delta;
    if (distance <= 0) distance = 0.001f;
    Vector3 forward = GetCameraForward(camera);
    camera->position = Vector3Add(camera->target, Vector3Scale(forward, -distance));
}

void CameraYaw(Camera *camera, float angle, bool rotateAroundTarget) {
    Vector3 up = GetCameraUp(camera);

    Vector3 targetPosition = Vector3Subtract(camera->target, camera->position);

    targetPosition = Vector3RotateByAxisAngle(targetPosition, up, angle);

    if (rotateAroundTarget) {
        camera->position = Vector3Subtract(camera->target, targetPosition);
    }
    else {
        camera->target = Vector3Add(camera->position, targetPosition);
    }
}

void CameraPitch(Camera *camera, float angle, bool lockView, bool rotateAroundTarget, bool rotateUp) {
    Vector3 up = GetCameraUp(camera);

    Vector3 targetPosition = Vector3Subtract(camera->target, camera->position);

    if (lockView) {
        float maxAngleUp = Vector3Angle(up, targetPosition);
        maxAngleUp -= 0.001f;
        if (angle > maxAngleUp) angle = maxAngleUp;

        float maxAngleDown = Vector3Angle(Vector3Negate(up), targetPosition);
        maxAngleDown *= -1.0f;
        maxAngleDown += 0.001f;
        if (angle < maxAngleDown) angle = maxAngleDown;
    }

    Vector3 right = GetCameraRight(camera);

    targetPosition = Vector3RotateByAxisAngle(targetPosition, right, angle);

    if (rotateAroundTarget) {
        camera->position = Vector3Subtract(camera->target, targetPosition);
    }
    else {
        camera->target = Vector3Add(camera->position, targetPosition);
    }

    if (rotateUp) {
        camera->up = Vector3RotateByAxisAngle(camera->up, right, angle);
    }
}

void CameraRoll(Camera *camera, float angle) {
    Vector3 forward = GetCameraForward(camera);

    camera->up = Vector3RotateByAxisAngle(camera->up, forward, angle);
}

void update_camera(Camera *camera, int mode) {
    Vector2 mousePositionDelta = GetMouseDelta();

    bool moveInWorldPlane = ((mode == CAMERA_FIRST_PERSON) || (mode == CAMERA_THIRD_PERSON));
    bool rotateAroundTarget = ((mode == CAMERA_THIRD_PERSON) || (mode == CAMERA_ORBITAL));
    bool lockView = ((mode == CAMERA_FIRST_PERSON) || (mode == CAMERA_THIRD_PERSON) || (mode == CAMERA_ORBITAL));
    bool rotateUp = false;

    if (mode == CAMERA_ORBITAL) {
        Matrix rotation = MatrixRotate(GetCameraUp(camera), CAMERA_ORBITAL_SPEED*GetFrameTime());
        Vector3 view = Vector3Subtract(camera->position, camera->target);
        view = Vector3Transform(view, rotation);
        camera->position = Vector3Add(camera->target, view);
    }
    else {
        if (IsKeyDown(KEY_DOWN)) CameraPitch(camera, -CAMERA_ROTATION_SPEED, lockView, rotateAroundTarget, rotateUp);
        if (IsKeyDown(KEY_UP)) CameraPitch(camera, CAMERA_ROTATION_SPEED, lockView, rotateAroundTarget, rotateUp);
        if (IsKeyDown(KEY_RIGHT)) CameraYaw(camera, -CAMERA_ROTATION_SPEED, rotateAroundTarget);
        if (IsKeyDown(KEY_LEFT)) CameraYaw(camera, CAMERA_ROTATION_SPEED, rotateAroundTarget);
        if (IsKeyDown(KEY_Q)) CameraRoll(camera, -CAMERA_ROTATION_SPEED);
        if (IsKeyDown(KEY_E)) CameraRoll(camera, CAMERA_ROTATION_SPEED);

        if (!IsGamepadAvailable(0)) {
            if ((mode == CAMERA_FREE) && (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))) {
                const Vector2 mouseDelta = GetMouseDelta();
                if (mouseDelta.x > 0.0f) CameraMoveRight(camera, CAMERA_PAN_SPEED, moveInWorldPlane);
                if (mouseDelta.x < 0.0f) CameraMoveRight(camera, -CAMERA_PAN_SPEED, moveInWorldPlane);
                if (mouseDelta.y > 0.0f) CameraMoveUp(camera, -CAMERA_PAN_SPEED);
                if (mouseDelta.y < 0.0f) CameraMoveUp(camera, CAMERA_PAN_SPEED);
            }
            else {
                CameraYaw(camera, -mousePositionDelta.x*CAMERA_MOUSE_MOVE_SENSITIVITY, rotateAroundTarget);
                CameraPitch(camera, -mousePositionDelta.y*CAMERA_MOUSE_MOVE_SENSITIVITY, lockView, rotateAroundTarget, rotateUp);
            }

            if (IsKeyDown(KEY_W)) CameraMoveForward(camera, CAMERA_MOVE_SPEED, moveInWorldPlane);
            if (IsKeyDown(KEY_A)) CameraMoveRight(camera, -CAMERA_MOVE_SPEED, moveInWorldPlane);
            if (IsKeyDown(KEY_S)) CameraMoveForward(camera, -CAMERA_MOVE_SPEED, moveInWorldPlane);
            if (IsKeyDown(KEY_D)) CameraMoveRight(camera, CAMERA_MOVE_SPEED, moveInWorldPlane);
        }
        else {
            CameraYaw(camera, -(GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X) * 2)*CAMERA_MOUSE_MOVE_SENSITIVITY, rotateAroundTarget);
            CameraPitch(camera, -(GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y) * 2)*CAMERA_MOUSE_MOVE_SENSITIVITY, lockView, rotateAroundTarget, rotateUp);

            if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) <= -0.25f) CameraMoveForward(camera, CAMERA_MOVE_SPEED, moveInWorldPlane);
            if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) <= -0.25f) CameraMoveRight(camera, -CAMERA_MOVE_SPEED, moveInWorldPlane);
            if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) >= 0.25f) CameraMoveForward(camera, -CAMERA_MOVE_SPEED, moveInWorldPlane);
            if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) >= 0.25f) CameraMoveRight(camera, CAMERA_MOVE_SPEED, moveInWorldPlane);
        }

        if (mode == CAMERA_FREE) {
            if (IsKeyDown(KEY_SPACE)) CameraMoveUp(camera, CAMERA_MOVE_SPEED);
            if (IsKeyDown(KEY_LEFT_CONTROL)) CameraMoveUp(camera, -CAMERA_MOVE_SPEED);
        }
    }

    if ((mode == CAMERA_THIRD_PERSON) || (mode == CAMERA_ORBITAL) || (mode == CAMERA_FREE)) {
        CameraMoveToTarget(camera, -GetMouseWheelMove());
        if (IsKeyPressed(KEY_KP_SUBTRACT)) CameraMoveToTarget(camera, 2.0f);
        if (IsKeyPressed(KEY_KP_ADD)) CameraMoveToTarget(camera, -2.0f);
    }
}

#endif //SPLINETESTING_CAMERA_HPP
