#include "texture.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint texture(const std::string& filename)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = nullptr;

	std::string texPaths[] =
	{
				filename,
		"Rose/" + filename,
		"../" + filename,
		"../Rose/" + filename,
		"shaders/../" + filename
	};

	for (const std::string& p: texPaths)
	{
		data = stbi_load(p.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			std::cout << "Loaded texture: " << p << " " << width << "x" << height << " ch=" << nrChannels << std::endl;
			break;
		}
	}

	if (data)
	{
		GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}
	else
	{
		std::cout << "Failed to load texture: " << stbi_failure_reason() << std::endl;
		glDeleteTextures(1, &texture);
		return 0;
	}
	stbi_image_free(data);

	return texture;
}