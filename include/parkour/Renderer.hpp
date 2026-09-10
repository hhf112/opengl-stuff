#include "parkour/shader.hpp"
#include <parkour/Renderable.hpp>
#include <vector>

class Renderer
{
  public:
    Shader boundary_shader;
    std::vector<Shader> globalShaders;
    enum ShaderIndex
    {
        MeshDefault,
        LineShader,
    };

    void Draw(Renderable &Renderable);
};
