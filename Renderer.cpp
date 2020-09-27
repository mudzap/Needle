#include "Renderer.h"

void Renderer::InitBuffers() {

	//GEN
	/* SHOULD BE PER OBJECT, NOT DELEGATED TO RENDERER
	glGenVertexArrays(1, &instancedVao);
	glGenBuffers(1, &instancedMeshVbo);
	glGenBuffers(1, &instanceTransfVbo);
	*/

	glGenVertexArrays(1, &fanVao);
	glGenBuffers(1, &fanVbo);

	//glGenVertexArrays(1, &fanTransfVao);
	//glGenBuffers(1, &fanTransfVbo);

	glGenVertexArrays(1, &stripVao);
	glGenBuffers(1, &stripVbo);

	glGenVertexArrays(1, &trisVao);
	glGenBuffers(1, &trisVbo);
	glGenBuffers(1, &trisIbo);

	//INSTANCED RENDERING
	/* SHOULD BE PER OBJECT, NOT DELEGATED TO RENDERER
	glBindVertexArray(instancedVao);

	glBindBuffer(GL_ARRAY_BUFFER, instancedMeshVbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindBuffer(GL_ARRAY_BUFFER, instancedTransfVbo);

	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)offsetof(TVertex, translation));
	glVertexAttribDivisor(2, 1);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)(sizeof(float) * 0 + offsetof(TVertex, transform)));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)(sizeof(float) * 4 + offsetof(TVertex, transform)));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)(sizeof(float) * 8 + offsetof(TVertex, transform)));
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)(sizeof(float) * 12 + offsetof(TVertex, transform)));
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	*/

	//FAN(TEXT)
	glBindVertexArray(fanVao);

	glBindBuffer(GL_ARRAY_BUFFER, fanVbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));


	//FAN(TRANSF)
	//glBindVertexArray(fanTransfVao);

	//glBindBuffer(GL_ARRAY_BUFFER, fanTransfVbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	//glBindBuffer(GL_ARRAY_BUFFER, fanTransfInstanceVbo);

	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)offsetof(TVertex, translation));
	glVertexAttribDivisor(2, 1);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)(sizeof(float) * 0 + offsetof(TVertex, transform)));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)(sizeof(float) * 4 + offsetof(TVertex, transform)));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)(sizeof(float) * 8 + offsetof(TVertex, transform)));
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)(sizeof(float) * 12 + offsetof(TVertex, transform)));
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);


	//STRIP
	glBindVertexArray(stripVao);

	glBindBuffer(GL_ARRAY_BUFFER, stripVbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	//TRIS
	glBindVertexArray(trisVao);

	glBindBuffer(GL_ARRAY_BUFFER, trisVbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trisIbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindVertexArray(0);
}