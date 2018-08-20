#include <Tunis.h>

#include "TunisBackend.h"
#include "soa.h"

using namespace tunis;

Context::Context() :
    m_pBackend(new Backend())
{
    // Create a default texture atlas.
    int32_t maxTexSize = m_pBackend->getMaxTextureSize();
    Texture tex = Texture(maxTexSize, maxTexSize);
    m_textures.push_back(tex); // retain

    m_pBackend->bindTexture(tex);
    setBackgroundColor(color::White);

    m_batches.reserve(1024);
}

Context::~Context()
{
    m_textures.clear();
    m_pBackend->reset();
}

void Context::setBackgroundColor(const Color &color)
{
    m_pBackend->setClearColor(color);
}

void Context::beginFrame(int32_t x, int32_t y, int32_t w, int32_t h)
{
    m_batches.resize(0);
    m_pBackend->vertexBuffer.resize(0);

    m_pBackend->setViewport(glm::ivec4(x, y, w, h));
    m_pBackend->clearFrame();
}

void Context::endFrame()
{
    m_pBackend->flushVertexBuffer();

    // render every batches
    for (size_t i = 0; i < m_batches.size(); ++i)
    {
        m_pBackend->bindTexture(m_batches.get<_texture>(i));

        m_pBackend->render(m_batches.get<_vertexStartOffset>(i),
                           m_batches.get<_vertexCount>(i));
    }
}

void Context::fillRect(float x, float y, float width, float height)
{
    pushColorRect(x, y, width, height, fillStyle.getInnerColor());
}

void Context::clearRect(float x, float y, float width, float height)
{
    pushColorRect(x, y, width, height, m_pBackend->getClearColor());
}

void Context::strokeRect(float x, float y, float width, float height)
{
    float hlw = lineWidth * 0.5f;

    // top line
    pushColorRect(x-hlw, y-hlw, width+lineWidth, lineWidth, strokeStyle.getInnerColor());

    // bottom line
    pushColorRect(x-hlw, y+height-hlw, width+lineWidth, lineWidth, strokeStyle.getInnerColor());

    // left line
    pushColorRect(x-hlw, y+hlw, lineWidth, height-lineWidth, strokeStyle.getInnerColor());

    // right line
    pushColorRect(x+width-hlw, y+hlw, lineWidth, height-lineWidth, strokeStyle.getInnerColor());
}

void Context::pushColorRect(float x, float y, float width, float height, const Color &color)
{
    size_t i = m_pBackend->vertexBuffer.size();

    // allocate room for 6 new vertices for a pair of triangles.
    m_pBackend->vertexBuffer.resize(i + 6);

    // ---- FIRST TRIANGLE ---

    // top left
    m_pBackend->vertexBuffer[i].pos.x = x;
    m_pBackend->vertexBuffer[i].pos.y = y;
    m_pBackend->vertexBuffer[i].tcoord.s = 0;
    m_pBackend->vertexBuffer[i].tcoord.t = 0;
    m_pBackend->vertexBuffer[i].color = color;
    ++i;

    // bottom left
    m_pBackend->vertexBuffer[i].pos.x = x;
    m_pBackend->vertexBuffer[i].pos.y = y + height;
    m_pBackend->vertexBuffer[i].tcoord.s = 0;
    m_pBackend->vertexBuffer[i].tcoord.t = 1;
    m_pBackend->vertexBuffer[i].color = color;
    ++i;

    // bottom right
    m_pBackend->vertexBuffer[i].pos.x = x + width;
    m_pBackend->vertexBuffer[i].pos.y = y + height;
    m_pBackend->vertexBuffer[i].tcoord.s = 1;
    m_pBackend->vertexBuffer[i].tcoord.t = 1;
    m_pBackend->vertexBuffer[i].color = color;
    ++i;

    // ---- FIRST TRIANGLE ---

    // bottom right
    m_pBackend->vertexBuffer[i].pos.x = x + width;
    m_pBackend->vertexBuffer[i].pos.y = y + height;
    m_pBackend->vertexBuffer[i].tcoord.s = 1;
    m_pBackend->vertexBuffer[i].tcoord.t = 1;
    m_pBackend->vertexBuffer[i].color = color;
    ++i;

    // top right
    m_pBackend->vertexBuffer[i].pos.x = x + width;
    m_pBackend->vertexBuffer[i].pos.y = y;
    m_pBackend->vertexBuffer[i].tcoord.s = 1;
    m_pBackend->vertexBuffer[i].tcoord.t = 0;
    m_pBackend->vertexBuffer[i].color = color;
    ++i;

    // top left
    m_pBackend->vertexBuffer[i].pos.x = x;
    m_pBackend->vertexBuffer[i].pos.y = y;
    m_pBackend->vertexBuffer[i].tcoord.s = 0;
    m_pBackend->vertexBuffer[i].tcoord.t = 0;
    m_pBackend->vertexBuffer[i].color = color;
    ++i;

    if (m_batches.size() > 0)
    {
        size_t lastBatchId = m_batches.size() - 1;

        if (m_batches.get<_renderType>(lastBatchId) == RenderDefault2D)
        {
            // the batch may continue
            m_batches.get<_vertexCount>(lastBatchId) += 6;
            return;
        }
    }

    // start a new batch. RenderDefault2D can use any textures for now, as long
    // as they have that little white square in them.
    Texture tex = m_textures.back();
    m_batches.push(RenderDefault2D, std::move(tex), m_batches.size(), 6);
}


