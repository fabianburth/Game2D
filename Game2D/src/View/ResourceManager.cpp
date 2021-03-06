#include "ResourceManager.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;


Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile);
    return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char* ImageFile, std::string name)
{
    Textures[name] = loadTextureFromFile(ImageFile);
    return Textures[name];
}

Texture2D ResourceManager::LoadCompressedTexture(const char* file, std::string name)
{
    Textures[name] = loadTextureFromCompressedFile(file);
    return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

void ResourceManager::Clear()
{
    // (properly) delete all shaders	
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
    // (properly) delete all textures
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char* ImageFile)
{
    // create texture object
    Texture2D texture;

    //Data read from the header of the BMP file
    unsigned char header[54]; //Each BMP file begins by a 54-bytes header
    unsigned int dataPos; //Position in the file where the actual data begins
    unsigned int imageSize; // = (width * height * 3)
    unsigned int width, height;
    //Actual RGB data
    unsigned char* data;

    FILE* file = fopen(ImageFile, "rb");
    if (!file)
    {
        std::cout << "Image could not be opened" << std::endl;
    }
    if (fread(header, 1, 54, file) != 54) //If there is no 54 bytes header, there's something wrong with the bmp file
    {
        std::cout << "Not a correct BMP file" << std::endl;
    }
    if (header[0] != 'B' || header[1] != 'M') //Every bmp file begins with BM
    {
        std::cout << "Not a correct BMP file" << std::endl;
    }

    //Read the metadata of the image
    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize == 0)
        imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component

    if (dataPos == 0)
        dataPos = 54; // The BMP header is done that way

    // Create a buffer
    data = new unsigned char[imageSize];
    // Read the actual data from the file into the buffer
    fread(data, 1, imageSize, file);

    // now generate texture
    texture.Generate(width, height, data);

    //Everything is in memory now, the file can be closed
    fclose(file);
    //and so can the image data
    delete[](data);

    return texture;
}

Texture2D ResourceManager::loadTextureFromCompressedFile(const char* file)
{
    Texture2D texture;

    unsigned char header[124];

    FILE* fp;

    /* try to open the file */
    fp = fopen(file, "rb");
    if (fp == NULL)
        std::cout << "Image could not be opened" << std::endl;

    /* verify the type of file */
    char filecode[4];
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0) {
        fclose(fp);
        std::cout << "Not a correct BMP file" << std::endl;
    }

    /* get the surface desc */
    fread(&header, 124, 1, fp);

    unsigned int height = *(unsigned int*)&(header[8]);
    unsigned int width = *(unsigned int*)&(header[12]);
    unsigned int linearSize = *(unsigned int*)&(header[16]);
    unsigned int mipMapCount = *(unsigned int*)&(header[24]);
    unsigned int fourCC = *(unsigned int*)&(header[80]);

    unsigned char* buffer;
    unsigned int bufsize;
    /* how big is it going to be including all mipmaps? */
    bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
    fread(buffer, 1, bufsize, fp);

    texture.GenerateFromCompressed(fourCC, height, width, linearSize, mipMapCount, buffer);
    /* close the file pointer */
    fclose(fp);
    delete(buffer);

    return texture;
}
