#include "PlayUI.h"

PlayUI::PlayUI() {

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, ibo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(Vertex), playUI, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(unsigned int), playIndices, GL_STATIC_DRAW);

	glBindVertexArray(0);

}

void PlayUI::Draw() {

	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, 24 , GL_UNSIGNED_INT, playIndices);
	
	glBindVertexArray(0);

}