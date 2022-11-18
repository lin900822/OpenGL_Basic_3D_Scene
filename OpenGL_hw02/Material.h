#pragma once

#include "Shader.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Material
{
public:
	Shader* shader;
	glm::vec3 ambient;
	unsigned int diffuse;
	unsigned int specular;
	float shininess;

	Material(Shader* shader, glm::vec3 ambient, unsigned int diffuse, unsigned int specular, float shininess);
};

