#include "shader.h"

shader::shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vertexFile;
    std::ifstream fragmentFile;

    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);

        std::stringstream vertexStream, fragmentStream;

        vertexStream << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();

        vertexCode = vertexStream.str();
        fragmentCode = fragmentStream.str();

        vertexFile.close();
        fragmentFile.close();
    }

    catch(std::ifstream::failure) { 
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }


    const char* strVertexCode = vertexCode.c_str();
    const char* strFragmentCode = fragmentCode.c_str();

    GLuint vertexShader, fragmentShader;
    char infoLog[512];
    int succes;


    // create & compile vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1,  &strVertexCode, NULL);
    glCompileShader(vertexShader);


    // check vertex shader
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &succes);
    if(!succes) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    // create & compile fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1,  &strFragmentCode, NULL);
    glCompileShader(fragmentShader);

    // check fragment shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &succes);
    if(!succes) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    ///create program & set the id
    this->id = glCreateProgram();
    glAttachShader(this->id, vertexShader);
    glAttachShader(this->id, fragmentShader);
    glLinkProgram(this->id);

    // check linking err
    glGetProgramiv(this->id, GL_LINK_STATUS, &succes);
    if(!succes) {
        glGetShaderInfoLog(this->id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

shader::~shader() {

}

void shader::use() const {
    glUseProgram(this->id);
}

GLuint shader::getId() const {
    return this->id;
}
