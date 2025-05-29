#pragma once
#include "Input.h"
#include "Camera.h"
#include "GameObject.h"

#include <memory>
#include <type_traits>
#include <initializer_list>

namespace camera_controller {
    inline void free_camera_movemant(
        Camera& camera, 
        const float& movement_speed, 
        const float& rotation_speed, 
        const float& delta_time) {

        float v = Input::get_axis("Vertical");
        float h = Input::get_axis("Horizontal");

        glm::vec3 movemant =
            camera.get_forward() * v * movement_speed * delta_time
            + camera.get_right() * h * movement_speed * delta_time;

        camera.set_position(
            camera.get_position() + movemant
        );

        float x = 0.0, y = 0.0;
        Input::get_mouse_position(&x, &y);

        float yaw = camera.get_rotation().y + float(-x * rotation_speed * delta_time);
        float pitch = camera.get_rotation().x + float(-y * rotation_speed * delta_time);

        pitch = glm::clamp(pitch, -359.0f, 359.0f);

        camera.set_rotation(glm::vec3(pitch, yaw, 0.0f));
    }

    template<typename... T>
    inline void enter_2d_rendering(T&... objects) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        glMultMatrixf(glm::value_ptr(glm::ortho(0.0f, (float) glutGet(GLUT_WINDOW_WIDTH), 0.0f, (float)glutGet(GLUT_WINDOW_HEIGHT), -1.0f, 1.0f)));

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(GL_COLOR_MATERIAL);
        glDisable(GL_CULL_FACE);

        (
            [&]() {
                auto* tf = objects.get_component<Transform>();
                tf->rotation = glm::quat{ 1,0,0,0 };

                objects.render();
            }(),
        ...);

        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);

        glPopAttrib();
    }

    template<typename... T>
    inline void enter_3d_rendering(Camera& camera, T&... objects) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_CULL_FACE);

        (objects.render(), ...);

        glPopMatrix();

        glPopAttrib();
    }
}