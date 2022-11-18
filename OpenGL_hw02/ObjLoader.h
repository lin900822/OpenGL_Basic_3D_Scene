#pragma once

#include <vector>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class ObjLoader
{
public:
    bool loadOBJ(
        const char* path,
        std::vector < float >& out_model
    );
};

