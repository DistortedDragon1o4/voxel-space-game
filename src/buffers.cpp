#include "../include/buffers.h"

void EBO::Gen(std::vector<GLuint> indices) {
   	glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
}

void EBO::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind() {
  	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete() {
    glDeleteBuffers(1, &ID);
}

void VBO::Gen(std::vector<short> vertices) {
   	glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(short), &vertices[0], GL_DYNAMIC_DRAW);
}

void VBO::Gen(std::vector<float> vertices) {
   	glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
}

void VBO::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() {
  	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
    glDeleteBuffers(1, &ID);
}

void SSBO::Gen(std::vector<int> data, int bindIndex) {
   	glGenBuffers(1, &ID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(short), &data[0], GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindIndex, ID);
}

void SSBO::Bind() {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
}

void SSBO::Unbind() {
  	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void SSBO::Delete() {
    glDeleteBuffers(1, &ID);
}