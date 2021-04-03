#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(Shader& shader)
{
    this->shader = shader;
    this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::DrawSprite(Texture2D& texture, std::array<float, 2> position, std::array<float, 4> size, std::array<float, 3> color)
{
    // prepare transformations
    this->shader.Use();

    // the first 2 elements of size are the scaling values for x and y
    // the other 2 elements are the values to correct the position when scaling
    this->shader.SetVector4f("size", size[0], size[1], size[2], size[3]);
    this->shader.SetVector2f("position", position[0], position[1]);
    // render textured quad
    this->shader.SetVector3f("spriteColor", color[0], color[1], color[2]);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::initRenderData()
{
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // pos                                                                                      // tex
        //x                                      //y
        -1.0f + 0.5f * WIDTH_UNIT,              -1.0f + 0.5f * HEIGHT_UNIT,                         0.0f, 0.0f, //bottom-left
        -1.0f + 0.5f * WIDTH_UNIT,              -1.0f + 0.5f * HEIGHT_UNIT + HEIGHT_UNIT,           0.0f, 1.0f, //top-left
        -1.0f + 0.5f * WIDTH_UNIT + WIDTH_UNIT, -1.0f + 0.5f * HEIGHT_UNIT,                         1.0f, 0.0f, //bottom-right

        -1.0f + 0.5f * WIDTH_UNIT,              -1.0f + 0.5f * HEIGHT_UNIT + HEIGHT_UNIT,           0.0f, 1.0f, //top-left
        -1.0f + 0.5f * WIDTH_UNIT + WIDTH_UNIT, -1.0f + 0.5f * HEIGHT_UNIT + HEIGHT_UNIT,           1.0f, 1.0f, //top-right
        -1.0f + 0.5f * WIDTH_UNIT + WIDTH_UNIT, -1.0f + 0.5f * HEIGHT_UNIT,                         1.0f, 0.0f, //bottom-right
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}