//
// Created by pbialas on 05.08.2020.
//


#pragma once

#include <vector>


#include "Application/application.h"
#include "Application/utils.h"

#include "glad/glad.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/gtc/matrix_transform.hpp>
#include <Exercises/Zoom/camera.h>

class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, int major = 4, int minor = 1) :
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

private:
    GLuint vao_;

    // Pola okreœlaj¹ce macierz projekcji
    /*float fov_;
    float aspect_;
    float near_;
    float far_;

    glm::mat4 P_;
    glm::mat4 V_;*/

    // Zmienna zawieraj¹ca nazwê bufora uniform odpowiedzialnego za przesy³anie macierzy PVM do szadera
    GLuint u_pvm_buffer_;

    Camera* camera_;
};