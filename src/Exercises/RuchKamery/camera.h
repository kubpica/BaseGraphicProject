#pragma once
#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
public:

    void look_at(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) {
        V_ = glm::lookAt(eye, center, up);
    }

    void perspective(float fov, float aspect, float near, float far) {
        fov_ = fov;
        aspect_ = aspect;
        near_ = near;
        far_ = far;
    }

    void set_aspect(float aspect) {
        aspect_ = aspect;
    }

    glm::mat4 view() const { return V_; }

    glm::mat4 projection() const { return glm::perspective(fov_, aspect_, near_, far_); }

    // W celu p�ynnego przechodzenia od jednego k�nca zakresu do drugiego wykorzystamy funkcj� logistyczn�
    float logistic(float y) {
        return 1.0f / (1.0f + std::exp(-y));
    }

    // i jej odwrotno��:
    float inverse_logistics(float x) {
        return std::log(x / (1.0f - x));
    }

    // Niech y_offset b�dzie warto�ci� otrzyman� poprzez obr�t k�kiem myszy.
    void zoom(float y_offset) {
        // Najpierw obliczamy
        auto x = fov_ / glm::pi<float>();
        // jest to liczba z zakresu [0,1], a nast�pnie liczymy
        auto y = inverse_logistics(x);
        // y jest ju� liczb� z zakresu (-Inf, Inf). Do tej liczby dodajemy nasz y_offset
        y += y_offset;
        // i przekszta�camy z powrotem do przedzia�u (0,1) za pomoc� funkcji logistycznej:
        x = logistic(y);
        // i w k�ncu do przedzia�u (0, pi):
        fov_ = x * glm::pi<float>();


        /*
          Implementacja  ...
        */
    }

private:
    // Pola okre�laj�ce macierz projekcji
    float fov_;
    float aspect_;
    float near_;
    float far_;

    glm::mat4 V_;
};