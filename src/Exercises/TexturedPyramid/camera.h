#pragma once
#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace std;
using namespace glm;

class Camera {
public:

    void look_at(const vec3& eye, const vec3& center, const vec3& up) {
        z_ = normalize(eye - center);
        x_ = normalize(cross(up, z_));
        y_ = normalize(cross(z_, x_));

        position_ = eye;
        center_ = center;
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

    // Zamiast zwraca� macierz V_, b�dziemy j� generowa� z nowych zmiennych "na ��danie"
    mat4 view() const {
        mat4 V(1.0f);
        for (int i = 0; i < 3; ++i) {
            V[i][0] = x_[i];
            V[i][1] = y_[i];
            V[i][2] = z_[i];
        }

        auto t = -vec3{
                dot(x_, position_),
                dot(y_, position_),
                dot(z_, position_),
        };
        V[3] = vec4(t, 1.0f);

        return V;

    }

    mat4 projection() const { return perspective(fov_, aspect_, near_, far_); }

    // W celu p�ynnego przechodzenia od jednego k�nca zakresu do drugiego wykorzystamy funkcj� logistyczn�
    float logistic(float y) {
        return 1.0f / (1.0f + exp(-y));
    }

    // i jej odwrotno��:
    float inverse_logistics(float x) {
        return log(x / (1.0f - x));
    }

    // Niech y_offset b�dzie warto�ci� otrzyman� poprzez obr�t k�kiem myszy.
    void zoom(float y_offset) {
        // Najpierw obliczamy
        auto x = fov_ / pi<float>();
        // jest to liczba z zakresu [0,1], a nast�pnie liczymy
        auto y = inverse_logistics(x);
        // y jest ju� liczb� z zakresu (-Inf, Inf). Do tej liczby dodajemy nasz y_offset
        y += y_offset;
        // i przekszta�camy z powrotem do przedzia�u (0,1) za pomoc� funkcji logistycznej:
        x = logistic(y);
        // i w k�ncu do przedzia�u (0, pi):
        fov_ = x * pi<float>();
    }

    vec3 x() const { return x_; }
    vec3 y() const { return y_; }
    vec3 z() const { return z_; }
    vec3 position() const { return position_; }
    vec3 center() const { return center_; }

    // Obr�t kamery wok� osi w punkcie centralnym polega na
    // 1. Obrocie po�ozenia kamery wok� tej osi i punktu.
    // 2. Obrotu wszystkich trzech wektor�w definiuj�cych orientacj� kamery wok� tej osi.
    // Pomocnicza funkcja tworz�ca macierz obrotu o kat angle wok� osi axis:
    inline mat3 rotation(float angle, const vec3& axis) {
        auto u = normalize(axis);
        auto s = sin(angle);
        auto c = cos(angle);

        return mat3(
            c + u.x * u.x * (1.0f - c),
            u.y * u.x * (1.0f - c) + u.z * s,
            u.z * u.x * (1.0f - c) - u.y * s,

            u.x * u.y * (1.0f - c) - u.z * s,
            c + u.y * u.y * (1.0f - c),
            u.z * u.y * (1.0f - c) + u.x * s,

            u.x * u.z * (1.0f - c) + u.y * s,
            u.y * u.z * (1.0f - c) - u.x * s,
            c + u.z * u.z * (1.0f - c)
        );
    }

    void rotate_around_point(float angle, const vec3& axis, const vec3& c) {
        auto R = rotation(angle, axis);
        x_ = R * x_;
        y_ = R * y_;
        z_ = R * z_;

        auto t = position_ - c;
        t = R * t;
        position_ = c + t;

    }

    void rotate_around_center(float angle, const vec3& axis) {
        rotate_around_point(angle, axis, center_);
    }

private:
    // Pola okre�laj�ce macierz projekcji
    float fov_;
    float aspect_;
    float near_;
    float far_;

    vec3 position_; // po�o�enie kamery
    vec3 center_; // punkt centralny
    // wektory definiuj�ce orientacj�
    vec3 x_;
    vec3 y_;
    vec3 z_;
};