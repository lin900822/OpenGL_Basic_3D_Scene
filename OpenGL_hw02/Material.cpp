#include "Material.h"

Material::Material(Shader* shader, glm::vec3 ambient, unsigned int diffuse, unsigned int specular, float shininess)
{
	this->shader = shader;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
}
