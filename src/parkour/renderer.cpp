#pragma once

#include "parkour/Renderer.hpp"

void Renderer::Draw(Renderable &renderable)
{
    if (renderable.renderOpts.show_boundaries)
    {
        Shader &lineShader = globalShaders[LineShader];
        lineShader.use();
        lineShader.setMat4("model", renderable.getTransform());
        lineShader.setMat4("view", renderable.renderOpts.view);
        lineShader.setMat4("projection", renderable.renderOpts.projection);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
        for (auto &mesh : renderable.getLineMeshes())
            mesh.Draw(lineShader, GL_LINES);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    renderable.renderOpts.shader->use();
    renderable.renderOpts.shader->setMat4("model", renderable.getTransform());
    renderable.renderOpts.shader->setMat4("view", renderable.renderOpts.view);
    renderable.renderOpts.shader->setMat4("projection", renderable.renderOpts.projection);

    renderable.getModel()->Draw(*renderable.renderOpts.shader, renderable.renderOpts.format);
}
