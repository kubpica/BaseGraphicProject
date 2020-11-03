//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>


#include "Application/utils.h"

void SimpleShapeApplication::init() {


    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");


    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    std::vector<GLfloat> vertices = {
            // Dach - tr�jk�t
            -0.8f, 0.2f, 0.0f, 1.0f, 0.0f, 0.0f, // 0
            0.8f, 0.2f, 0.0f, 1.0f, 0.0f, 0.0f, // 1
            0.0f, 0.6f, 0.0f, 1.0f, 0.0f, 0.0f, // 2
            // Blok - kwadrat z dw�ch tr�jk�t�w
            // Lewy tr�jk�t
            -0.4f, -0.6f, 0.0f, 1.0f, 1.0f, 0.0f, // 3
            0.4f, -0.6f, 0.0f, 1.0f, 1.0f, 0.0f, // 4
            -0.4f, 0.2f, 0.0f, 1.0f, 1.0f, 0.0f, // 5
            // Prawy tr�jk�t
            0.4f, 0.2f, 0.0f, 1.0f, 1.0f, 0.0f // 6
    };

    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Wypisujemy tyle element�w ile mamy wierzcho�k�w
    std::vector<GLushort> indices = {
           0,1,2, // Dach
           // Blok - kwadrat z dw�ch tr�jk�t�w
           3,4,5, // Lewy tr�jk�t
           5,4,6 // Prawy tr�jk�t 
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
            // Podpinamy blok Modifiers z shadera fragment�w pod punkt bindowania `0`
            auto u_modifiers_index = glGetUniformBlockIndex(program, "Modifiers");
            if (u_modifiers_index == GL_INVALID_INDEX)
            {
                std::cout << "Cannot find Modifiers uniform block in program" << std::endl;
            }
            else
            {
                glUniformBlockBinding(program, u_modifiers_index, 0);
            }

            // Nast�pnie tworzymy uniform buffer poleceniem ```glsl
            GLuint ubo_handle(0u);
            glGenBuffers(1, &ubo_handle);

            // Bindujemy go i alokujemy w nim 32 bajty pami�ci:
            glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle);
            glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);

            // Przesy�amy do niego dane korzystaj�c z polece� glBufferSubData
            float strength = 0.5;
            float light[3] = { 0.7, 0.2, 0.3 };
            // Updates a subset of a buffer object's data store
            // void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &strength);
            glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), light);

            // Teraz mo�emy ju� go odbindowa�
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
#pragma endregion

            // Przed rysowaniem musimy podpi�� bufor do zmiennej w szaderze
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_handle);

            // Pod��czenie pozycji wierzcho�k�w
            glEnableVertexAttribArray(0);
            // Define an array of generic vertex attribute data (index, size, type, normalized, stride, pointer)
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));

            // Pod��czenie kolor�w wierzcho�k�w
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    int w, h;
    std::tie(w, h) = frame_buffer_size();
    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);
}

void SimpleShapeApplication::frame() {
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
}
