#include "Stage.h"

void Stage::SetStageTile(const unsigned int index, const Complex cardinalDirection, const itrio tile, const unsigned int objectID) {

	const glm::mat4 translationMat = glm::translate(glm::mat4(1.f), glm::vec3{ tile.x * TILE_W, tile.y * TILE_H, tile.z * TILE_L });

	const glm::mat4 rotationMat = glm::mat4{
	cardinalDirection.y, 0, cardinalDirection.x, 0,
	0,					 1, 0,					 0,
   -cardinalDirection.x, 0, cardinalDirection.y, 0,
	0,					 0, 0,					 1
	};

	const glm::mat4 modelMat = translationMat * rotationMat;

	tiles[index] = { cardinalDirection, tile, objectID, modelMat };

	Mesh3D tempMesh = GetMesh(objectID);

	for (int i = 0; i < tempMesh.vertices.size(); i++) {

		if (glm::dot(glm::cross(tempMesh.vertices[i].normal, tempMesh.vertices[i].tangent), tempMesh.vertices[i].bitangent) < 0.0f) {
			tempMesh.vertices[i].tangent = -1.0f * tempMesh.vertices[i].tangent;
		}

		tempMesh.vertices[i].position = glm::vec3(modelMat * glm::vec4(tempMesh.vertices[i].position, 1.f));
		tempMesh.vertices[i].normal = glm::normalize(glm::vec3(modelMat * glm::vec4(tempMesh.vertices[i].normal, 0.f)));
		tempMesh.vertices[i].tangent = glm::normalize(glm::vec3(modelMat * glm::vec4(tempMesh.vertices[i].tangent, 0.f)));
		tempMesh.vertices[i].bitangent = glm::normalize(glm::vec3(modelMat * glm::vec4(tempMesh.vertices[i].bitangent, 0.f)));

	}

	mesh[index]= tempMesh;

}

void Stage::PushBackStageTile(const Complex cardinalDirection, const itrio tile, const unsigned int objectID) {

	const glm::mat4 translationMat = glm::translate(glm::mat4(1.f), glm::vec3{ tile.x * TILE_W, tile.y * TILE_H, tile.z * TILE_L });

	const glm::mat4 rotationMat = glm::mat4{
	cardinalDirection.y, 0, cardinalDirection.x, 0,
	0,					 1, 0,					 0,
   -cardinalDirection.x, 0, cardinalDirection.y, 0,
	0,					 0, 0,					 1
	};

	const glm::mat4 modelMat = translationMat * rotationMat;

	tiles.push_back({ cardinalDirection, tile, objectID, modelMat });

	//THANK DOG FOR COMPILER OPTIMIZATIONS

	Mesh3D tempMesh = GetMesh(objectID);
	//glm::mat4 inverseMat = glm::transpose(glm::inverse(modelMat));

	for (int i = 0; i < tempMesh.vertices.size(); i++) {

		tempMesh.vertices[i].position = glm::vec3(modelMat * glm::vec4(tempMesh.vertices[i].position, 1.f));
		tempMesh.vertices[i].normal = glm::normalize(glm::vec3(modelMat * glm::vec4(tempMesh.vertices[i].normal, 0.f)));
		tempMesh.vertices[i].tangent = glm::normalize(glm::vec3(modelMat * glm::vec4(tempMesh.vertices[i].tangent, 0.f)));
		tempMesh.vertices[i].bitangent = glm::normalize(glm::vec3(modelMat * glm::vec4(tempMesh.vertices[i].bitangent, 0.f)));
		//tempMesh.vertices[i].normal = glm::normalize(glm::vec3(inverseMat * glm::vec4(tempMesh.vertices[i].normal, 0.f)));
		//tempMesh.vertices[i].tangent = glm::normalize(glm::vec3(inverseMat * glm::vec4(tempMesh.vertices[i].tangent, 0.f)));
		//tempMesh.vertices[i].bitangent = glm::normalize(glm::vec3(inverseMat * glm::vec4(tempMesh.vertices[i].bitangent, 0.f)));
		
		if (glm::dot(glm::cross(tempMesh.vertices[i].normal, tempMesh.vertices[i].tangent), tempMesh.vertices[i].bitangent) < 0.0f) {
			tempMesh.vertices[i].tangent = -1.0f * tempMesh.vertices[i].tangent;
		}

	}

	mesh.emplace_back(tempMesh);

}

void Stage::PopBackStageTile() {

	tiles.pop_back();
	mesh.pop_back();

}

void Stage::GetDrawables(const Camera& camera) {

	int tileIterator = 0;

	for (int n = 0; n < textures.size(); n++) {

		int elements = 0;

		//tileIterator < tiles.size() must go first, otherwise, out of subscript vector error.
		//this is dumb
		while (tileIterator < tiles.size() && tiles[tileIterator].objectModelID == n) {

			const glm::vec3 tilePosition = { tiles[tileIterator].tile.x * TILE_W, tiles[tileIterator].tile.y * TILE_H, tiles[tileIterator].tile.z * TILE_L };

			//ASSUMES TILE AS A CUBE RATHER THAN A RECT. PRISM, WHICH IS ASSUMED AS A SPHERE FOR FRUSTUM CULLING
			//EASE CONSTANT IS MEANT TO GIVE PLAY AS TO CULL THINGS EARLIER
			//UNIT_CUBE_DIAGONAL_LENGTH IS GIVEN BY SQRT{1^2 + 1^2 + 1^2}
			if (camera.IsSphereInsideView(tilePosition, SPHERE_RADIUS)) {
				drawables.emplace_back(tileIterator);
				elements++;
			}

			tileIterator++;

		}

		elementsPerMaterial.emplace_back(elements);

	}

}

void Stage::DrawTilesOcclude(Shader& shader) {

	int tileIterator = 0;

	StartDraw();

	for (int n = 0; n < textures.size(); n++) {

		shader.SetUniform1i("u_Texture", textures[n].slot);

		glBindBuffer(GL_ARRAY_BUFFER, matVbo[n]);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Material), (void*)offsetof(Material, Ka));
		glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Material), (void*)offsetof(Material, Kd));
		glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(Material), (void*)offsetof(Material, Ks));
		glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, sizeof(Material), (void*)offsetof(Material, Ns));
		glVertexAttribPointer(9, 1, GL_FLOAT, GL_FALSE, sizeof(Material), (void*)offsetof(Material, Tr));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[n]);

		for (int i = 0; i < elementsPerMaterial[n]; i++) {

			Draw(drawables[tileIterator]);

			tileIterator++;

		}

	}

	EndDraw();

	drawables.clear();
	elementsPerMaterial.clear();

}

void Stage::ClearTiles() {
	tiles.clear();
}

void Stage::StartDraw() {
	glBindVertexArray(vao);
}

void Stage::Draw(const unsigned int modelID) {

	glBindBuffer(GL_ARRAY_BUFFER, vbo[modelID]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, normal));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, tangent));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, bitangent));
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, uv));

	glDrawElements(GL_TRIANGLES, mesh[modelID].indices.size(), GL_UNSIGNED_INT, 0);

}

void Stage::EndDraw() {
	glBindVertexArray(0);
}

void Stage::FillBuffer() {

	InitBuffer();

	glBindVertexArray(vao);

	for (int i = 0; i < mesh.size(); i++) {

		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
		glBufferData(GL_ARRAY_BUFFER, mesh[i].vertices.size() * sizeof(Vertex3D), mesh[i].vertices.data(), GL_STATIC_DRAW);
		mesh[i].vertices.clear();
		mesh[i].vertices.shrink_to_fit();

	}

	for (int i = 0; i < textures.size(); i++) {
		const Mesh3D tempMesh = GetMesh(i);

		glBindBuffer(GL_ARRAY_BUFFER, matVbo[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Material), &tempMesh.meshMaterial, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, tempMesh.indices.size() * sizeof(unsigned int), tempMesh.indices.data(), GL_STATIC_DRAW);
	}

	mesh.shrink_to_fit();

	glBindVertexArray(0);

}

void Stage::InitBuffer() {

	drawables.reserve(mesh.size());
	vbo.resize(mesh.size());
	matVbo.resize(textures.size());
	ibo.resize(textures.size());
	elementsPerMaterial.reserve(textures.size());

	glGenVertexArrays(1, &vao);
	glGenBuffers(mesh.size(), vbo.data());
	glGenBuffers(textures.size(), matVbo.data());
	glGenBuffers(textures.size(), ibo.data());

	glBindVertexArray(vao);

	for (int i = 0; i < mesh.size(); i++) {

		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, position));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, normal));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, tangent));
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, bitangent));
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, uv));

	}

	for (int i = 0; i < textures.size(); i++) {
		glBindBuffer(GL_ARRAY_BUFFER, matVbo[i]);

		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);
		glEnableVertexAttribArray(7);
		glEnableVertexAttribArray(8);
		glEnableVertexAttribArray(9);

		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Material), (void*)offsetof(Material, Ka));
		glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Material), (void*)offsetof(Material, Kd));
		glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(Material), (void*)offsetof(Material, Ks));
		glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, sizeof(Material), (void*)offsetof(Material, Ns));
		glVertexAttribPointer(9, 1, GL_FLOAT, GL_FALSE, sizeof(Material), (void*)offsetof(Material, Tr));

		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
		glVertexAttribDivisor(8, 1);
		glVertexAttribDivisor(9, 1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[i]);
	}

	glBindVertexArray(0);

}
