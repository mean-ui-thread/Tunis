#include <Tunis.h>

#include <easy/profiler.h>

namespace tunis
{

inline Context::Context()
{
    auto tunisGL_initialized = tunisGLInit();
    if (!tunisGL_initialized)
    {
       abort();
    }

    // Create a default texture atlas.
#ifdef TUNIS_MAX_TEXTURE_SIZE
    detail::global.maxTexSize = TUNIS_MAX_TEXTURE_SIZE;
#else
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &detail::global.maxTexSize);
#endif
    Texture tex = Texture(detail::global.maxTexSize, detail::global.maxTexSize);
    data.textures.push_back(tex); // retain

    Paint::reserve(1024);
    Path2D::reserve(1024);
    data.batches.reserve(1024);

    data.vertexBuffer.reserve(TUNIS_VERTEX_MAX);


    data.default2DShader.vert = glCreateShader(GL_VERTEX_SHADER);
    data.default2DShader.frag = glCreateShader(GL_FRAGMENT_SHADER);
    data.default2DShader.program = glCreateProgram();

    GLint vert_len = static_cast<GLint>(strlen(detail::Default2DShader::vertSrc));
    GLint frag_len = static_cast<GLint>(strlen(detail::Default2DShader::fragSrc));

    glShaderSource(data.default2DShader.vert, 1, &detail::Default2DShader::vertSrc, &vert_len);
    glShaderSource(data.default2DShader.frag, 1, &detail::Default2DShader::fragSrc, &frag_len);

    glCompileShader(data.default2DShader.vert);
    glCompileShader(data.default2DShader.frag);

    glAttachShader(data.default2DShader.program, data.default2DShader.vert);
    glAttachShader(data.default2DShader.program, data.default2DShader.frag);

    glLinkProgram(data.default2DShader.program);

    // attribute locations
    data.default2DShader.a_position = glGetAttribLocation(data.default2DShader.program, "a_position");
    data.default2DShader.a_texcoord = glGetAttribLocation(data.default2DShader.program, "a_texcoord");
    data.default2DShader.a_color = glGetAttribLocation(data.default2DShader.program, "a_color");

    // uniform locations
    data.default2DShader.u_viewSize = glGetUniformLocation(data.default2DShader.program, "u_viewSize");
    data.default2DShader.u_texture0 = glGetUniformLocation(data.default2DShader.program, "u_texture0");

    // activate shader program.
    glUseProgram(data.default2DShader.program);
    detail::global.programId = data.default2DShader.program;

    // shader's sampler setup
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(data.default2DShader.u_texture0, 0);

    if (tunisGLSupport(GL_VERSION_3_0))
    {
        // Create a dummy vertex array object (mandatory since GL Core profile)
        glGenVertexArrays(1, &data.vao);
        glBindVertexArray(data.vao);
    }

#if TUNIS_USE_IBO
    // Create static index buffer object.
    uint16_t indices[TUNIS_VERTEX_MAX / 4 * 6];
    for (uint16_t i = 0; i < TUNIS_VERTEX_MAX / 4; ++i)
    {
        indices[i * 6 + 0] = i * 4 + 0;
        indices[i * 6 + 1] = i * 4 + 1;
        indices[i * 6 + 2] = i * 4 + 2;
        indices[i * 6 + 3] = i * 4 + 2;
        indices[i * 6 + 4] = i * 4 + 3;
        indices[i * 6 + 5] = i * 4 + 0;
    }
    glGenBuffers(1, &data.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
#endif

    // Create vertex buffer object for the batches
    glGenBuffers(1, &data.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, data.vbo);
#if TUNIS_USE_IBO
        glBufferData(GL_ARRAY_BUFFER, TUNIS_VERTEX_MAX * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
#endif

    // Configure our vertex attributes
    glVertexAttribPointer(static_cast<GLuint>(data.default2DShader.a_position), decltype(Vertex::pos)::length(),    GL_FLOAT,          GL_FALSE, sizeof(Vertex), reinterpret_cast<const void *>(0));
    glVertexAttribPointer(static_cast<GLuint>(data.default2DShader.a_texcoord), decltype(Vertex::tcoord)::length(), GL_UNSIGNED_SHORT, GL_TRUE,  sizeof(Vertex), reinterpret_cast<const void *>(sizeof(Vertex::pos)));
    glVertexAttribPointer(static_cast<GLuint>(data.default2DShader.a_color),    decltype(Vertex::color)::length(),  GL_UNSIGNED_BYTE,  GL_TRUE,  sizeof(Vertex), reinterpret_cast<const void *>(sizeof(Vertex::pos) + sizeof(Vertex::tcoord)));
    glEnableVertexAttribArray(static_cast<GLuint>(data.default2DShader.a_position));
    glEnableVertexAttribArray(static_cast<GLuint>(data.default2DShader.a_texcoord));
    glEnableVertexAttribArray(static_cast<GLuint>(data.default2DShader.a_color));

    /* set default state */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

inline Context::~Context()
{
    // unload texture data by deleting every potential texture holders.
    data.textures.resize(0);
    data.batches.resize(0);
    fillStyle.reset();
    strokeStyle.reset();

    // reset global states.
    detail::global = detail::GlobalContextData();

    // reset global GL states
    glBindTexture(GL_TEXTURE_2D , 0);
    if (tunisGLSupport(GL_VERSION_3_0))
    {
        glBindVertexArray(0);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
#if TUNIS_USE_IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
    glUseProgram(0);


    // shut down GL wrangler/
    tunisGLShutdown();
}

inline void Context::clearFrame(int fbLeft, int fbTop, int fbWidth, int fbHeight, Color backgroundColor)
{
    EASY_FUNCTION(profiler::colors::Teal);

    // update the clear color if necessary
    if (detail::global.backgroundColor != backgroundColor)
    {
        detail::global.backgroundColor = backgroundColor;

        glClearColor(backgroundColor.r/255.0f,
                     backgroundColor.g/255.0f,
                     backgroundColor.b/255.0f,
                     backgroundColor.a/255.0f);
    }

    // update the viewport if necessary
    Viewport viewport(fbLeft, fbTop, fbWidth, fbHeight);
    if (detail::global.viewport != viewport)
    {
        detail::global.viewport = viewport;

        glViewport(fbLeft, fbTop, fbWidth, fbHeight);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

inline void Context::beginFrame(int winWidth, int winHeight, float devicePixelRatio)
{
    EASY_FUNCTION(profiler::colors::Teal);
    data.batches.resize(0);
    data.vertexBuffer.resize(0);

    glUniform2f(data.default2DShader.u_viewSize,
                static_cast<GLfloat>(winWidth),
                static_cast<GLfloat>(winHeight));
}

inline void Context::endFrame()
{
    EASY_FUNCTION(profiler::colors::Teal);

    if (data.vertexBuffer.size() > 0 && data.batches.size() > 0)
    {
#if TUNIS_USE_IBO
        // flush the vertex batch.
        glBufferSubData(GL_ARRAY_BUFFER, 0,
                        static_cast<GLsizeiptr>(data.vertexBuffer.size() * sizeof(Vertex)),
                        &data.vertexBuffer.front());
#else
        // flush the vertex batch.
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(data.vertexBuffer.size() * sizeof(Vertex)),
                     &data.vertexBuffer.front(),
                     GL_STREAM_DRAW);
#endif

        // render every batches
        for (size_t i = 0; i < data.batches.size(); ++i)
        {
            if (detail::global.programId != data.batches.program(i))
            {
                glUseProgram(data.batches.program(i));
                detail::global.programId = data.batches.program(i);
            }

            data.batches.texture(i).bind();

#if TUNIS_USE_IBO
            glDrawElements(GL_TRIANGLES,
                           data.batches[i].vertexCount() / 4 * 6,
                           GL_UNSIGNED_SHORT,
                           (void*)(data.batches[i].vertexStartOffset() * sizeof(GLushort)));
#else
            glDrawArrays(GL_TRIANGLES,
                         data.batches.vertexStartOffset(i),
                         data.batches.vertexCount(i));
#endif

        }
    }

}

inline void Context::fillRect(float x, float y, float width, float height)
{
    beginPath();
    rect(std::move(x), std::move(y), std::move(width), std::move(height));
    fill(data.currentPath);
}

inline void Context::strokeRect(float x, float y, float width, float height)
{
    beginPath();
    rect(std::move(x), std::move(y), std::move(width), std::move(height));
    stroke(data.currentPath);
}

inline void Context::clearRect(float x, float y, float width, float height)
{
    EASY_FUNCTION(profiler::colors::Teal);

    Path2D path;
    path.rect(std::move(x), std::move(y), std::move(width), std::move(height));

    Paint prevFillStyle = fillStyle;
    fillStyle = detail::global.backgroundColor;
    fill(path);
    fillStyle = prevFillStyle;

}

inline void Context::beginPath()
{
    data.currentPath.reset();
}

inline void Context::closePath()
{
    data.currentPath.closePath();
}

inline void Context::moveTo(float x, float y)
{
    data.currentPath.moveTo(std::move(x), std::move(y));
}

inline void Context::lineTo(float x, float y)
{
    data.currentPath.lineTo(std::move(x), std::move(y));
}

inline void Context::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y)
{
    data.currentPath.bezierCurveTo(std::move(cp1x), std::move(cp1y), std::move(cp2x), std::move(cp2y), std::move(x), std::move(y));
}

inline void Context::quadraticCurveTo(float cpx, float cpy, float x, float y)
{
    data.currentPath.quadraticCurveTo(std::move(cpx), std::move(cpy), std::move(x), std::move(y));
}

inline void Context::arc(float x, float y, float radius, float startAngle, float endAngle, bool anticlockwise)
{
    data.currentPath.arc(std::move(x), std::move(y), std::move(radius), std::move(startAngle), std::move(endAngle), std::move(anticlockwise));
}

inline void Context::arcTo(float x1, float y1, float x2, float y2, float radius)
{
    data.currentPath.arcTo(std::move(x1), std::move(y1), std::move(x2), std::move(y2), std::move(radius));
}

inline void Context::ellipse(float x, float y, float radiusX, float radiusY, float rotation, float startAngle, float endAngle, bool anticlockwise)
{
    data.currentPath.ellipse(std::move(x), std::move(y), std::move(radiusX), std::move(radiusY), std::move(rotation), std::move(startAngle), std::move(endAngle), std::move(anticlockwise));
}

inline void Context::rect(float x, float y, float width, float height)
{
    data.currentPath.rect(std::move(x), std::move(y), std::move(width), std::move(height));
}

inline void Context::fill(FillRule fillRule)
{
    fill(data.currentPath, std::move(fillRule));
}

inline void Context::stroke()
{
    stroke(data.currentPath);
}

inline void Context::fill(Path2D &path, FillRule fillRule)
{
    EASY_FUNCTION(profiler::colors::Teal);

    if (path.dirty())
    {
        // TODO fill path's contour points cache.

        path.dirty() = false;
    }

    if (path.points().size() < 3)
    {
        return; // cannot fill with less than 3 points.

    }

    std::vector<Vertex> vertices;

    // TODO contour points into vertices using fillStyle to populate the u/v coordinates correctly.

    GLint startOffset = static_cast<GLint>(data.vertexBuffer.size());
    GLsizei vertexCount = static_cast<GLsizei>(vertices.size());

    data.vertexBuffer.insert(data.vertexBuffer.end(), vertices.begin(), vertices.end());

    if (data.batches.size() > 0)
    {
        size_t lastBatchId = data.batches.size() - 1;

        if (data.batches.program(lastBatchId) == data.default2DShader.program)
        {
            // the batch may continue
            data.batches.vertexCount(lastBatchId) += vertexCount;
            return;
        }
    }

    // start a new batch. RenderDefault2D can use any textures for now, as long
    // as they have that little white square in them.
    GLuint program = data.default2DShader.program;
    Texture tex = data.textures.back();
    data.batches.push(std::move(program),
                      std::move(tex),
                      std::move(startOffset),
                      std::move(vertexCount));


}

inline void Context::stroke(Path2D &path)
{
    EASY_FUNCTION(profiler::colors::Teal);

    if (path.dirty())
    {
        // TODO fill path's contour points cache.

        path.dirty() = false;
    }

    if (path.points().size() < 3)
    {
        return; // cannot fill with less than 3 points.

    }

    std::vector<Vertex> vertices;

    // TODO contour points into vertices using trokeStyle to populate the u/v coordinates correctly.

    GLint startOffset = static_cast<GLint>(data.vertexBuffer.size());
    GLsizei vertexCount = static_cast<GLsizei>(vertices.size());

    data.vertexBuffer.insert(data.vertexBuffer.end(), vertices.begin(), vertices.end());

    if (data.batches.size() > 0)
    {
        size_t lastBatchId = data.batches.size() - 1;

        if (data.batches.program(lastBatchId) == data.default2DShader.program)
        {
            // the batch may continue
            data.batches.vertexCount(lastBatchId) += vertexCount;
            return;
        }
    }

    // start a new batch. RenderDefault2D can use any textures for now, as long
    // as they have that little white square in them.
    GLuint program = data.default2DShader.program;
    Texture tex = data.textures.back();
    data.batches.push(std::move(program),
                      std::move(tex),
                      std::move(startOffset),
                      std::move(vertexCount));
}


}

