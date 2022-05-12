//
// Created by Владимир Шелестов aka DarthShelL on 25.04.2022.
//

#ifndef GRAPHICS_TEXTURE_H
#define GRAPHICS_TEXTURE_H

#include <string>
#include <iostream>
#include <glad/glad.h>


class Texture {
public:
    unsigned int id;
    int width;
    int height;
    Texture(unsigned int id, int width, int height);
    ~Texture();

    void bind();
};

extern Texture* load_texture(std::string filename);


#endif //GRAPHICS_TEXTURE_H
