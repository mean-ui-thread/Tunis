#include <Tunis.h>

#include <easy/profiler.h>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace tunis
{

inline Context::Context() :
    data(this)
{
}

inline Context::~Context()
{
}

inline void Context::clearFrame(int fbLeft, int fbTop, int fbWidth, int fbHeight, Color backgroundColor)
{
    EASY_FUNCTION(profiler::colors::Teal);

    // update the clear color if necessary
    if (detail::globalContextData.backgroundColor != backgroundColor)
    {
        detail::globalContextData.backgroundColor = backgroundColor;

        glClearColor(backgroundColor.r/255.0f,
                     backgroundColor.g/255.0f,
                     backgroundColor.b/255.0f,
                     backgroundColor.a/255.0f);
    }

    // update the viewport if necessary
    Viewport viewport(fbLeft, fbTop, fbWidth, fbHeight);
    if (detail::globalContextData.viewport != viewport)
    {
        detail::globalContextData.viewport = viewport;

        glViewport(fbLeft, fbTop, fbWidth, fbHeight);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

inline void Context::beginFrame(int winWidth, int winHeight, double devicePixelRatio)
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
    if (data.vertexBuffer.size() > 0)
    {
#if TUNIS_USE_IBO
        glBufferSubData(GL_ARRAY_BUFFER, 0,
                        static_cast<GLsizeiptr>(data.vertexBuffer.size() * sizeof(Vertex)),
                        &data.vertexBuffer.front());
#else
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(data.vertexBuffer.size() * sizeof(Vertex)),
                     &data.vertexBuffer.front(),
                     GL_STREAM_DRAW);
#endif
    }

    // render every batches
    for (size_t i = 0; i < data.batches.size(); ++i)
    {
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

inline void Context::fillRect(double x, double y, double width, double height)
{
    beginPath();
    rect(x, y, width, height);
    fill(data.currentPath);
}

inline void Context::strokeRect(double x, double y, double width, double height)
{
    beginPath();
    rect(x, y, width, height);
    stroke(data.currentPath);
}

inline void Context::clearRect(double x, double y, double width, double height)
{
    EASY_FUNCTION(profiler::colors::Teal);

    Path2D path;
    path.rect(x, y, width, height);

    Paint prevFillStyle = fillStyle;
    fillStyle = detail::globalContextData.backgroundColor;
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

inline void Context::moveTo(double x, double y)
{
    data.currentPath.moveTo(x, y);
}

inline void Context::lineTo(double x, double y)
{
    data.currentPath.lineTo(x, y);
}

inline void Context::bezierCurveTo(double cp1x, double cp1y, double cp2x, double cp2y, double x, double y)
{
    data.currentPath.bezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y);
}

inline void Context::quadraticCurveTo(double cpx, double cpy, double x, double y)
{
    data.currentPath.quadraticCurveTo(cpx, cpy, x, y);
}

inline void Context::arc(double x, double y, double radius, double startAngle, double endAngle, bool anticlockwise)
{
    data.currentPath.arc(x, y, radius, startAngle, endAngle, anticlockwise);
}

inline void Context::arcTo(double x1, double y1, double x2, double y2, double radius)
{
    data.currentPath.arcTo(x1, y1, x2, y2, radius);
}

inline void Context::ellipse(double x, double y, double radiusX, double radiusY, double rotation, double startAngle, double endAngle, bool anticlockwise)
{
    data.currentPath.ellipse(x, y, radiusX, radiusY, rotation, startAngle, endAngle, anticlockwise);
}

inline void Context::rect(double x, double y, double width, double height)
{
    data.currentPath.rect(x, y, width, height);
}

inline void Context::fill(FillRule fillRule)
{
    fill(data.currentPath, fillRule);
}

inline void Context::stroke()
{
    stroke(data.currentPath);
}

inline void Context::fill(Path2D &path, FillRule fillRule)
{
    EASY_FUNCTION(profiler::colors::Teal);

    if (path.points().size() == 0)
    {
        return;
    }

    if (path.vertexCache().size() == 0)
    {
        EASY_BLOCK("GLU");
        detail::globalContextData.tessPath = path;

        switch(fillRule)
        {
        case evenodd:
            gluTessProperty(data.tesselator, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD );
            break;
        case nonzero:
            gluTessProperty(data.tesselator, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO );
            break;
        }
        gluTessProperty(data.tesselator, GLU_TESS_TOLERANCE, TUNIS_TESS_TOLERANCE);

        gluTessBeginPolygon(data.tesselator, &data);

        size_t pointId = 0;
        int contourCount = 0;
        for (auto cmd : path.commands())
        {
            switch (cmd)
            {
            case detail::MOVE_TO:
                if (contourCount)
                {
                    gluTessEndContour(data.tesselator);
                    contourCount--;
                }

                gluTessBeginContour(data.tesselator);
                contourCount++;

                {
                    GLdouble *v = glm::value_ptr(path.points()[pointId]);
                    gluTessVertex(data.tesselator, v, v);
                }
                ++pointId;
                break;
            case detail::LINE_TO:
                {
                    GLdouble *v = glm::value_ptr(path.points()[pointId]);
                    gluTessVertex(data.tesselator, v, v);
                }
                ++pointId;
                break;
            case detail::CLOSE:
                if (contourCount)
                {
                    gluTessEndContour(data.tesselator);
                    contourCount--;
                }

                break;

            default:
                break;
            }
        }

        while (contourCount)
        {
            gluTessEndContour(data.tesselator);
            contourCount--;
        }


        gluTessEndPolygon(data.tesselator);
    }

    if (path.vertexCache().size() == 0)
    {
        return;
    }

    GLint startOffset = static_cast<GLint>(data.vertexBuffer.size());
    GLsizei vertexCount = static_cast<GLsizei>(path.vertexCache().size());
    std::copy(path.vertexCache().begin(), path.vertexCache().end(), std::back_inserter(data.vertexBuffer));

    if (data.batches.size() > 0)
    {
        size_t lastBatchId = data.batches.size() - 1;

        if (data.batches.renderType(lastBatchId) == detail::Default2D)
        {
            // the batch may continue
            data.batches.vertexCount(lastBatchId) += vertexCount;
            return;
        }
    }

    // start a new batch. RenderDefault2D can use any textures for now, as long
    // as they have that little white square in them.
    Texture tex = data.textures.back();
    data.batches.push(detail::Default2D,
                      std::move(tex),
                      std::move(startOffset),
                      std::move(vertexCount));


}

inline void Context::stroke(Path2D &path)
{
    EASY_FUNCTION(profiler::colors::Teal);

    if (path.points().size() == 0)
    {
        return;
    }

    if (path.vertexCache().size() == 0)
    {
        Point coords(0);
        Point prev(0);
        Point closeTo(0);

        size_t pointId = 0;
        for (auto cmd : path.commands())
        {
            switch (cmd)
            {
            case detail::MOVE_TO:
                prev.x = closeTo.x = coords.x = path.points()[pointId].x;
                prev.y = closeTo.y = coords.y = path.points()[pointId].y;
                ++pointId;
                break;
            case detail::LINE_TO:
                prev.x = coords.x;
                prev.y = coords.y;
                coords.x = path.points()[pointId].x;
                coords.y = path.points()[pointId].y;
                ++pointId;
                data.addLineSegment(path, prev, coords, static_cast<double>(lineWidth));
                break;
            case detail::CLOSE:
                data.addLineSegment(path, coords, closeTo, static_cast<double>(lineWidth));
                break;

            default:
                break;
            }
        }

        gluTessEndPolygon(data.tesselator);
    }

    if (path.vertexCache().size() == 0)
    {
        return;
    }

    GLint startOffset = static_cast<GLint>(data.vertexBuffer.size());
    GLsizei vertexCount = static_cast<GLsizei>(path.vertexCache().size());
    std::copy(path.vertexCache().begin(), path.vertexCache().end(), std::back_inserter(data.vertexBuffer));

    if (data.batches.size() > 0)
    {
        size_t lastBatchId = data.batches.size() - 1;

        if (data.batches.renderType(lastBatchId) == detail::Default2D)
        {
            // the batch may continue
            data.batches.vertexCount(lastBatchId) += vertexCount;
            return;
        }
    }

    // start a new batch. RenderDefault2D can use any textures for now, as long
    // as they have that little white square in them.
    Texture tex = data.textures.back();
    data.batches.push(detail::Default2D,
                      std::move(tex),
                      std::move(startOffset),
                      std::move(vertexCount));
}

namespace detail
{

inline ContextData::ContextData(Context *pub) :
    pub(pub)
{
    auto tunisGL_initialized = tunisGLInit();
    if (!tunisGL_initialized)
    {
       abort();
    }

    // Create a default texture atlas.
#ifdef TUNIS_MAX_TEXTURE_SIZE
    globalContextData.maxTexSize = TUNIS_MAX_TEXTURE_SIZE;
#else
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &globalContextData.maxTexSize);
#endif
    Texture tex = Texture(globalContextData.maxTexSize, globalContextData.maxTexSize);
    textures.push_back(tex); // retain

    Paint::reserve(512);
    Path2D::reserve(1024);
    batches.reserve(1024);

    vertexBuffer.reserve(TUNIS_VERTEX_MAX);


    default2DShader.vert = glCreateShader(GL_VERTEX_SHADER);
    default2DShader.frag = glCreateShader(GL_FRAGMENT_SHADER);
    default2DShader.program = glCreateProgram();

    GLint vert_len = static_cast<GLint>(strlen(Default2DShader::vertSrc));
    GLint frag_len = static_cast<GLint>(strlen(Default2DShader::fragSrc));

    glShaderSource(default2DShader.vert, 1, &Default2DShader::vertSrc, &vert_len);
    glShaderSource(default2DShader.frag, 1, &Default2DShader::fragSrc, &frag_len);

    glCompileShader(default2DShader.vert);
    glCompileShader(default2DShader.frag);

    glAttachShader(default2DShader.program, default2DShader.vert);
    glAttachShader(default2DShader.program, default2DShader.frag);

    glLinkProgram(default2DShader.program);

    // attribute locations
    default2DShader.a_position = glGetAttribLocation(default2DShader.program, "a_position");
    default2DShader.a_texcoord = glGetAttribLocation(default2DShader.program, "a_texcoord");
    default2DShader.a_color = glGetAttribLocation(default2DShader.program, "a_color");

    // uniform locations
    default2DShader.u_viewSize = glGetUniformLocation(default2DShader.program, "u_viewSize");
    default2DShader.u_texture0 = glGetUniformLocation(default2DShader.program, "u_texture0");

    // activate shader program.
    glUseProgram(default2DShader.program);
    glUniform1i(default2DShader.u_texture0, 0);
    glActiveTexture(GL_TEXTURE0);

    // Create a vertex array object
    if (tunisGLSupport(GL_VERSION_3_0) || tunisGLSupport(GL_ES_VERSION_3_0))
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
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
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
#endif

    // Create dynamic vertex buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
#if TUNIS_USE_IBO
        glBufferData(GL_ARRAY_BUFFER, TUNIS_VERTEX_MAX * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
#endif

    glVertexAttribPointer(static_cast<GLuint>(default2DShader.a_position), decltype(Vertex::pos)::length(),    GL_FLOAT,          GL_FALSE, sizeof(Vertex), reinterpret_cast<const void *>(0));
    glVertexAttribPointer(static_cast<GLuint>(default2DShader.a_texcoord), decltype(Vertex::tcoord)::length(), GL_UNSIGNED_SHORT, GL_TRUE,  sizeof(Vertex), reinterpret_cast<const void *>(sizeof(Vertex::pos)));
    glVertexAttribPointer(static_cast<GLuint>(default2DShader.a_color),    decltype(Vertex::color)::length(),  GL_UNSIGNED_BYTE,  GL_TRUE,  sizeof(Vertex), reinterpret_cast<const void *>(sizeof(Vertex::pos) + sizeof(Vertex::tcoord)));
    glEnableVertexAttribArray(static_cast<GLuint>(default2DShader.a_position));
    glEnableVertexAttribArray(static_cast<GLuint>(default2DShader.a_texcoord));
    glEnableVertexAttribArray(static_cast<GLuint>(default2DShader.a_color));


    /* set default state */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    tesselator = gluNewTess();

    gluTessCallback(tesselator, GLU_TESS_VERTEX_DATA, reinterpret_cast<GLUfuncptr>(tessVertex));
    gluTessCallback(tesselator, GLU_TESS_COMBINE_DATA, reinterpret_cast<GLUfuncptr>(tessCombine));
    gluTessCallback(tesselator, GLU_TESS_EDGE_FLAG_DATA, reinterpret_cast<GLUfuncptr>(tessEdgeFlag));
}

inline ContextData::~ContextData()
{
    globalContextData.textureId = 0;
    glBindTexture(GL_TEXTURE_2D , 0);
    textures.clear();

    gluDeleteTess(tesselator);

    pub = nullptr;
}

inline void ContextData::tessEdgeFlag(void)
{
    /*
     * providing this empty callback to GLU_TESS_EDGE_FLAG_DATA property will
     * prevent GL_TRIANGLE_FAN and GL_TRIANGLE_STRIP from happening.
     */
}

inline void ContextData::tessVertex( GLvoid* d, GLvoid* user )
{
    Point &pos = *reinterpret_cast<Point*>(d);
    ContextData &data = *reinterpret_cast<ContextData*>(user);
    Path2D &path = globalContextData.tessPath;

    double rangeX = path.maxBoundX() - path.minBoundX();
    double rangeY = path.maxBoundY() - path.minBoundY();

    uint16_t u = static_cast<uint16_t>(((pos.x - path.minBoundX()) / rangeX * 16.0) / globalContextData.maxTexSize * 0xFFFF);
    uint16_t v = static_cast<uint16_t>(((pos.y - path.minBoundY()) / rangeY * 16.0) / globalContextData.maxTexSize * 0xFFFF);

    path.vertexCache().push_back({
                                     { static_cast<float>(pos.x), static_cast<float>(pos.y) },
                                     { u, v },
                                     data.pub->fillStyle.innerColor()
                                 });
}

inline void ContextData::tessCombine(GLvoid* data, void *vertex_data[4],
                                     GLfloat weight[4], void **outData,
                                     void *user)
{
    Point &newPoint = *reinterpret_cast<Point*>(data);
    Path2D &path = globalContextData.tessPath;
    path.points().push_back(newPoint);
    *outData = reinterpret_cast<void*>(&path.points().back());
}

inline void ContextData::addLineSegment(Path2D &path, const Point& p0, const Point& p1, const double lineWidth )
{
    EASY_FUNCTION(profiler::colors::Teal);

    if ( glm::all(glm::epsilonEqual(p0, p1, glm::epsilon<double>())) )
    {
        return;
    }


    double dx = p1.y - p0.y;
    double dy = p0.x - p1.x;
    const double inv_mag = 1.0 / glm::sqrt(dx*dx + dy*dy);
    dx = dx * inv_mag;
    dy = dy * inv_mag;

    uint16_t u, v;
    const double radius = lineWidth * 0.5;

    double pos0X = p0.x - radius * dx;
    double pos0Y = p0.y - radius * dy;
    double pos1X = p0.x + radius * dx;
    double pos1Y = p0.y + radius * dy;
    double pos2X = p1.x + radius * dx;
    double pos2Y = p1.y + radius * dy;
    double pos3X = p1.x - radius * dx;
    double pos3Y = p1.y - radius * dy;

    double minBoundX = glm::min(glm::min(glm::min(pos0X, pos1X), pos2X), pos3X);
    double minBoundY = glm::min(glm::min(glm::min(pos0Y, pos1Y), pos2Y), pos3Y);
    double maxBoundX = glm::max(glm::max(glm::max(pos0X, pos1X), pos2X), pos3X);
    double maxBoundY = glm::max(glm::max(glm::max(pos0Y, pos1Y), pos2Y), pos3Y);

    double rangeX = maxBoundX - minBoundX;
    double rangeY = maxBoundY - minBoundY;

    u = static_cast<uint16_t>(((pos0X - minBoundX) / rangeX * 16.0) / globalContextData.maxTexSize * 0xFFFF);
    v = static_cast<uint16_t>(((pos0Y - minBoundY) / rangeY * 16.0) / globalContextData.maxTexSize * 0xFFFF);
    Vertex v0 = {
        { static_cast<float>(pos0X), static_cast<float>(pos0Y) },
        { u, v },
        pub->strokeStyle.innerColor()
    };

    u = static_cast<uint16_t>(((pos1X - minBoundX) / rangeX * 16.0) / globalContextData.maxTexSize * 0xFFFF);
    v = static_cast<uint16_t>(((pos1Y - minBoundY) / rangeY * 16.0) / globalContextData.maxTexSize * 0xFFFF);
    Vertex v1 = {
        { static_cast<float>(pos1X), static_cast<float>(pos1Y) },
        { u, v },
        pub->strokeStyle.innerColor()
    };

    u = static_cast<uint16_t>(((pos2X - minBoundX) / rangeX * 16.0) / globalContextData.maxTexSize * 0xFFFF);
    v = static_cast<uint16_t>(((pos2Y - minBoundY) / rangeY * 16.0) / globalContextData.maxTexSize * 0xFFFF);
    Vertex v2 = {
        { static_cast<float>(pos2X), static_cast<float>(pos2Y) },
        { u, v },
        pub->strokeStyle.innerColor()
    };

    u = static_cast<uint16_t>(((pos3X - minBoundX) / rangeX * 16.0) / globalContextData.maxTexSize * 0xFFFF);
    v = static_cast<uint16_t>(((pos3Y - minBoundY) / rangeY * 16.0) / globalContextData.maxTexSize * 0xFFFF);
    Vertex v3 = {
        { static_cast<float>(pos3X), static_cast<float>(pos3Y) },
        { u, v },
        pub->strokeStyle.innerColor()
    };

    path.vertexCache().push_back(v0);
    path.vertexCache().push_back(v1);
    path.vertexCache().push_back(v2);
    path.vertexCache().push_back(v2);
    path.vertexCache().push_back(v3);
    path.vertexCache().push_back(v0);
}

}

}

