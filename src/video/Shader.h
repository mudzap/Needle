#ifndef _SHADER_
#define _SHADER_

//#define GLM_FORCE_ALIGNED
//#define GLM_FORCE_SSE2

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
//#include <gl/glu.h>
#include <unordered_map>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

class Shader {

public:

	Shader();
	Shader(const std::string& filepathVertex, const std::string& filepathFragment);
	~Shader();

	void InitShader(const std::string& filepathVertex, const std::string& filepathFragment);
	void ReInitShader();
	void PrintShaderLog(unsigned int shader);

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform2f(const std::string& name, float v0, float v1);
	void SetUniform1f(const std::string& name, float v0);
	void SetUniform1fArray(const std::string& name, unsigned int size, float* v0);
	void SetUniform4i(const std::string& name, int v0, int v1, int v2, int v3);
	void SetUniform3i(const std::string& name, int v0, int v1, int v2);
	void SetUniform2i(const std::string& name, int v0, int v1);
	void SetUniform1iArray(const std::string& name, unsigned int size, int* v0);
	void SetUniform1i(const std::string& name, int v0);
	void SetUniformMat3f(const std::string& name, const glm::mat3 matrix);
	void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);

	void DestroyShader();

	static std::string glslVersion;

private:

	std::string filepathVertex;
	std::string filepathFragment;
	std::unordered_map<std::string, int> locationCache;

	unsigned int shader;

	unsigned int CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
	std::string OpenShader(const std::string& path);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	int GetUniformLocation(const std::string& name);


};

#endif