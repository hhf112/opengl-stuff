// #define DEBUG

#include <parkour/hitbox.hpp>
#include <parkour/mesh.hpp>

#define HITBOX std::cerr << "HITBOX VERTICES:------------------------------------------\n";\
    for (auto &x : hitbox.vertices)\
    {\
        const auto &pos = x.Position;\
        std::cerr << pos.x << ',' << pos.y << ',' << pos.z << '\n';\
    }\
    std::cerr << "-----------------------------------------------------\n";\

Mesh HitboxMeshConstructor::createMeshFromHitbox(Hitbox &hitbox)
{
    float h = hitbox.getHeight(), w = hitbox.getWidth(), d = hitbox.getDepth();
    glm::vec3 hw(w / 2.0f, h / 2.0f, d / 2.0f);

    std::vector<unsigned int> indices = {
        0, 1, 1, 2, 2, 3, 3, 0, // back face
        4, 5, 5, 6, 6, 7, 7, 4, // front face
        0, 4, 1, 5, 2, 6, 3, 7  // connecting edges
    };

#ifdef DEBUG
	HITBOX
#endif

    return Mesh(hitbox.getVertices(), indices, {});
}
