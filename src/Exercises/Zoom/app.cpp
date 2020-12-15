//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>


#include "Application/utils.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/gtc/matrix_transform.hpp>

void SimpleShapeApplication::init() {
    set_camera(new Camera);

    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");


    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    // Piramidka
    std::vector<GLfloat> vertices = {
            // Podstawa - kwadrat z dwóch trójk¹tów
            // Lewy trójk¹t
            -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, // 0
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, // 1
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, // 2
            // Prawy trójk¹t
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, // 3
            // Przednia œcianka
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 4
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 5
            0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, // 6
            // Tylna œcianka
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 7
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 8
            0.0f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, // 9
            // Lewa œcianka
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 10
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 11
            0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, // 12
            // Prawa œcianka
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f, // 13
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, // 14
            0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f // 15
    };

    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Wypisujemy tyle elementów ile mamy wierzcho³ków
    // Backface culling: Œcianka ma byc zdefiniowana w ten sposób, ¿e jak na ni¹ patrzymy to wierzcho³ki pobierane s¹ z bufora w kolejnoœci odwrotnej do wskazówek zegara (glFrontFace(GL_CCW);)
    std::vector<GLushort> indices = {
           // Podstawa - kwadrat z dwóch trójk¹tów
           0,2,1, // Lewy trójk¹t
           2,3,1, // Prawy trójk¹t
           // Przednia œcianka
           4,5,6,
           // Tylna œcianka
           7,9,8,
           // Lewa œcianka
           10,11,12,
           // Prawa œcianka
           13,15,14
    };
    GLuint idx_buffer_handle;
    glGenBuffers(1, &idx_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(),
        GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);

        glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);

#pragma region Zadanie 4 - Uniform interface block
            // Podpinamy blok Modifiers z shadera fragmentów pod punkt bindowania `0`
            auto u_modifiers_index = glGetUniformBlockIndex(program, "Modifiers");
            if (u_modifiers_index == GL_INVALID_INDEX)
            {
                std::cout << "Cannot find Modifiers uniform block in program" << std::endl;
            }
            else
            {
                glUniformBlockBinding(program, u_modifiers_index, 0);
            }

            // Nastêpnie tworzymy uniform buffer poleceniem ```glsl
            GLuint u_buffer_handle;
            glGenBuffers(1, &u_buffer_handle);

            // Bindujemy go i alokujemy w nim 32 bajty pamiêci
            glBindBuffer(GL_UNIFORM_BUFFER, u_buffer_handle);
            glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);

            // Przesy³amy do niego dane korzystaj¹c z poleceñ glBufferSubData
            float strength = 0.5; // light_intensity
            float light[3] = { 1.0, 1.0, 1.0 }; // light_color
            // Updates a subset of a buffer object's data store
            // void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &strength);
            glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), light);

            // Teraz mo¿emy ju¿ go odbindowaæ
            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            // Przed rysowaniem musimy podpi¹æ bufor do zmiennej w szaderze
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, u_buffer_handle);
#pragma endregion

#pragma region Zadanie 5 - PVM
            // Podpinamy blok Transformations z shadera wierzcho³ków pod punkt bindowania `1`
            auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
            if (u_transformations_index == GL_INVALID_INDEX) 
            { 
                std::cout << "Cannot find Transformations uniform block in program" << std::endl; 
            }
            else 
            { 
                glUniformBlockBinding(program, u_transformations_index, 1); 
            }

            // Tworzymy uniform buffer poleceniem ```glsl
            glGenBuffers(1, &u_pvm_buffer_); 

            // Bindujemy go i alokujemy pamiêæ dla bufora
            glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
            // W metodzie init pozostawiamy kod odpowiadaj¹cy za inicjalizacjê bufora u_pvm_buffer_
            // ale kod odpowiadaj¹cy za przesy³anie danych do bufora musimy przenieœæ do metody frame.
            glBindBuffer(GL_UNIFORM_BUFFER, 0); // Odbindowanie

            // Tworzymy macierz PVM bêd¹c¹ iloczynem macierzy P, V i M za pomoc¹ biblioteki glm (OpenGL mathematics).
            // Macierz modelu M mo¿emy pocz¹tkowo ustawiæ jako macierz jednostkow¹: glm::mat4 M(1.0f);
            int w, h;
            std::tie(w, h) = frame_buffer_size();
            float aspect = (float)w / h;
            float fov = glm::pi<float>() / 4.0;
            float near = 0.1f;
            float far = 100.0f;
            // Tworzymy macierz widoku V
            camera()->look_at(glm::vec3{ 1.0, .5, 2.0 }
                                , glm::vec3{ 0.0f, 0.0f, 0.0f }
                                , glm::vec3{ 0.0, 0.0, 1.0 });
            // Macierz projekcji P
            camera()->setPerspective(fov, aspect, near, far);

            // Przed rysowaniem musimy podpi¹æ bufor do zmiennej w szaderze
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);
#pragma endregion

            // Pod³¹czenie pozycji wierzcho³ków
            glEnableVertexAttribArray(0);
            // Define an array of generic vertex attribute data (index, size, type, normalized, stride, pointer)
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));

            // Pod³¹czenie kolorów wierzcho³ków
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


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
}

void SimpleShapeApplication::frame() {
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);

    // Poniewa¿ macierz projekcji P_ mo¿e zmieniaæ siê od klatki do klatki 
    // kod obliczaj¹cy i przesy³aj¹cy macierz PVM do szadera poprzez bufor uniform musi byæ w metodzie frame.
    auto PVM = camera()->projection() * camera()->view();
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0, 0, w, h);
    auto aspect = (float)w / h;
    camera()->set_aspect(aspect);
}
