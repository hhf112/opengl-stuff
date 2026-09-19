#pragma once
#include "parkour/shader.hpp"
#include <parkour/Renderable.hpp>

class Renderer
{
  public:
    Shader boundary_shader;
	glm::mat4 view;
	glm::mat4 projection;

    void Draw(Renderable &Renderable);
};
