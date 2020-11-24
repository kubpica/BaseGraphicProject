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


    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");


    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    // Piramidka
    std::vector<GLfloat> vertices = {
            // Podstawa - kwadrat z dw�ch tr�jk�t�w
            // Lewy tr�jk�t
            -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, // 0
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, // 1
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, // 2
            // Prawy tr�jk�t
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, // 3
            // Przednia �cianka
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 4
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 5
            0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, // 6
            // Tylna �cianka
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 7
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 8
            0.0f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, // 9
            // Lewa �cianka
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 10
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 11
            0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, // 12
            // Prawa �cianka
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f, // 13
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, // 14
            0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f // 15
    };

    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Wypisujemy tyle element�w ile mamy wierzcho�k�w
    // Backface culling: �cianka ma byc zdefiniowana w ten spos�b, �e jak na ni� patrzymy to wierzcho�ki pobierane s� z bufora w kolejno�ci odwrotnej do wskaz�wek zegara (glFrontFace(GL_CCW);)
    std::vector<GLushort> indices = {
           // Podstawa - kwadrat z dw�ch tr�jk�t�w
           0,1,2, // Lewy tr�jk�t
           2,1,3, // Prawy tr�jk�t 
           // Przednia �cianka
           4,5,6,
           // Tylna �cianka
           7,9,8,
           // Lewa �cianka
           10,11,12,
           // Prawa �cianka
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
            GLuint u_buffer_handle[2];
            glGenBuffers(2, u_buffer_handle);

            // Bindujemy go i alokujemy w nim 32 bajty pami�ci
            glBindBuffer(GL_UNIFORM_BUFFER, u_buffer_handle[0]);
            glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);

            // Przesy�amy do niego dane korzystaj�c z polece� glBufferSubData
            float strength = 0.5; // light_intensity
            float light[3] = { 1.0, 1.0, 1.0 }; // light_color
            // Updates a subset of a buffer object's data store
            // void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &strength);
            glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), light);

            // Teraz mo�emy ju� go odbindowa�
            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            // Przed rysowaniem musimy podpi�� bufor do zmiennej w szaderze
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, u_buffer_handle[0]);
#pragma endregion

#pragma region Zadanie 5 - PVM
            // Podpinamy blok Transformations z shadera wierzcho�k�w pod punkt bindowania `1`
            auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
            if (u_transformations_index == GL_INVALID_INDEX) 
            { 
                std::cout << "Cannot find Transformations uniform block in program" << std::endl; 
            }
            else 
            { 
                glUniformBlockBinding(program, u_transformations_index, 1); 
            }

            // Bindujemy go i alokujemy pami�� dla bufora
            glBindBuffer(GL_UNIFORM_BUFFER, u_buffer_handle[1]);
            glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);

            // Tworzymy macierz PVM b�d�c� iloczynem macierzy P, V i M za pomoc� biblioteki glm (OpenGL mathematics).
            // Macierz modelu M mo�emy pocz�tkowo ustawi� jako macierz jednostkow�: glm::mat4 M(1.0f);
            int w, h;
            std::tie(w, h) = frame_buffer_size();
            // Tworzymy macierz widoku V
            auto V = glm::lookAt(glm::vec3{ 1.0, .5, 2.0 }
                                , glm::vec3{ 0.0f, 0.0f, 0.0f }
                                , glm::vec3{ 0.0, 0.0, 1.0 });
            // Macierz projekcji P
            auto P = glm::perspective(glm::half_pi<float>(), (float)w / h, 0.1f, 100.0f);
            
            // Przesy�amy macierz PVM do bufora uniform
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &P[0]);
            glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &V[0]);
                // Adres pierwszego elementu macierzy mo�emy pobra� pobra� wyra�eniem &PVM[0].
                // Oczywi�cie moglibysmy r�wnie� jednocze�nie zaalokowa� pamie� i przes�a� macierz do bufora poleceniem glBuferSubdata. 
                // Ale nast�pnych �wiczeniach b�dziemy przesy�ali wi�cej macierzy oraz b�dziemy wielokrotnie przesy�ali macierze w ci�gu jednej klatki. Dlatego lepiej rodzieli� te dwa procesy.


            // Teraz mo�emy ju� go odbindowa�
            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            // Przed rysowaniem musimy podpi�� bufor do zmiennej w szaderze
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_buffer_handle[1]);
#pragma endregion

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
    /*int w, h;
    std::tie(w, h) = frame_buffer_size();*/
    glViewport(0, 0, w, h);

#pragma region Backface culling
    // Wy��czenie rysowania �cianek tylnych
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
#pragma endregion

    glEnable(GL_DEPTH_TEST); // W��cza algorytm bufora g��bi
    glUseProgram(program);
}

void SimpleShapeApplication::frame() {
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
}
