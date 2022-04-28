//
// Created by Владимир Шелестов aka DarthShelL on 24.03.2022.
//

#include "TextureLoader.h"

#include <iostream>
#include <regex>
#include <png.h>
#include <glad/glad.h>
#include "../graphics/Texture.h"

#define TX_ERR 0
#define TX_PNG 1
#define TX_JPG 2

unsigned char _get_format(std::string filename) {
    // PNG
    if (std::regex_match(filename, std::regex("[^\\s]+(.*?)\\.(PNG|png)$")))
        return TX_PNG;

    // JPG
    if (std::regex_match(filename, std::regex("[^\\s]+(.*?)\\.(jpg|jpeg|JPG|JPEG)$")))
        return TX_JPG;

    return TX_ERR;
}

GLuint _load_png(const char *file, int *width, int *height) {
    FILE *f;
    int is_png, bit_depth, color_type, row_bytes;
    png_infop info_ptr, end_info;
    png_uint_32 t_width, t_height;
    png_byte header[8], *image_data;
    png_bytepp row_pointers;
    png_structp png_ptr;
    GLuint texture;
    int alpha;

    if (!(f = fopen(file, "r"))) {
        return 0;
    }
    fread(header, 1, 8, f);
    is_png = !png_sig_cmp(header, 0, 8);
    if (!is_png) {
        fclose(f);
        return 0;
    }
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(f);
        return 0;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
        fclose(f);
        return 0;
    }
    end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
        fclose(f);
        return 0;
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(f);
        return 0;
    }
    png_init_io(png_ptr, f);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, &t_width, &t_height, &bit_depth, &color_type, NULL, NULL, NULL);
    *width = t_width;
    *height = t_height;
    png_read_update_info(png_ptr, info_ptr);
    row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    image_data = (png_bytep) malloc(row_bytes * t_height * sizeof(png_byte));
    if (!image_data) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(f);
        return 0;
    }
    row_pointers = (png_bytepp) malloc(t_height * sizeof(png_bytep));
    if (!row_pointers) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(f);
        return 0;
    }
    for (unsigned int i = 0; i < t_height; ++i) {
        row_pointers[t_height - 1 - i] = image_data + i * row_bytes;
    }
    png_read_image(png_ptr, row_pointers);
    switch (png_get_color_type(png_ptr, info_ptr)) {
        case PNG_COLOR_TYPE_RGBA:
            alpha = GL_RGBA;
            break;
        case PNG_COLOR_TYPE_RGB:
            alpha = GL_RGB;
            break;
        default:
            printf("Color type %d not supported!\n", png_get_color_type(png_ptr, info_ptr));
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            return 0;
    }
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0, alpha, GL_UNSIGNED_BYTE, (GLvoid *) image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    fclose(f);
    return texture;
}

Texture *load_texture(std::string filename) {
    int width, height;

    // Check image format
    unsigned char format = _get_format(filename);
    switch (format) {
        case (TX_ERR):
            std::cerr << "ERROR::TEXTURELOADER::WRONG FORMAT" << std::endl;
            return nullptr;
            break;
        case (TX_PNG):
            GLuint texture = _load_png(filename.c_str(), &width, &height);
            if (texture == 0) {
                std::cerr << "ERROR::TEXTURELOADER::CAN'T LOAD TEXTURE: " << filename << std::endl;
                return nullptr;
            }
            return new Texture(texture, width, height);
            break;
    }

    return nullptr;
}

