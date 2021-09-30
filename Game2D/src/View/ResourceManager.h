#pragma once
#ifndef RESOURCE_MANAGER_H
    #define RESOURCE_MANAGER_H

    #include "Shader.h"
    #include "Texture.h"

    #include <GL/glew.h>
    #include <map>
    #include <string>

// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no
// public constructor is defined.
class ResourceManager {
  public:
    // deleted constructor, that is we do not want any actual resource manager objects. Its members and functions should
    // be publicly available (static).
    ResourceManager() = delete;
    // resource storage
    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Texture2D> Textures;
    // loads (and generates) a shader program from file loading vertex, fragment shader's source code.
    static auto LoadShader(const char *vShaderFile, const char *fShaderFile, const std::string &name) -> Shader;
    // retrieves a stored sader
    static auto GetShader(const std::string &name) -> Shader;
    // loads (and generates) a texture from file
    static auto LoadTexture(const char *file, const std::string &name) -> Texture2D;
    static auto LoadCompressedTexture(const char *file, const std::string &name) -> Texture2D;
    // retrieves a stored texture
    static auto GetTexture(const std::string &name) -> Texture2D;
    // properly de-allocates all loaded resources
    static void Clear();

  private:
    // loads and generates a shader from file
    static auto loadShaderFromFile(const char *vShaderFile, const char *fShaderFile) -> Shader;
    // loads a single texture from file
    static auto loadTextureFromFile(const char *file) -> Texture2D;

    static auto loadTextureFromCompressedFile(const char *file) -> Texture2D;
};

#endif