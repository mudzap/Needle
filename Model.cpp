#include "Model.h"


Model::Model() {
}

Model::~Model() {
}

void Model::LoadModel(const std::string& filepath, const std::string& texture, unsigned int slot) {

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(
		filepath,
		aiProcess_CalcTangentSpace |
		aiProcess_GenSmoothNormals |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ImproveCacheLocality |
		aiProcess_FindDegenerates |
		aiProcess_Triangulate |
		aiProcess_FindInvalidData
	);

	if (!scene) {
		printf("Couldn't find model: %s.\n", filepath.c_str());
		return;
	}
	else if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
		printf("Model in dir %s is incomplete.\n", filepath.c_str());
		return;
	}
	else if (!scene->mRootNode) {
		printf("Model in dir %s has no root node.\n", filepath.c_str());
		return;
	}


	mesh.reserve(scene->mNumMeshes);
	ProcessNode(scene->mRootNode, scene);

	for (int i = 0; i < scene->mNumMeshes; i++) {

		textures.emplace_back(Texture());
		textures.back().LoadTextureMipmap(texture);
		textures.back().Bind(slot);

	}

}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->mesh.push_back(ProcessMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh3D Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {

	std::string name = "Default";
	std::vector<Vertex3D> vertices;
	std::vector<unsigned int> indices;
	Material material; //DO LATER

	// process vertex positions, normals and texture coordinates
	vertices.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {

		glm::vec3 position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		glm::vec3 normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
		glm::vec3 tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
		glm::vec3 bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };

		glm::vec2 uv = { 0, 0 };
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			uv.x = mesh->mTextureCoords[0][i].x;
			uv.y = mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(Vertex3D{ position, normal, tangent, bitangent, uv });
	}


	unsigned int indicesNum = 0;

	// process indices
	indices.reserve(mesh->mNumFaces * 3);
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		indicesNum += face.mNumIndices;
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	printf("Mesh vertices: %i \tMesh faces: %i\t Mesh indices: %i\n", mesh->mNumVertices, mesh->mNumFaces, indicesNum);


	// process material
	if (mesh->mMaterialIndex >= 0) {
		/*
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		*/
	}

	return Mesh3D{ name, vertices, indices, material };
}

void Model::StartDraw() {
	glBindVertexArray(vao);
}

void Model::Draw(const unsigned int modelID) {

	glBindBuffer(GL_ARRAY_BUFFER, vbo[modelID]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, normal));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, tangent));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, bitangent));
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, uv));

	glBindBuffer(GL_ARRAY_BUFFER, matVbo[modelID]);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Material), (void*)offsetof(Material, Ka));
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Material), (void*)offsetof(Material, Kd));
	glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(Material), (void*)offsetof(Material, Ks));
	glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, sizeof(Material), (void*)offsetof(Material, Ns));
	glVertexAttribPointer(9, 1, GL_FLOAT, GL_FALSE, sizeof(Material), (void*)offsetof(Material, Tr));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[modelID]);

	glDrawElements(GL_TRIANGLES, mesh[modelID].indices.size(), GL_UNSIGNED_INT, 0);

}

void Model::EndDraw() {
	glBindVertexArray(0);
}

void Model::FillBuffer() {

	InitBuffer();

	glBindVertexArray(vao);

	for (int i = 0; i < mesh.size(); i++) {


		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
		glBufferData(GL_ARRAY_BUFFER, mesh[i].vertices.size() * sizeof(Vertex3D), mesh[i].vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, matVbo[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Material), &mesh[i].meshMaterial, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh[i].indices.size() * sizeof(unsigned int), mesh[i].indices.data(), GL_STATIC_DRAW);

	}

	glBindVertexArray(0);

}

void Model::InitBuffer() {

	vbo.resize(mesh.size());
	matVbo.resize(mesh.size());
	ibo.resize(mesh.size());

	glGenVertexArrays(1, &vao);
	glGenBuffers(mesh.size(), vbo.data());
	glGenBuffers(mesh.size(), matVbo.data());
	glGenBuffers(mesh.size(), ibo.data());

	glBindVertexArray(vao);

	for (int i = 0; i < mesh.size(); i++) {

		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[i]);

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

	}

	glBindVertexArray(0);

}

Mesh3D Model::GetMesh(const unsigned int modelID)
{
	return mesh[modelID];
}