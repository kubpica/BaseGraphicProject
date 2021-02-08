//
// Created by pbialas on 05.08.2020.
//


#pragma once

#include <vector>
#include <chrono>

#include "Application/application.h"
#include "Application/utils.h"

#include "glad/glad.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp> // vec3
#include <glm/vec4.hpp> // vec4
#include <glm/gtc/matrix_transform.hpp>
#include <Exercises/PhongDiffuseLightning/camera.h>
#include <Exercises/PhongDiffuseLightning/camera_controler.h>
#include "quad.h"

using namespace std;
using namespace xe;
using namespace glm;

struct Light {
    glm::vec4 position;
    glm::vec4 color;
    glm::vec4 a;
    glm::vec4 ambient;
};

class SimpleShapeApplication : public Application {
public:
    SimpleShapeApplication(int width, int height, string title, int major = 4, int minor = 1) :
            Application(width, height, title, major, minor) {}

    void init() override;;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override;

    void set_camera(Camera* camera) { camera_ = camera; }
    Camera* camera() { return camera_; }
    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
        }
    }

    // nadpisujemy implementacjê metody obs³uguj¹cej kó³ko myszy
    void scroll_callback(double xoffset, double yoffset) override {
        Application::scroll_callback(xoffset, yoffset);
        camera()->zoom(yoffset / 30.0f); // Sta³a przeskalowywuj¹ca 30.0f zosta³a dobrana doœwiadczalnie.
    }

    void set_controler(CameraControler* controler) { controler_ = controler; }

    void mouse_button_callback(int button, int action, int mods) {
        Application::mouse_button_callback(button, action, mods);

        if (controler_) {
            double x, y;
            glfwGetCursorPos(window_, &x, &y);

            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
                controler_->LMB_pressed(x, y);

            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
                controler_->LMB_released(x, y);
        }

    }

    void cursor_position_callback(double x, double y) {
        Application::cursor_position_callback(x, y);
        if (controler_) {
            controler_->mouse_moved(x, y);
        }
    }

    Quad* quad;

    // Zaczniemy od obracania kamer¹ wokó³ jej osi pionowej.
    float rotation_period;
private:
    // ¯eby coœ animowaæ bêdziemy potrzebowali mierzyæ up³yw czasu.
    std::chrono::steady_clock::time_point start_;

    GLuint vao_;

    // Zmienna zawieraj¹ca nazwê bufora uniform odpowiedzialnego za przesy³anie macierzy PVM do szadera
    GLuint u_pvm_buffer_;
    GLuint u_light_buffer;

    Camera* camera_;
    CameraControler* controler_;

    Light light;
};