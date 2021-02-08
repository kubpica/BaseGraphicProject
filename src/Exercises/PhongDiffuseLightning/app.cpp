//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>


#include "Application/utils.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp> // vec3
#include <glm/vec4.hpp> // vec4
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace xe;
using namespace glm;

void SimpleShapeApplication::init() {
    set_camera(new Camera);
    set_controler(new CameraControler(camera()));

    auto program = create_program(string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      string(PROJECT_DIR) + "/shaders/base_fs.glsl");


    if (!program) {
        cerr << "Cannot create program from " << string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        cerr << string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << endl;
    }

    quad = new Quad();

#pragma region Zadanie 5 - PVM
            // Podpinamy blok Transformations z shadera wierzcho³ków pod punkt bindowania `1`
            auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
            if (u_transformations_index == GL_INVALID_INDEX) 
            { 
                cout << "Cannot find Transformations uniform block in program" << endl; 
            }
            else 
            { 
                glUniformBlockBinding(program, u_transformations_index, 1); 
            }

            // Tworzymy uniform buffer poleceniem ```glsl
            glGenBuffers(1, &u_pvm_buffer_); 

            // Bindujemy go i alokujemy pamiêæ dla bufora
            glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
            glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(mat4), nullptr, GL_STATIC_DRAW);
            // W metodzie init pozostawiamy kod odpowiadaj¹cy za inicjalizacjê bufora u_pvm_buffer_
            // ale kod odpowiadaj¹cy za przesy³anie danych do bufora musimy przenieœæ do metody frame.
            glBindBuffer(GL_UNIFORM_BUFFER, 0); // Odbindowanie

            // Tworzymy macierz PVM bêd¹c¹ iloczynem macierzy P, V i M za pomoc¹ biblioteki glm (OpenGL mathematics).
            // Macierz modelu M mo¿emy pocz¹tkowo ustawiæ jako macierz jednostkow¹: mat4 M(1.0f);
            int w, h;
            tie(w, h) = frame_buffer_size();
            float aspect = (float)w / h;
            float fov = pi<float>() / 4.0;
            float near = 0.1f;
            float far = 100.0f;
            // Tworzymy macierz widoku V
            camera()->look_at(vec3{ 1.0, .5, 2.0 }
                                , vec3{ 0.0f, 0.0f, 0.0f }
                                , vec3{ 0.0, 0.0, 1.0 });
            // Macierz projekcji P
            camera()->setPerspective(fov, aspect, near, far);

            // Przed rysowaniem musimy podpi¹æ bufor do zmiennej w szaderze
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);
#pragma endregion

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    glViewport(0, 0, w, h);

#pragma region Backface culling
    // Wy³¹czenie rysowania œcianek tylnych
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
#pragma endregion

    glEnable(GL_DEPTH_TEST); // W³¹cza algorytm bufora g³êbi
    glUseProgram(program);

    auto  u_diffuse_map_location = glGetUniformLocation(program, "diffuse_map");
    if (u_diffuse_map_location == -1) {
        std::cerr << "Cannot find uniform diffuse_map" << std::endl;
    }
    else {
        glUniform1ui(u_diffuse_map_location, 0);
    }

    start_ = std::chrono::steady_clock::now();
}

void SimpleShapeApplication::frame() {
    // Poniewa¿ macierz projekcji P_ mo¿e zmieniaæ siê od klatki do klatki 
    // kod obliczaj¹cy i przesy³aj¹cy macierz PVM do szadera poprzez bufor uniform musi byæ w metodzie frame.
    glm::mat4 P = camera()->projection();
    glm::mat4 VM = camera()->view();

    auto R = glm::mat3(VM);
    auto N = glm::transpose(glm::inverse(R));

    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &P[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &VM[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), &N[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec4), sizeof(glm::vec3), &N[1]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 2 * sizeof(glm::vec4), sizeof(glm::vec3), &N[2]);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    
    this->quad->draw();
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0, 0, w, h);
    auto aspect = (float)w / h;
    camera()->set_aspect(aspect);
}
