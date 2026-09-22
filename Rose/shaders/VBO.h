#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>

class VBO
{
public:

	GLuint ID;
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBO(GLfloat* vertics, GLsizeiptr size);

	void Bind();
	void UnBind();
	void Delete();

};

#endif // !VBO_CLASS_H
