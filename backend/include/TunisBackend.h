#ifndef TUNISBACKEND_H
#define TUNISBACKEND_H

#include <cinttypes>
#include <vector>

#include <TunisRenderState.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace tunis
{

class Backend
{
public:
    Backend();
    ~Backend();

    RenderState renderState;

    void clearFrame();

    struct Vertex
    {
        glm::vec2 pos;
        glm::vec2 tcoord;
        glm::vec4 color;
    };

    std::vector<Vertex> vertexBuffer;

    float getTextelSize() const;
    int32_t getMaxTextureSize() const;
    int32_t getTexturePadding() const;

};

}

#endif // TUNISBACKEND_H
