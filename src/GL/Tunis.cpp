
#define TUNIS_GL_IMPLEMENTATION
#include <TunisGL.h>


#include <TunisContextData.h>
tunis::detail::GraphicStates tunis::detail::gfxStates;


#include <Tunis.h>

namespace tunis
{

namespace detail
{

void* poolAlloc( void* userData, uint32_t size )
{
    MemPool* pool = reinterpret_cast<MemPool*>(userData);
    size = (size+0x7) & ~0x7;
    if (pool->size + size < pool->cap)
    {
        unsigned char* ptr = pool->buf + pool->size;
        pool->size += size;
        return ptr;
    }
    fprintf(stderr, "out of mem: %zu < %zu!\n", pool->size + size, pool->cap);
    return nullptr;
}

void poolFree( void* userData, void* ptr )
{
    // empty
    TESS_NOTUSED(userData);
    TESS_NOTUSED(ptr);
}
}

Context::Context()
{
    EASY_FUNCTION(profiler::colors::Coral);

    auto tunisGL_initialized = tunisGLInit();
    if (!tunisGL_initialized)
    {
        abort();
    }

    ctx.mem.resize(1024*1024);
    ctx.pool.size = 0;
    ctx.pool.cap = ctx.mem.size();
    ctx.pool.buf = ctx.mem.data();
    memset(&ctx.ma, 0, sizeof(ctx.ma));
    ctx.ma.memalloc = detail::poolAlloc;
    ctx.ma.memfree = detail::poolFree;
    ctx.ma.userData = &ctx.pool;
    ctx.ma.extraVertices = 256; // realloc not provided, allow 256 extra vertices.

    ctx.tess = tessNewTess(&ctx.ma);
    if (!ctx.tess)
    {
        abort();
    }

    tessSetOption(ctx.tess, TESS_CONSTRAINED_DELAUNAY_TRIANGULATION, 1);

    // default state.
    ctx.states.push_back(*this);

    // Create a default texture atlas.
#ifdef TUNIS_MAX_TEXTURE_SIZE
    detail::gfxStates.maxTexSize = TUNIS_MAX_TEXTURE_SIZE;
#else
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &detail::global.maxTexSize);
#endif
    Texture tex = Texture(detail::gfxStates.maxTexSize, detail::gfxStates.maxTexSize);
    ctx.textures.push_back(tex); // retain

    Paint::reserve(1024);
    Path2D::reserve(1024);
    ctx.batches.reserve(1024);

    ctx.vertexBuffer.reserve(TUNIS_VERTEX_MAX);

    // Initialize our default shader.
    ctx.default2DProgram.init();

    if (tunisGLSupport(GL_VERSION_3_0))
    {
        // Create a dummy vertex array object (mandatory since GL Core profile)
        glGenVertexArrays(1, &ctx.vao);
        glBindVertexArray(ctx.vao);
    }

    // Create vertex and index buffer objects for the batches
    glGenBuffers(2, ctx.buffers);
    glBindBuffer(GL_ARRAY_BUFFER, ctx.buffers[detail::ContextData::VBO]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx.buffers[detail::ContextData::IBO]);

    // Configure our vertex attributes
    glVertexAttribPointer(static_cast<GLuint>(ctx.default2DProgram.a_position), decltype(Vertex::pos)::length(),    GL_FLOAT,          GL_FALSE, sizeof(Vertex), reinterpret_cast<const void *>(0));
    glVertexAttribPointer(static_cast<GLuint>(ctx.default2DProgram.a_texcoord), decltype(Vertex::tcoord)::length(), GL_UNSIGNED_SHORT, GL_TRUE,  sizeof(Vertex), reinterpret_cast<const void *>(sizeof(Vertex::pos)));
    glVertexAttribPointer(static_cast<GLuint>(ctx.default2DProgram.a_color),    decltype(Vertex::color)::length(),  GL_UNSIGNED_BYTE,  GL_TRUE,  sizeof(Vertex), reinterpret_cast<const void *>(sizeof(Vertex::pos) + sizeof(Vertex::tcoord)));
    glEnableVertexAttribArray(static_cast<GLuint>(ctx.default2DProgram.a_position));
    glEnableVertexAttribArray(static_cast<GLuint>(ctx.default2DProgram.a_texcoord));
    glEnableVertexAttribArray(static_cast<GLuint>(ctx.default2DProgram.a_color));

    /* set default state */
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_SCISSOR_TEST);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

}

Context::~Context()
{
    EASY_FUNCTION(profiler::colors::Coral);

    // unload texture data by deleting every potential texture holders.
    ctx.textures.resize(0);
    ctx.batches.resize(0);
    fillStyle.reset();
    strokeStyle.reset();

    // unload shader program
    ctx.default2DProgram.shutdown();

    // unload vertex and index buffers
    glDeleteBuffers(2, ctx.buffers);
    ctx.buffers[detail::ContextData::VBO] = 0;
    ctx.buffers[detail::ContextData::IBO] = 0;

    // reset global states.
    detail::gfxStates = detail::GraphicStates();

    // reset global GL states
    glBindTexture(GL_TEXTURE_2D , 0);
    if (tunisGLSupport(GL_VERSION_3_0))
    {
        glBindVertexArray(0);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // shut down GL wrangler/
    tunisGLShutdown();
}
}
