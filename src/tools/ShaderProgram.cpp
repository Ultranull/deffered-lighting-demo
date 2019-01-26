#include "ShaderProgram.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

string read(const char *file) {//FIX! place in a header as a util
	string content;
	ifstream stream(file);
	if (stream.is_open()) {
		stringstream ss;
		ss << stream.rdbuf();
		content = ss.str();
		stream.close();
	}
	else {
		printf("Failed to open %s\n", file);
		return "";
	}
	return content;
}



bool compile(const char* file, GLuint id) {
	GLint result = GL_FALSE;
	int infoLogLength;
	printf("Compiling shader: %s\n", file);
	string content = read(file);
	if (content.compare("") == 0) {
		printf("compile error: file not found!");
		return false; 
	}
	char const * src = content.c_str();
	const GLint length = content.size();
	glShaderSource(id, 1, &src, &length);
	glCompileShader(id);

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 1) {
		vector<char> errormessage(infoLogLength + 1);
		glGetShaderInfoLog(id, infoLogLength, NULL, &errormessage[0]);
		printf("%s compile error:\n\t%s\n", file, &errormessage[0]);
		return false;
	}
	return true;
}

Shader::Shader(std::string file, GLuint t) :type(t) {
	id = glCreateShader(t);
	if (!compile(file.c_str(), id)) {
		getchar();
		exit(-1);
	}
}

void Shader::cleanup(){
	glDeleteShader(id);
}

Program::Program(Shader vert, Shader frag):vertex(vert),fragment(frag) {

	GLint result = GL_FALSE;
	int infoLogLength;

	printf("linking program: %d %d\n", vertex.id, fragment.id);
	programID = glCreateProgram();
	glAttachShader(programID, vertex.id);
	glAttachShader(programID, fragment.id);
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 1) {
		vector<char> errormessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &errormessage[0]);
		printf("link error:\n%s\n", &errormessage[0]);
		getchar();
		exit(-1);
	}
	glDetachShader(programID, vertex.id);
	glDetachShader(programID, fragment.id);
}
Program::Program(Shader vert, Shader frag, Shader geom) :vertex(vert), fragment(frag),geometry(geom) {

	GLint result = GL_FALSE;
	int infoLogLength;

	printf("linking program\n");
	programID = glCreateProgram();
	glAttachShader(programID, vertex.id);
	glAttachShader(programID, fragment.id);
	glAttachShader(programID, geometry.id);
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 1) {
		vector<char> errormessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &errormessage[0]);
		printf("link error:\n%s\n", &errormessage[0]);
		getchar();
		exit(-1);
	}
}

Program::~Program() {
}

void Program::cleanup() {
	vertex.cleanup();
	fragment.cleanup();
	geometry.cleanup();
	glDeleteProgram(programID);
}

void Program::setUniform(std::string name, glm::vec3 *v) {
	GLuint id = glGetUniformLocation(programID, name.c_str());
	if (id == -1)return;
	glUniform3fv(id,1, &(*v)[0]);
}

void Program::setUniform(std::string name, glm::mat4 *m) {
	GLuint id = glGetUniformLocation(programID, name.c_str());
	if (id == -1)return;
	glUniformMatrix4fv(id, 1, GL_FALSE, &(*m)[0][0]);
}

void Program::setUniform(std::string name, float f) {
	GLuint id = glGetUniformLocation(programID, name.c_str());
	if (id == -1)return;
	glUniform1f(id, f);
}

void Program::setUniform(std::string name, int i) {
	GLuint id = glGetUniformLocation(programID, name.c_str());
	if (id == -1)return;
	glUniform1i(id, i);
}


void Program::bind() {
	glUseProgram(programID);
}

GLuint Program::getProgramID() {
	return programID;
}
