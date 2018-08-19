#ifndef TUNISBACKEND_H
#define TUNISBACKEND_H

#include <cinttypes>
#include <memory>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "soa.h"

#include "TunisTexture.h"

namespace tunis
{

using Position = glm::tvec2<float>;
using TCoord = glm::tvec2<uint16_t>;
using Color = glm::tvec4<uint8_t>;

struct BackendData;

class Backend
{
public:
    Backend();
    ~Backend();

    Backend(const Backend &other) = delete;
    Backend& operator=(const Backend &other) = delete;

    void setClearColor(const Color &color);
    void setViewport(const glm::ivec4 &rect);
    void bindTexture(const Texture &tex);

    void clearFrame();

    struct Vertex
    {
        Position pos;
        TCoord   tcoord;
        Color    color;
    };

    std::vector<Vertex> vertexBuffer;

    void reset();
    void flushVertexBuffer();

    void render(size_t vertexStartIndex, size_t vertexCount);

    float getTextelSize() const;
    int32_t getMaxTextureSize() const;
    int32_t getTexturePadding() const;

private:

    Color m_clearColor;
    glm::ivec4 m_viewport;
    Texture m_texture;

    std::unique_ptr<BackendData> m_data;

};

}

#endif // TUNISBACKEND_H
