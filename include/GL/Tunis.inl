#include <Tunis.h>

namespace tunis
{

inline Context::Context()
{
    EASY_FUNCTION(profiler::colors::Coral);

    auto tunisGL_initialized = tunisGLInit();
    if (!tunisGL_initialized)
    {
        abort();
    }

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

inline Context::~Context()
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

inline void Context::clearFrame(int32_t fbLeft, int32_t fbTop, int32_t fbWidth, int32_t fbHeight, Color backgroundColor)
{
    EASY_FUNCTION(profiler::colors::Coral);

    // update the clear color if necessary
    if (detail::gfxStates.backgroundColor != backgroundColor)
    {
        glClearColor(backgroundColor.r/255.0f,
                     backgroundColor.g/255.0f,
                     backgroundColor.b/255.0f,
                     backgroundColor.a/255.0f);

        detail::gfxStates.backgroundColor = backgroundColor;
    }

    // update the viewport if necessary
    Viewport viewport(fbLeft, fbTop, fbWidth, fbHeight);
    if (detail::gfxStates.viewport != viewport)
    {
        glViewport(fbLeft, fbTop, fbWidth, fbHeight);
        detail::gfxStates.viewport = viewport;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

inline void Context::beginFrame(int32_t winWidth, int32_t winHeight, float devicePixelRatio)
{
    EASY_FUNCTION(profiler::colors::Coral);

    // reset the states.
    ctx.states.resize(0);
    *static_cast<ContextState*>(this) = detail::ContextState();

    ctx.renderViewport(std::move(winWidth), std::move(winHeight), std::move(devicePixelRatio));
}

inline void Context::endFrame()
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.renderFlush();
}

inline void Context::save()
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.states.push_back(*this);
}

inline void Context::restore()
{
    EASY_FUNCTION(profiler::colors::Coral);
    if (ctx.states.size() > 0)
    {
        *static_cast<ContextState*>(this) = ctx.states.back();
        ctx.states.pop_back();
    }
}

inline void Context::fillRect(float x, float y, float width, float height)
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.pushColorRect(std::move(x), std::move(y), std::move(width), std::move(height), fillStyle.innerColor());
}

inline void Context::strokeRect(float x, float y, float width, float height)
{
    EASY_FUNCTION(profiler::colors::Coral);

    // helps to get the lines a bit sharper when the line width is not divisible by two.
    float offset = fmodf(lineWidth, 2.0f) / 2.0f;
    x+=offset;
    y+=offset;

    float hlw = lineWidth * 0.5f;

    // top line
    ctx.pushColorRect(x-hlw, y-hlw, width+lineWidth-1, lineWidth, strokeStyle.innerColor());

    // bottom line
    ctx.pushColorRect(x-hlw, y+height-hlw, width+lineWidth-1, lineWidth, strokeStyle.innerColor());

    // left line
    ctx.pushColorRect(x-hlw, y+hlw, lineWidth, height-lineWidth, strokeStyle.innerColor());

    // right line
    ctx.pushColorRect(x+width-hlw-1, y+hlw, lineWidth, height-lineWidth, strokeStyle.innerColor());
}

inline void Context::clearRect(float x, float y, float width, float height)
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.pushColorRect(std::move(x), std::move(y), std::move(width), std::move(height), detail::gfxStates.backgroundColor);
}

inline void Context::beginPath()
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.currentPath.reset();
}

inline void Context::closePath()
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.currentPath.closePath();
}

inline void Context::moveTo(float x, float y)
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.currentPath.moveTo(std::move(x), std::move(y));
}

inline void Context::lineTo(float x, float y)
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.currentPath.lineTo(std::move(x), std::move(y));
}

inline void Context::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y)
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.currentPath.bezierCurveTo(std::move(cp1x), std::move(cp1y), std::move(cp2x), std::move(cp2y), std::move(x), std::move(y));
}

inline void Context::quadraticCurveTo(float cpx, float cpy, float x, float y)
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.currentPath.quadraticCurveTo(std::move(cpx), std::move(cpy), std::move(x), std::move(y));
}

inline void Context::arc(float x, float y, float radius, float startAngle, float endAngle, bool anticlockwise)
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.currentPath.arc(std::move(x), std::move(y), std::move(radius), std::move(startAngle), std::move(endAngle), std::move(anticlockwise));
}

inline void Context::arcTo(float x1, float y1, float x2, float y2, float radius)
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.currentPath.arcTo(std::move(x1), std::move(y1), std::move(x2), std::move(y2), std::move(radius));
}

inline void Context::ellipse(float x, float y, float radiusX, float radiusY, float rotation, float startAngle, float endAngle, bool anticlockwise)
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.currentPath.ellipse(std::move(x), std::move(y), std::move(radiusX), std::move(radiusY), std::move(rotation), std::move(startAngle), std::move(endAngle), std::move(anticlockwise));
}

inline void Context::rect(float x, float y, float width, float height)
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.currentPath.rect(std::move(x), std::move(y), std::move(width), std::move(height));
}

inline void Context::fill(Fill fillRule)
{
    EASY_FUNCTION(profiler::colors::Coral);
    fill(ctx.currentPath, std::move(fillRule));
}

inline void Context::stroke()
{
    EASY_FUNCTION(profiler::colors::Coral);
    stroke(ctx.currentPath);
}

inline void Context::fill(Path2D &path, Fill fillRule)
{
    EASY_FUNCTION(profiler::colors::Coral);

    ctx.flattenPaths(path, fillRule);

    //ctx.calculateJoins(path, lineJoin, miterLimit);

    // Calculate max vertex usage.
    size_t cverts = 0;
    for (size_t i = 0; i < path.subpaths().size(); ++i)
    {
        cverts += path.subpaths().points(i).size() + path.subpaths().nbevel(i);
    }

    ctx.addBatch(&ctx.default2DProgram,
                 ctx.textures.back(),
                 static_cast<GLuint>(ctx.indexBuffer.size()),
                 static_cast<GLsizei>(cverts));

    size_t dst = ctx.vertexBuffer.size();
    ctx.vertexBuffer.resize(ctx.vertexBuffer.size() + cverts);

    float rangeX = path.boundRight() - path.boundLeft();
    float rangeY = path.boundBottom() - path.boundTop();

    for (size_t i = 0; i < path.subpaths().size(); i++)
    {
        auto &pts = path.subpaths().points(i);

        for (size_t j = 0; j < pts.size(); ++j)
        {
            ctx.vertexBuffer[dst].pos.x = pts.x(j);
            ctx.vertexBuffer[dst].pos.y = pts.y(j);
            ctx.vertexBuffer[dst].tcoord.s = static_cast<uint16_t>(((((pts.x(j) - path.boundLeft()) / rangeX) * 16.0f) / detail::gfxStates.maxTexSize) * 0xFFFF);
            ctx.vertexBuffer[dst].tcoord.t = static_cast<uint16_t>(((((pts.y(j) - path.boundTop()) / rangeY) * 16.0f) / detail::gfxStates.maxTexSize) * 0xFFFF);
            ctx.vertexBuffer[dst].color = fillStyle.innerColor();
            ctx.indexBuffer.push_back(static_cast<uint16_t>(dst));
            ++dst;
        }
    }


}

inline void Context::stroke(Path2D &path)
{
    EASY_FUNCTION(profiler::colors::Coral);

    if (path.dirty())
    {
        // TODO fill path's contour points cache.

        path.dirty() = false;
    }

    if (path.subpaths().size() < 3)
    {
        return; // cannot fill with less than 3 points.

    }

    std::vector<Vertex> vertices;

    // TODO contour points into vertices using trokeStyle to populate the u/v coordinates correctly.

    ctx.addBatch(&ctx.default2DProgram,
                 ctx.textures.back(),
                 static_cast<GLuint>(ctx.vertexBuffer.size()),
                 static_cast<GLsizei>(vertices.size()));

    ctx.vertexBuffer.insert(ctx.vertexBuffer.end(), vertices.begin(), vertices.end());

}


}

