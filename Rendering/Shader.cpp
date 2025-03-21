#include "Shader.h"

Shader::Shader() : m_filePath(""), m_id(0)
{

}

Shader::Shader(const std::string& filepath) : m_id(0)
{
    set(filepath);
}

Shader::~Shader()
{
    glDeleteProgram(m_id);
    m_id = 0;
}

void Shader::set(const std::string& filepath)
{
    m_filePath = filepath;
    ShaderProgramSource sources = parseShader(m_filePath);
    m_id = createShader(sources);
}

ShaderProgramSource Shader::parseShader(const std::string& filepath)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::ifstream shader_source(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(shader_source, line, '\r'))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else ss[(int)type] << line << '\n';
    }
    //std::cout << ss[0].str() << std::endl;
    //std::cout << ss[1].str() << std::endl;
    std::cout << "Shader source parsed" << std::endl;
    return { ss[0].str(), ss[1].str() };

}

unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    std::cout << "Shader compiled" << std::endl;
    return id;
}

unsigned int Shader::createShader(ShaderProgramSource sources)
{
    unsigned int program = glCreateProgram();
    unsigned int vertex_shader = compileShader(GL_VERTEX_SHADER, sources.vertexSource);
    unsigned int fragment_shader = compileShader(GL_FRAGMENT_SHADER, sources.fragmentSource);

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

void Shader::bind() const
{
    glUseProgram(m_id);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

void Shader::setUniform1i(const std::string& name, int value) const
{
    glUniform1i(Shader::getUniformLocation(name), value);
}

void Shader::setUniform1f(const std::string& name, float value) const
{
    glUniform1f(Shader::getUniformLocation(name), value);
}

void Shader::setUniform2f(const std::string& name, float v0, float v1) const
{
    glUniform2f(Shader::getUniformLocation(name), v0, v1);
}

void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2) const
{
    glUniform3f(Shader::getUniformLocation(name), v0, v1, v2);
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const
{
    glUniform4f(Shader::getUniformLocation(name), v0, v1, v2, v3);
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix) const
{
    glUniformMatrix4fv(Shader::getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

int Shader::getUniformLocation(const std::string& name) const
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache[name];

    int location = glGetUniformLocation(m_id, name.c_str());
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' is invalid" << std::endl;

    m_uniformLocationCache[name] = location;
    return location;
}

