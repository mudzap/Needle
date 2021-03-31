#ifndef _MATERIAL_
#define _MATERIAL_

#include "Texture.h"
#include "Math.h"

#include <map>
#include <vector>
#include <string>
#include <functional>
#include <memory>

#include "glm/glm.hpp"

#define MAPS 3

struct Material {

	// AMBIENT
	glm::vec3 Ka = glm::vec3(0.1);

	// DIFFUSE
	glm::vec3 Kd = glm::vec3(0.8);

	// SPECULAR
	glm::vec3 Ks = glm::vec3(0.2);

	// SPECULAR EXPONENT
	float Ns = 32;

	//TRANSPARENCY
	float Tr = 0.0;

	int materialHandle;

};

struct TextureSet {

	//DIFFUSE, NORMAL AND SPECULAR, IN THAT ORDER.
	Texture textures[MAPS];

	int width;
	int height;
	int arrayIndicesCount;

	TextureSet(unsigned int textureSize = 0) : width(textureSize), height(textureSize), arrayIndicesCount(0) {
	}

	void PrepareTextureArrays(unsigned int arrayIndices) {

		arrayIndicesCount = arrayIndices;

		for (Texture& texture : textures) {

			glGenTextures(1, &texture.texture);
			glBindTexture(GL_TEXTURE_2D_ARRAY, texture.texture);

			glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, arrayIndicesCount, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

		}

	}

	void LoadTextures(std::vector<stbi_uc*> dataPointers[], unsigned int offset) {

		int n = 0;
		for (Texture& texture : textures) {

			glBindTexture(GL_TEXTURE_2D_ARRAY, texture.texture);

			for (int i = 0; i < dataPointers[n + offset].size(); i++) {
				glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, dataPointers[n + offset][i]);
			}
			n++;

			glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

		}

	}

};

template<unsigned int initialSize, unsigned int textureSizes>
class TextureMap {

	public:

		//
		// TEXTURES
		//	1024X1024		512X512			256X256...
		//
		// HANDLES
		//	1, 3, 5			0, 2			4
		//
		// RETURNS TEXTURE SLOT + TEXTURE ARRAY INDEX
		// 3 [0, 1, 2]		6 [0, 1]		9 [0]
		//
		// INDIVIDUAL MAPS ARE OFFSET IN SHADER.
		//
		// TEXTURE SLOT EXPRESSABLE AS:
		//		firstSlot + textureID * MAPS;
		//

		//#MAPS ARRAYS PER SIZE
		
		TextureMap(unsigned int startingSlot) :
			firstSlot(startingSlot),
			totalSlots(MAPS * textureSizes) {

			for (int i = 0; i < textureSizes; i++) {
				textures[i] = TextureSet(initialSize * pow(2, i));
			}

			//sizeCount.resize(textureSizes);

		};

		~TextureMap() {
			FlushTextures();
		}

		//ADDS TEXTURE SET FROM A SET OF STRINGS, RETURNS THE HANDLE
		int AddTextureSet(std::vector<std::string>& path) {

			stbi_set_flip_vertically_on_load(1);

			int expectedW, expectedH;

			int countIndex = 0;

			if (path.size() > MAPS) {
				printf("Attempting to load more textures than max amount of maps, texture paths are:\n");

				for (int i = 0; i < path.size(); i++) {
					printf("%s\n", path[i].c_str());
				}

				printf("Loading only %i.\n", MAPS);
			}

			for (int i = 0; i < path.size(); i++) {

				int width, height, bpp;
				bool success = true;

				data.emplace_back(stbi_load(path[i].c_str(), &width, &height, &bpp, 4));

				//FIRST TEXTURE DEFINES SIZE
				if (width != height) {
					printf("Texture in path %s must be square.\n", path[i].c_str());
					success = false;
				}

				if (data.back() == NULL) {
					printf("Failed to load texture %s.\n", path[i].c_str());
					success = false;
				}

				if (i == 0) {

					if (success) {

						expectedW = width;
						expectedH = height;

					}
					else {
						printf("Failed base texture loading at path %s with dimensions %ix%i, setting default texture.\n", path[i].c_str(), width, height);
						stbi_image_free(data.back());
						expectedW = 2;
						expectedH = 2;
					}

					//FUNCTION TO MAP:
					//	{2^N} -> {N - 1 + K} FOR ALL INTEGER N > 0;
					//FOR EXAMPLE:
					// {256, 512, 1024} -> {0, 1, 2}
					//
					// 1. COMPUTE COMMON DENOMINATOR
					// {1, 2, 4}*256 -> {0, 1, 2}
					//
					// 2. SOLVE F(2^N) = N
					//  2^N = N
					//  2 = LOG N (N)... LATER

					//FOR NOW, 'NAIVE' APPROACH WITH O(N) COMPLEXITY

					for (i = expectedW / initialSize; i > 0; i /= 2) {
						countIndex++;
					}

					//INDEX OFFSET (SO IF WIDTH < INITIALSIZE, WE CAN IGNORE)
					countIndex--;

					if (countIndex < 0 || countIndex > textureSizes - 1) {
						success = false;
						printf("Texture size not available, failed on texture %s with dimensions %ix%i.\n", path[i].c_str(), width, height);
					}

				}
				else if (width != expectedW || height != expectedH) {
					printf("Texture in path %s with dimensions %ix%i doesn't fit texture array with dimensions %ix%i.\n", path[i].c_str(), width, height, expectedW, expectedH);
					success = false;
				}


				if (!success) {
					stbi_image_free(data.back());
					//data.back() = errorTexture(expectedW, expectedH);
				}

				//sizeCount[countIndex]++;
				orderedData[countIndex * MAPS + i].emplace_back(data.back());

			}

			ipair dir;
			dir.x = firstSlot + countIndex * MAPS;
			dir.y = orderedData[countIndex * MAPS].size() - 1;

			arrayHandleMap.emplace_back(dir);
			return (arrayHandleMap.size() - 1);

		}

		void LoadTextureSets() {

			int i = 0;
			for (TextureSet& textureSet : this->textures) {
				//DIFFUSE MAP IS BASE TEXTURE, SO IT DICTATES THE SIZE, EASILY ADJUSTABLE BUT NOT NEEDED.
				textureSet.PrepareTextureArrays(orderedData[i * MAPS].size());
				textureSet.LoadTextures(orderedData, i * MAPS);
				i++;
			}

		}

		void BindTextures() {

			int i = 0;
			for (TextureSet& textureSet : this->textures) {

				for (Texture& texture : textureSet.textures) {

					texture.Bind(firstSlot + i);
					i++;

				}

			}

		}

		void UnbindTextures() {

			for (TextureSet textureSet : this->textures) {
				for (Texture texture : textureSet.textures) {
					texture.Unbind();
				}
			}

		}

		void FlushTextures() {

			currentHandle = -1;
			arrayHandleMap.clear();

			for (stbi_uc* image : data) {
				stbi_image_free(image);
			}

			data.clear();

			for (std::vector<stbi_uc*>& vector : orderedData) {
				vector.clear();
			}

		}

	private:

		//MAPS EACH HANDLE TO AN ARRAY INDEX
		std::vector<ipair> arrayHandleMap;
		//std::vector<unsigned int> sizeCount;

		int firstSlot = 0;
		int totalSlots = 0;
		int currentHandle = -1;

		//STORES ALL POINTERS TO IMAGE DATA
		std::vector<stbi_uc*> data;

		//STORES POINTER FOR EACH MAP TYPE FOR EACH SIZE
		std::vector<stbi_uc*> orderedData[MAPS * textureSizes];

		TextureSet textures[textureSizes];
		
};

#endif