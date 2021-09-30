#pragma once

#ifndef SHADER_H
    #define SHADER_H

    #include <GL/glew.h>
    #include <string>


// General purpsoe shader object. Compiles from file, generates
// compile/link-time error messages and hosts several utility
// functions for easy management.
class Shader {
  public:
    // state
    unsigned int ID;
    // constructor
    Shader() = default;
    // sets the current shader as active
    auto Use() -> Shader &;
    // compiles the shader from given source code
    void Compile(const char *vertexSource, const char *fragmentSource);
    // utility functions
    void SetFloat(const char *name, float value, bool useShader = false);
    void SetInteger(const char *name, int value, bool useShader = false);
    void SetVector2f(const char *name, float x, float y, bool useShader = false);
    void SetVector3f(const char *name, float x, float y, float z, bool useShader = false);
    void SetVector4f(const char *name, float x, float y, float z, float w, bool useShader = false);

  private:
    // checks if compilation or linking failed and if so, print the error logs
    void checkCompileErrors(unsigned int object, std::string type);
};

#endif
