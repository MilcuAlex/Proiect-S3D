#pragma once
#pragma once

#include <glm.hpp>
#include <GL/glew.h>

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLint diffuseTex;
	GLint speculatTex;
};