#pragma once

#ifndef TEXTURE_H
    #define TEXTURE_H

    #include <GL/glew.h>

    #define FOURCC_DXT1 0x31545844// Equivalent to "DXT1" in ASCII
    #define FOURCC_DXT3 0x33545844// Equivalent to "DXT3" in ASCII
    #define FOURCC_DXT5 0x35545844// Equivalent to "DXT5" in ASCII

// Texture2D is able to store and configure a texture in OpenGL.
// It also hosts utility functions for easy management.
class Texture2D {
  public:
    // holds the ID of the texture object, used for all texture operations to reference to this particlar texture
    unsigned int ID;
    // texture image dimensions
    unsigned int Width;// width and height of loaded image in pixels
    // texture Format
    unsigned int Internal_Format;// format of texture object
    unsigned int Image_Format;   // format of loaded image
    // texture configuration
    unsigned int Wrap_S;    // wrapping mode on S axis
    unsigned int Wrap_T;    // wrapping mode on T axis
    unsigned int Filter_Min;// filtering mode if texture pixels < screen pixels
    unsigned int Filter_Max;// filtering mode if texture pixels > screen pixels
    // constructor (sets default texture modes)
    Texture2D();
    // generates texture from image data
    void Generate(unsigned int width, unsigned int height, unsigned char *data);
    void GenerateFromCompressed(unsigned int fourCC, unsigned int height, unsigned int width, unsigned int linearSize,
                                unsigned int mipMapCount, unsigned char *data) const;
    // binds the texture as the current active GL_TEXTURE_2D texture object
    void Bind() const;
};

#endif