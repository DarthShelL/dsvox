//
// Created by Владимир Шелестов aka DarthShelL on 25.04.2022.
//

#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(unsigned int id, int width, int height) : id(id), width(width), height(height) {
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

void Texture::bind(){
    glBindTexture(GL_TEXTURE_2D, id);
}

Texture *load_texture(std::string filename) {
    unsigned int texture;
    glGenTextures(1, &texture);
    // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT);    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;

    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
        return nullptr;
    }
    stbi_image_free(data);

    return new Texture(texture, width, height);
}
