#define STB_IMAGE_IMPLEMENTATION
#include "3rdParty/src/stb/stb_image.h"
#include "pyramid.h"
#include <vector>
#include <iostream>
using namespace std;

Pyramid::Pyramid()
{
    // Do wczytania obrazka pos�u�y nam biblioteka stb_image
    auto texture_filename = std::string(PROJECT_DIR) + "/Textures/multicolor.png";
    int width, height, n_channels;
    uint8_t* data = stbi_load(texture_filename.c_str(), &width, &height, &n_channels, 0);
    if (data != nullptr) 
    {
        cout << "Za�adowano tekstur� " << width << "x" << height << " n_channels " << n_channels << endl;
    }
    else 
    {
        cerr << "B��d �adowania tekstury " << endl;
    }

    // bindujemy t� tekstur� do targetu GL_TEXTURE_2D
    glBindTexture(GL_TEXTURE_2D, diffuse_texture_);
    // i �adujemy do niej obrazek
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    // Na koniec ustalamy rodzaj interpolacji
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // I mo�emy ju� j� odbindowa�
    glBindTexture(GL_TEXTURE_2D, 0);

    /*
        Tu dodajemy kod tworz�cy bufory indeks�w i wierzcho�k�w oraz obiekt VAO,
        Potem �adujemy dane wierzcho�k�w i indeksow do bufor�w  i  pod�aczamy je do atrybut�w w szaderze
   */

   // Piramidka
    vector<GLfloat> vertices = {
        // Podstawa - kwadrat z dw�ch tr�jk�t�w
        // Lewy tr�jk�t
        -0.5f, -0.5f, -0.5f, 0.5f, 0.1910f, // 0
        0.5f, -0.5f, -0.5f, 0.809f, 0.5f, // 1
        -0.5f, 0.5f, -0.5f, 0.191f, 0.5f, // 2
        // Prawy tr�jk�t
        0.5f, 0.5f, -0.5f, 0.5f, 0.809f, // 3
        // Cztery razy powt�rzony "czubek" piramidy
        // Przednia �cianka
        0.0f, 0.0f, 0.5f, 1.0f, 0.0f, // 4
        // Tylna �cianka
        0.0f, 0.0f, 0.5f, 0.0f, 1.0f, // 5
        // Lewa �cianka
        0.0f, 0.0f, 0.5f, 0.0f, 0.0f, // 6
        // Prawa �cianka
        0.0f, 0.0f, 0.5f, 1.0f, 1.0f // 7
    };

    glGenBuffers(2, this->buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffer_[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Wypisujemy tyle element�w ile mamy wierzcho�k�w
    // Backface culling: �cianka ma byc zdefiniowana w ten spos�b, �e jak na ni� patrzymy to wierzcho�ki pobierane s� z bufora w kolejno�ci odwrotnej do wskaz�wek zegara (glFrontFace(GL_CCW);)
    vector<GLushort> indices = {
        // Podstawa - kwadrat z dw�ch tr�jk�t�w
        0,2,1, // Lewy tr�jk�t
        2,3,1, // Prawy tr�jk�t
        // Przednia �cianka
        0,1,4,
        // Tylna �cianka
        2,5,3,
        // Lewa �cianka
        2,0,6,
        // Prawa �cianka
        3,7,1
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffer_[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(),
        GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffer_[0]);

    // Pod��czenie pozycji wierzcho�k�w
    glEnableVertexAttribArray(0);
    // Define an array of generic vertex attribute data (index, size, type, normalized, stride, pointer)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));

    // Pod��czenie kolor�w wierzcho�k�w
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffer_[1]);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Pyramid::~Pyramid()
{
    //Tu usuwamy VAO i bufory
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(2, this->buffer_);
}

void Pyramid::draw()
{
    // Bindowanie tekstury do jednstki teksturuj�cej zero
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_texture_);

    //Tu wywolujemy polecenie glDrawElements
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid*>(0));
    glBindVertexArray(0);

    // Po wywo�aniu funkcji glDrawElements mo�emy tekstur� odbindowa�.
    glBindTexture(GL_TEXTURE_2D, 0);
}
