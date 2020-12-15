#pragma once
#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace std;
using namespace glm;

class Camera {
public:

    void look_at(const vec3& eye, const vec3& center, const vec3& up) {
        V_ = lookAt(eye, center, up);
    }

    void setPerspective(float fov, float aspect, float near, float far) {
        fov_ = fov;
        aspect_ = aspect;
        near_ = near;
        far_ = far;
    }

    void set_aspect(float aspect) {
        aspect_ = aspect;
    }

    mat4 view() const { return V_; }

    mat4 projection() const { return perspective(fov_, aspect_, near_, far_); }

    // W celu p³ynnego przechodzenia od jednego kónca zakresu do drugiego wykorzystamy funkcjê logistyczn¹
    float logistic(float y) {
        return 1.0f / (1.0f + exp(-y));
    }

    // i jej odwrotnoœæ:
    float inverse_logistics(float x) {
        return log(x / (1.0f - x));
    }

    // Niech y_offset bêdzie wartoœci¹ otrzyman¹ poprzez obrót kó³kiem myszy.
    void zoom(float y_offset) {
        // Najpierw obliczamy
        auto x = fov_ / pi<float>();
        // jest to liczba z zakresu [0,1], a nastêpnie liczymy
        auto y = inverse_logistics(x);
        // y jest ju¿ liczb¹ z zakresu (-Inf, Inf). Do tej liczby dodajemy nasz y_offset
        y += y_offset;
        // i przekszta³camy z powrotem do przedzia³u (0,1) za pomoc¹ funkcji logistycznej:
        x = logistic(y);
        // i w kóncu do przedzia³u (0, pi):
        fov_ = x * pi<float>();
    }

private:
    // Pola okreœlaj¹ce macierz projekcji
    float fov_;
    float aspect_;
    float near_;
    float far_;

    mat4 V_;
};