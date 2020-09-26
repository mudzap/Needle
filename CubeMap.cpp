#include "CubeMap.h"

//HARDCODED EVERYTHING BECAUSE NOT DEALING WITH THIS, ITS GOING TO BE STATIC ANYWAYS

CubeMap::CubeMap() {

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}

CubeMap::~CubeMap() {

}

void CubeMap::Draw(Shader& shader, Camera& camera) {

    glDepthFunc(GL_LEQUAL);

    shader.SetUniformMat4f("u_VP", camera.projection * camera.view);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);

    glDepthFunc(GL_LESS);

}
