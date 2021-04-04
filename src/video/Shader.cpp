#include "Shader.h"

Shader::Shader(){
}

Shader::Shader(const std::string& filepathVertex, const std::string& filepathFragment)
    : filepathVertex(filepathVertex), filepathFragment(filepathFragment), shader(0) {
    shader = CreateShader(OpenShader(filepathVertex), OpenShader(filepathFragment));
}

std::string Shader::glslVersion;

Shader::~Shader() {
    glDeleteProgram(shader);
}

void Shader::InitShader(const std::string& filepathVertex, const std::string& filepathFragment) {
    this->filepathVertex = filepathVertex;
    this->filepathFragment = filepathFragment;
    shader = CreateShader(OpenShader(filepathVertex), OpenShader(filepathFragment));
}

void Shader::ReInitShader() {
    glDeleteProgram(shader);
    shader = CreateShader(OpenShader(filepathVertex), OpenShader(filepathFragment));
}

std::string Shader::OpenShader(const std::string& path) {

    std::ifstream file(path);
    std::string temp;
    std::string shaderSource = Shader::glslVersion + "\n";
    while (std::getline(file, temp)) {
        shaderSource += temp + "\n";
    }

    return shaderSource;
}

unsigned int Shader::CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
    unsigned int program = glCreateProgram();
    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glValidateProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    //printf("source\n: %s\n", src);
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        printf("Unable to compile shader %d!\n", id);
        PrintShaderLog(id);
        return 0;
    }

    return id;
}

void Shader::PrintShaderLog(unsigned int shader)
{
    //Make sure name is shader
    if (glIsShader(shader))
    {
        //Shader log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        //Get info string length
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        //Allocate string
        char* infoLog = new char[maxLength];

        //Get info log
        glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0)
        {
            //Print Log
            printf("%s\n", infoLog);
        }

        //Deallocate string
        delete[] infoLog;
    }
    else
    {
        printf("Name %d is not a shader\n", shader);
    }
}

void Shader::Bind() const {
    glUseProgram(shader);
}

void Shader::Unbind() const{
    glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) {
    glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1) {
    glUniform2f(GetUniformLocation(name), v0, v1);
}

void Shader::SetUniform1f(const std::string& name, float v0) {
    glUniform1f(GetUniformLocation(name), v0);
}

void Shader::SetUniform1fArray(const std::string& name, unsigned int size, float* v0) {
    glUniform1fv(GetUniformLocation(name), size, v0);
}

void Shader::SetUniform4i(const std::string& name, int v0, int v1, int v2, int v3) {
    glUniform4i(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform3i(const std::string& name, int v0, int v1, int v2) {
    glUniform3i(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform2i(const std::string& name, int v0, int v1) {
    glUniform2i(GetUniformLocation(name), v0, v1);
}

void Shader::SetUniform1iArray(const std::string& name, unsigned int size, int* v0) {
    glUniform1iv(GetUniformLocation(name), size, v0);
}

void Shader::SetUniform1i(const std::string& name, int v0) {
    glUniform1i(GetUniformLocation(name), v0);
}

void Shader::SetUniformMat3f(const std::string& name, const glm::mat3 matrix) {
    glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4 matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

int Shader::GetUniformLocation(const std::string& name)
{

    if (locationCache.find(name) != locationCache.end())
        return locationCache[name];


    int location = glGetUniformLocation(shader, name.c_str());
    if (location == -1)
        printf("Uniform %s doesn't exist.\n", name.c_str());

    locationCache[name] = location;

    return location;
}

void Shader::DestroyShader() {
    glDeleteProgram(shader);
}