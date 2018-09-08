#define MPE_POLY2TRI_IMPLEMENTATION
#define TUNIS_GL_IMPLEMENTATION

#ifndef TUNIS_VERTEX_MAX
#define TUNIS_VERTEX_MAX 16384
#endif

//#ifndef TUNIS_MAX_TEXTURE_SIZE
#define TUNIS_MAX_TEXTURE_SIZE 2048
//#endif

#define TUNIS_CURVE_RECURSION_LIMIT 14


#if defined(_MSC_VER)
#include <basetsd.h>
#include <stdlib.h>
#include <search.h>
typedef SSIZE_T ssize_t;
#endif
#include <MPE_fastpoly2tri.h>

#include <TunisGL.h>
#include <TunisVertex.h>
#include "TunisGraphicStates.h"
#include <easy/profiler.h>
#include <Tunis.h>

#include <glm/gtc/epsilon.hpp>

namespace tunis
{

namespace detail
{

GraphicStates gfxStates;

using MemPool = std::vector<uint8_t>;

class ShaderProgram
{
public:
    ShaderProgram() = default;
    virtual ~ShaderProgram() = default;
    virtual void init();
    virtual void shutdown();

    void useProgram();
    void setTexture0Uniform(int32_t value);
    void setViewSizeUniform(int32_t width, int32_t height);

    GLuint programId = 0;

    // attribute locations
    GLint a_position = 0;
    GLint a_texcoord = 0;
    GLint a_color = 0;

    // uniform locations
    GLint u_viewSize = 0;
    GLint u_texture0 = 0;

    // uniform values
    int32_t viewWidth = 0;
    int32_t viewHeight = 0;
    int32_t texture0 = 0;
};

struct BatchArray : public SoA<ShaderProgram*, Texture, size_t, size_t>
{
    inline ShaderProgram* &program(size_t i) { return get<0>(i); }
    inline Texture &texture(size_t i) { return get<1>(i); }
    inline size_t &offset(size_t i) { return get<2>(i); }
    inline size_t &count(size_t i) { return get<3>(i); }
};

struct ContextPriv
{
    std::vector<ContextState> states;

    std::vector<Texture> textures;

    ShaderProgram default2DProgram;
    GLuint vao = 0;

    enum {
        VBO = 0,
        IBO = 1
    };
    GLuint buffers[2] = {0, 0};

    int32_t viewWidth = 0;
    int32_t viewHeight = 0;

    std::vector<Vertex> vertexBuffer; // write-only interleaved VBO data. DO NOT USE SoA on this member!!!
    std::vector<uint16_t> indexBuffer; // write-only

    BatchArray batches;

    MemPool pool;

    float tessTol = 0.25f;
    float distTol = 0.01f;

    void addBatch(ShaderProgram *program, Texture texture, size_t vertexCount, Vertex **out);
    void pushColorRect(float x, float y, float w, float h, const Color &color);
    void renderViewport(int w, int h, float devicePixelRatio);
    size_t addSubPath(detail::SubPath2DArray &subpaths, glm::vec2 &&startPos);
    size_t addPoint(detail::PointArray &points, glm::vec2 &&pos, PointMask &&flags);
    float calcSqrtDistance(float x1, float y1, float x2, float y2);
    void recursiveBezier(detail::PointArray &points,
                         float x1, float y1,
                         float x2, float y2,
                         float x3, float y3,
                         float x4, float y4,
                         unsigned &&level, PointMask &&type);
    float triarea2(glm::vec2 &a, glm::vec2 &b, glm::vec2 &c);
    float polyArea(detail::PointArray &pts);
    void polyReverse(detail::PointArray &pts);
    void updatePath(Path2D &path, FillRule fillRule);
};

void ShaderProgram::useProgram()
{
    EASY_FUNCTION(profiler::colors::Red);
    if (gfxStates.programId != programId)
    {
        glUseProgram(programId);
        gfxStates.programId = programId;
    }
}

void ShaderProgram::setTexture0Uniform(int32_t value)
{
    EASY_FUNCTION(profiler::colors::Red);
    assert(value >= 0 && value < 32);
    assert(gfxStates.programId == programId);

    if (texture0 != value)
    {
        glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(value));
        glUniform1i(u_texture0, value);
        texture0 = value;
    }
}

void ShaderProgram::setViewSizeUniform(int32_t width, int32_t height)
{
    EASY_FUNCTION(profiler::colors::Red);
    assert(gfxStates.programId == programId);

    if (viewWidth != width || viewHeight != height)
    {
        glUniform2f(u_viewSize,
                    static_cast<GLfloat>(width),
                    static_cast<GLfloat>(height));

        viewWidth = width;
        viewHeight = height;
    }
}

void ShaderProgram::init()
{
    EASY_FUNCTION(profiler::colors::Red);

    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    programId = glCreateProgram();

    const char * vertSrc =
            "#if defined(GL_ES)\n"
            "precision highp float;\n"
            "#endif\n"
            "\n"
            "uniform vec2 u_viewSize;\n"
            "\n"
            "attribute vec2 a_position;\n"
            "attribute vec2 a_texcoord;\n"
            "attribute vec4 a_color;\n"
            "\n"
            "varying vec2 v_texcoord;\n"
            "varying vec4 v_color;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    v_texcoord   = a_texcoord;\n"
            "    v_color      = a_color;\n"
            "    gl_Position  = vec4(2.0*a_position.x/u_viewSize.x - 1.0, 1.0 - 2.0*a_position.y/u_viewSize.y, 0, 1);\n"
            "}\n";

    const char * fragSrc =
            "#if defined(GL_ES)\n"
            "precision highp float;\n"
            "#endif\n"
            "\n"
            "varying vec2 v_texcoord;\n"
            "varying vec4 v_color;\n"
            "\n"
            "uniform sampler2D u_texture0;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    gl_FragColor = texture2D(u_texture0, v_texcoord) * v_color;\n"
            "}\n";

    GLint vert_len = static_cast<GLint>(strlen(vertSrc));
    GLint frag_len = static_cast<GLint>(strlen(fragSrc));

    glShaderSource(vert, 1, &vertSrc, &vert_len);
    glShaderSource(frag, 1, &fragSrc, &frag_len);

    glCompileShader(vert);
    glCompileShader(frag);

    glAttachShader(programId, vert);
    glAttachShader(programId, frag);

    glLinkProgram(programId);

    glDeleteShader(vert);
    glDeleteShader(frag);

    // attribute locations
    a_position = glGetAttribLocation(programId, "a_position");
    a_texcoord = glGetAttribLocation(programId, "a_texcoord");
    a_color = glGetAttribLocation(programId, "a_color");

    // uniform locations
    u_viewSize = glGetUniformLocation(programId, "u_viewSize");
    u_texture0 = glGetUniformLocation(programId, "u_texture0");

    // activate shader program.
    useProgram();

    // shader's sampler setup
    setTexture0Uniform(0);
}

void ShaderProgram::shutdown()
{
    EASY_FUNCTION(profiler::colors::Red);

    if (gfxStates.programId == programId)
    {
        glUseProgram(0);
        gfxStates.programId = 0;
    }

    glDeleteProgram(programId);

    programId = 0;
    a_position = 0;
    a_texcoord = 0;
    a_color = 0;
    u_viewSize = 0;
    u_texture0 = 0;
    viewWidth = 0;
    viewHeight = 0;
    texture0 = 0;
}

void ContextPriv::addBatch(ShaderProgram *program, Texture texture, size_t vertexCount, Vertex **out)
{
    EASY_FUNCTION(profiler::colors::DarkRed);

    assert(vertexCount >= 3);

    size_t elemCount = (vertexCount - 2) * 3;

    size_t offset = indexBuffer.size();
    size_t iend = offset + elemCount;

    size_t vstart = vertexBuffer.size();
    size_t vend = vstart + vertexCount;

    indexBuffer.resize(iend);
    vertexBuffer.resize(vend);

    *out = &vertexBuffer[vstart];

    /*
     * Note:
     *
     * Vertex is Position(2floats=8bytes)+TCoord(2xshort=4bytes)+Color(4bytes) = 16 bytes
     *
     * A rectangle is 4 vertices = 64 bytes + 6 indicies (12 bytes) = 76 bytes per rectangle.
     *
     * Without an IBO, it would have been 6 vertices = 96 bytes, so using an ibo
     * saves us 20 bytes per rectangles.
     */
    for (size_t i = offset, v = vstart; i < iend; i+=3, ++v)
    {
        indexBuffer[i] = static_cast<uint16_t>(vstart);
        indexBuffer[i+1]= static_cast<uint16_t>(v+1);
        indexBuffer[i+2] = static_cast<uint16_t>(v+2);
    }

    if (batches.size() > 0)
    {
        size_t id = batches.size() - 1;

        if (batches.program(id) == program && batches.texture(id) == texture)
        {
            // the batch may continue
            batches.count(id) += elemCount;
            return;
        }
    }

    // start a new batch. RenderDefault2D can use any textures for now, as long
    // as they have that little white square in them.
    batches.push(std::move(program),
                 std::move(texture),
                 std::move(offset),
                 std::move(elemCount));
}

void ContextPriv::pushColorRect(float x, float y, float w, float h, const Color &color)
{
    EASY_FUNCTION(profiler::colors::DarkRed);

    /*
        (tl)<---------(tr)
         | \           ^
         |   \  tri 2  |
         |     \       |
         |       \     |
         | tri 1   \   |
         V           \ |
        (bl)-------->(br)
    */

    Vertex *vertices;
    addBatch(&default2DProgram, textures.back(), 4, &vertices);

    vertices[0] = {{x,   y  }, {0, 0}, color}; // top left
    vertices[1] = {{x,   y+h}, {0, 1}, color}; // bottom left
    vertices[2] = {{x+w, y+h}, {1, 1}, color}; // bottom right
    vertices[3] = {{x+w, y  }, {1, 0}, color}; // top right
}

void ContextPriv::renderViewport(int w, int h, float devicePixelRatio)
{
    EASY_FUNCTION(profiler::colors::DarkRed);

    viewWidth = std::move(w);
    viewHeight = std::move(h);
    tessTol = 0.25f / devicePixelRatio;
    distTol = 0.01f / devicePixelRatio;

}

size_t ContextPriv::addSubPath(detail::SubPath2DArray &subpaths, glm::vec2 &&startPos)
{
    size_t id = subpaths.size();
    detail::PointArray points;
    addPoint(points, std::move(startPos), PointCorner);
    subpaths.push(std::move(points), false, 0);
    return id;
}

size_t ContextPriv::addPoint(detail::PointArray &points, glm::vec2 &&pos, PointMask &&flags)
{
    size_t ptID = points.size();
    if (ptID > 0)
    {
        --ptID;

        if (glm::all(glm::epsilonEqual(points.pos(ptID), pos, distTol)))
        {
            points.flags(ptID) |= flags;
            return ptID;
        }
    }

    points.push(std::move(pos), glm::vec2(), std::move(flags));
    return ptID;
}

float ContextPriv::calcSqrtDistance(float x1, float y1, float x2, float y2)
{
    float dx = x2-x1;
    float dy = y2-y1;
    return dx * dx + dy * dy;
}

// based of http://antigrain.com/__code/src/agg_curves.cpp.html by Maxim Shemanarev
void ContextPriv::recursiveBezier(detail::PointArray &points,
                                  float x1, float y1,
                                  float x2, float y2,
                                  float x3, float y3,
                                  float x4, float y4,
                                  unsigned &&level, PointMask &&type)
{
    static constexpr unsigned curve_recursion_limit = 32;

    if(level > curve_recursion_limit)
    {
        return;
    }

    // Calculate all the mid-points of the line segments
    //----------------------
    float x12   = (x1 + x2) / 2;
    float y12   = (y1 + y2) / 2;
    float x23   = (x2 + x3) / 2;
    float y23   = (y2 + y3) / 2;
    float x34   = (x3 + x4) / 2;
    float y34   = (y3 + y4) / 2;
    float x123  = (x12 + x23) / 2;
    float y123  = (y12 + y23) / 2;
    float x234  = (x23 + x34) / 2;
    float y234  = (y23 + y34) / 2;
    float x1234 = (x123 + x234) / 2;
    float y1234 = (y123 + y234) / 2;

    // Try to approximate the full cubic curve by a single straight line
    //------------------
    float dx = x4-x1;
    float dy = y4-y1;

    float d2 = glm::abs(((x2 - x4) * dy - (y2 - y4) * dx));
    float d3 = glm::abs(((x3 - x4) * dy - (y3 - y4) * dx));
    float da1, da2, k;

    switch((int(d2 > glm::epsilon<float>()) << 1) + int(d3 > glm::epsilon<float>()))
    {
    case 0:
        // All collinear OR p1==p4
        //----------------------
        k = dx*dx + dy*dy;
        if(glm::epsilonEqual(k, 0.0f, glm::epsilon<float>()))
        {
            d2 = calcSqrtDistance(x1, y1, x2, y2);
            d3 = calcSqrtDistance(x4, y4, x3, y3);
        }
        else
        {
            k   = 1 / k;
            da1 = x2 - x1;
            da2 = y2 - y1;
            d2  = k * (da1*dx + da2*dy);
            da1 = x3 - x1;
            da2 = y3 - y1;
            d3  = k * (da1*dx + da2*dy);
            if(d2 > 0 && d2 < 1 && d3 > 0 && d3 < 1)
            {
                // Simple collinear case, 1---2---3---4
                // We can leave just two endpoints
                return;
            }
                 if(d2 <= 0) d2 = calcSqrtDistance(x2, y2, x1, y1);
            else if(d2 >= 1) d2 = calcSqrtDistance(x2, y2, x4, y4);
            else             d2 = calcSqrtDistance(x2, y2, x1 + d2*dx, y1 + d2*dy);

                 if(d3 <= 0) d3 = calcSqrtDistance(x3, y3, x1, y1);
            else if(d3 >= 1) d3 = calcSqrtDistance(x3, y3, x4, y4);
            else             d3 = calcSqrtDistance(x3, y3, x1 + d3*dx, y1 + d3*dy);
        }
        if(d2 > d3)
        {
            if(d2 < tessTol)
            {
                addPoint(points, glm::vec2(x2, y2), std::move(type));
                return;
            }
        }
        else
        {
            if(d3 < tessTol)
            {
                addPoint(points, glm::vec2(x3, y3), std::move(type));
                return;
            }
        }
        break;

    case 1:
        // p1,p2,p4 are collinear, p3 is significant
        //----------------------
        if(d3 * d3 <= tessTol * (dx*dx + dy*dy))
        {
            addPoint(points, glm::vec2(x23, y23), std::move(type));
            return;
        }
        break;

    case 2:
        // p1,p3,p4 are collinear, p2 is significant
        //----------------------
        if(d2 * d2 <= tessTol * (dx*dx + dy*dy))
        {
            addPoint(points, glm::vec2(x23, y23), std::move(type));
            return;
        }
        break;

    case 3:
        // Regular case
        //-----------------
        if((d2 + d3)*(d2 + d3) <= tessTol * (dx*dx + dy*dy))
        {
            addPoint(points, glm::vec2(x23, y23), std::move(type));
            return;
        }
        break;
    }

    // Continue subdivision
    //----------------------
    recursiveBezier(points, x1, y1, x12, y12, x123, y123, x1234, y1234, level + 1, 0);
    recursiveBezier(points, x1234, y1234, x234, y234, x34, y34, x4, y4, level + 1, std::move(type));
}

float ContextPriv::triarea2(glm::vec2 &a, glm::vec2 &b, glm::vec2 &c)
{
    float abx = b.x - a.x;
    float aby = b.y - a.y;
    float acx = c.x - a.x;
    float acy = c.y - a.y;
    return acx*aby - abx*acy;
}

float ContextPriv::polyArea(detail::PointArray &pts)
{
    float area = 0;
    for (size_t i = 2; i < pts.size(); ++i)
    {
        area += triarea2(pts.pos(0),
                         pts.pos(i-1),
                         pts.pos(i));
    }
    return area * 0.5f;
}

void ContextPriv::polyReverse(detail::PointArray &pts)
{
    size_t i = 0, j = pts.size()-1;
    while (i < j)
    {
        pts.swap(i, j);
        i++;
        j--;
    }
}

void ContextPriv::updatePath(Path2D &path, FillRule fillRule)
{
    uint8_t &dirty = path.dirty();
    FillRule &rFillRule = path.fillRule();

    if (dirty || rFillRule != fillRule)
    {
        detail::SubPath2DArray &subpaths = path.subpaths();
        detail::PathCommandArray &commands = path.commands();
        glm::vec2 &boundTopLeft = path.boundTopLeft();
        glm::vec2 &boundBottomRight = path.boundBottomRight();

        size_t lastPointID;
        size_t currSubpathID = 0;

        // reset to default.
        subpaths.resize(0);
        boundTopLeft = glm::vec2(FLT_MAX);
        boundBottomRight = glm::vec2(-FLT_MAX);

        for(size_t i = 0; i < commands.size(); ++i)
        {
            switch(commands.type(i))
            {
            case detail::CLOSE:
                if (subpaths.size() > 0)
                {
                    subpaths.closed(currSubpathID) = true;
                }
                break;
            case detail::MOVE_TO:
                currSubpathID = addSubPath(subpaths, glm::vec2(commands.param0(i), commands.param1(i)));
                break;
            case detail::LINE_TO:
                if (subpaths.size() == 0) { currSubpathID = addSubPath(subpaths, glm::vec2(0, 0)); }
                addPoint(subpaths.points(currSubpathID), glm::vec2(commands.param0(i), commands.param1(i)), PointCorner);
                break;
            case detail::BEZIER_TO:
            {
                if (subpaths.size() == 0) { currSubpathID = addSubPath(subpaths, glm::vec2(0, 0)); }
                PointArray &points = subpaths.points(currSubpathID);
                glm::vec2 &prevPos = points.pos(points.size() - 1);
                recursiveBezier(points,
                                prevPos.x,
                                prevPos.y,
                                commands.param0(i), commands.param1(i),
                                commands.param2(i), commands.param3(i),
                                commands.param4(i), commands.param5(i),
                                0, PointCorner);

                break;
            }
            case detail::QUAD_TO:
            {
                if (subpaths.size() == 0) { currSubpathID = addSubPath(subpaths, glm::vec2(0, 0)); }
                PointArray &points = subpaths.points(currSubpathID);
                glm::vec2 &prevPos = points.pos(points.size() - 1);
                float x0 = prevPos.x;
                float y0 = prevPos.y;
                float cx = commands.param0(i);
                float cy = commands.param1(i);
                float x = commands.param2(i);
                float y = commands.param3(i);
                float c1x = x0 + 2.0f/3.0f*(cx - x0);
                float c1y = y0 + 2.0f/3.0f*(cy - y0);
                float c2x = x + 2.0f/3.0f*(cx - x);
                float c2y = y + 2.0f/3.0f*(cy - y);
                recursiveBezier(points,
                                std::move(x0), std::move(y0),
                                std::move(c1x), std::move(c1y),
                                std::move(c2x), std::move(c2y),
                                std::move(x), std::move(y),
                                0, PointCorner);
                break;
            }
            case detail::ARC:
                // TODO
                break;
            case detail::ARC_TO:
                // TODO
                break;
            case detail::ELLIPSE:
                // TODO
                break;
            case detail::RECT:
                // TODO
                break;
            }
        }

        // From this point, all of the commands have all been executed and
        // we should have 1 or more subpaths with contour points in them.

        for (size_t i = 0; i < subpaths.size(); ++i)
        {
            auto &points = subpaths.points(i);
            size_t &bevelCount = subpaths.bevelCount(i);

            bevelCount = 0;
            lastPointID = points.size() - 1;

            // Check if the first and last point are the same. Get rid of
            // the last point if that is the case, and close the subpath.
            if (glm::all(glm::epsilonEqual(points.pos(0), points.pos(lastPointID), distTol)))
            {
                points.pop_back();
                subpaths.closed(i) = true;
            }

            // check if we're still a polygon, reset the subpath if we lost
            // this status.
            if (points.size() <= 2)
            {
                points.resize(0);
                continue;
            }

            // make sure the polygon contour vertices are always
            // counter-clock wise. a negative polygon area is going to
            // indicate that situation.
            if (polyArea(points) < 0.0f)
            {
                polyReverse(points);
            }

            // Calculate direction vectors and update subpath's bounding box
            size_t prevPointID = lastPointID;
            for(size_t j = 0; j < points.size(); ++j)
            {
                glm::vec2 &currPos = points.pos(j);
                glm::vec2 &prevPos = points.pos(prevPointID);

                points.dir(prevPointID) = glm::normalize(currPos - prevPos);

                boundTopLeft = glm::min(boundTopLeft, currPos);
                boundBottomRight = glm::max(boundBottomRight, currPos);

                prevPointID = j;
            }

        }

        dirty = false;
        rFillRule = std::move(fillRule);
    }
}

}

Context::Context() :
    ctx(new detail::ContextPriv())
{
    EASY_FUNCTION(profiler::colors::DarkRed);

    auto tunisGL_initialized = tunisGLInit();
    if (!tunisGL_initialized)
    {
        abort();
    }

#if 0
    ctx->pool.reserve(1024*1024);
    memset(&ctx->ma, 0, sizeof(ctx->ma));
    ctx->ma.memalloc = detail::poolAlloc;
    ctx->ma.memfree = detail::poolFree;
    ctx->ma.userData = &ctx->pool;
    ctx->ma.extraVertices = 256; // realloc not provided, allow 256 extra vertices.
#else
    // Request how much memory (in bytes) you should
    // allocate for the library
    size_t MemoryRequired = MPE_PolyMemoryRequired(TUNIS_VERTEX_MAX);

    // Allocate a memory block of size MemoryRequired
    // IMPORTANT: The memory must be zero initialized
    ctx->pool.resize(MemoryRequired, 0);

#endif

    // default state.
    ctx->states.push_back(*this);

    // Create a default texture atlas.
#ifdef TUNIS_MAX_TEXTURE_SIZE
    detail::gfxStates.maxTexSize = TUNIS_MAX_TEXTURE_SIZE;
#else
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &detail::global.maxTexSize);
#endif
    Texture tex = Texture(detail::gfxStates.maxTexSize, detail::gfxStates.maxTexSize);
    ctx->textures.push_back(tex); // retain

    Paint::reserve(1024);
    Path2D::reserve(1024);
    ctx->batches.reserve(1024);

    ctx->vertexBuffer.reserve(TUNIS_VERTEX_MAX);

    // Initialize our default shader.
    ctx->default2DProgram.init();

    if (tunisGLSupport(GL_VERSION_3_0))
    {
        // Create a dummy vertex array object (mandatory since GL Core profile)
        glGenVertexArrays(1, &ctx->vao);
        glBindVertexArray(ctx->vao);
    }

    // Create vertex and index buffer objects for the batches
    glGenBuffers(2, ctx->buffers);
    glBindBuffer(GL_ARRAY_BUFFER, ctx->buffers[detail::ContextPriv::VBO]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx->buffers[detail::ContextPriv::IBO]);

    // Configure our vertex attributes
    glVertexAttribPointer(static_cast<GLuint>(ctx->default2DProgram.a_position), decltype(Vertex::pos)::length(),    GL_FLOAT,          GL_FALSE, sizeof(Vertex), reinterpret_cast<const void *>(0));
    glVertexAttribPointer(static_cast<GLuint>(ctx->default2DProgram.a_texcoord), decltype(Vertex::tcoord)::length(), GL_UNSIGNED_SHORT, GL_TRUE,  sizeof(Vertex), reinterpret_cast<const void *>(sizeof(Vertex::pos)));
    glVertexAttribPointer(static_cast<GLuint>(ctx->default2DProgram.a_color),    decltype(Vertex::color)::length(),  GL_UNSIGNED_BYTE,  GL_TRUE,  sizeof(Vertex), reinterpret_cast<const void *>(sizeof(Vertex::pos) + sizeof(Vertex::tcoord)));
    glEnableVertexAttribArray(static_cast<GLuint>(ctx->default2DProgram.a_position));
    glEnableVertexAttribArray(static_cast<GLuint>(ctx->default2DProgram.a_texcoord));
    glEnableVertexAttribArray(static_cast<GLuint>(ctx->default2DProgram.a_color));

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
    EASY_FUNCTION(profiler::colors::DarkRed);

    // unload texture data by deleting every potential texture holders.
    ctx->textures.resize(0);
    ctx->batches.resize(0);
    fillStyle.reset();
    strokeStyle.reset();

    // unload shader program
    ctx->default2DProgram.shutdown();

    // unload vertex and index buffers
    glDeleteBuffers(2, ctx->buffers);
    ctx->buffers[detail::ContextPriv::VBO] = 0;
    ctx->buffers[detail::ContextPriv::IBO] = 0;

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



void Context::clearFrame(int32_t fbLeft, int32_t fbTop, int32_t fbWidth, int32_t fbHeight, Color backgroundColor)
{
    EASY_FUNCTION(profiler::colors::DarkRed);

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

void Context::beginFrame(int32_t winWidth, int32_t winHeight, float devicePixelRatio)
{
    EASY_FUNCTION(profiler::colors::DarkRed);

    // reset the states.
    ctx->states.resize(0);
    *static_cast<ContextState*>(this) = ContextState();

    ctx->renderViewport(std::move(winWidth), std::move(winHeight), std::move(devicePixelRatio));
}

void Context::endFrame()
{
    EASY_FUNCTION(profiler::colors::DarkRed);

    // flush the vertex buffer.
    if (ctx->vertexBuffer.size() > 0) {
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(ctx->vertexBuffer.size() * sizeof(Vertex)),
                     &ctx->vertexBuffer.front(),
                     GL_STREAM_DRAW);
        ctx->vertexBuffer.resize(0);
    }


    // flush the index buffer.
    if (ctx->indexBuffer.size() > 0) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(ctx->indexBuffer.size() * sizeof(uint16_t)),
                     &ctx->indexBuffer.front(),
                     GL_STREAM_DRAW);
        ctx->indexBuffer.resize(0);
    }


    // flush the batches
    for (size_t i = 0; i < ctx->batches.size(); ++i)
    {
        ctx->batches.program(i)->useProgram();
        ctx->batches.program(i)->setViewSizeUniform(ctx->viewWidth, ctx->viewHeight);
        ctx->batches.texture(i).bind();

        size_t count = (ctx->batches.count(i)/3+2);

        glDrawArrays(GL_TRIANGLES,
                     static_cast<GLint>(ctx->batches.offset(i)),
                     static_cast<GLsizei>(count));

//        glDrawElements(GL_TRIANGLES,
//                       static_cast<GLsizei>(ctx->batches.count(i)),
//                       GL_UNSIGNED_SHORT,
//                       reinterpret_cast<void*>(ctx->batches.offset(i) * sizeof(GLushort)));

//        for (size_t j = 0; j < count/3; ++j)
//        {
//            glDrawArrays(GL_LINE_LOOP,
//                         static_cast<GLint>(ctx->batches.offset(i)+(j*3)),
//                         static_cast<GLsizei>(3));
//        }
    }

    ctx->batches.resize(0);
}

void Context::save()
{
    EASY_FUNCTION(profiler::colors::DarkRed);
    ctx->states.push_back(*this);
}

void Context::restore()
{
    EASY_FUNCTION(profiler::colors::DarkRed);
    if (ctx->states.size() > 0)
    {
        *static_cast<ContextState*>(this) = ctx->states.back();
        ctx->states.pop_back();
    }
}

void Context::fillRect(float x, float y, float width, float height)
{
    EASY_FUNCTION(profiler::colors::DarkRed);
    ctx->pushColorRect(std::move(x), std::move(y), std::move(width), std::move(height), fillStyle.innerColor());
}

void Context::strokeRect(float x, float y, float width, float height)
{
    EASY_FUNCTION(profiler::colors::DarkRed);

    // helps to get the lines a bit sharper when the line width is not divisible by two.
    float offset = fmodf(lineWidth, 2.0f) / 2.0f;
    x+=offset;
    y+=offset;

    float hlw = lineWidth * 0.5f;

    // top line
    ctx->pushColorRect(x-hlw, y-hlw, width+lineWidth-1, lineWidth, strokeStyle.innerColor());

    // bottom line
    ctx->pushColorRect(x-hlw, y+height-hlw, width+lineWidth-1, lineWidth, strokeStyle.innerColor());

    // left line
    ctx->pushColorRect(x-hlw, y+hlw, lineWidth, height-lineWidth, strokeStyle.innerColor());

    // right line
    ctx->pushColorRect(x+width-hlw-1, y+hlw, lineWidth, height-lineWidth, strokeStyle.innerColor());
}

void Context::clearRect(float x, float y, float width, float height)
{
    EASY_FUNCTION(profiler::colors::DarkRed);
    ctx->pushColorRect(std::move(x), std::move(y), std::move(width), std::move(height), detail::gfxStates.backgroundColor);
}

void Context::fill(Path2D &path, FillRule fillRule)
{
    EASY_FUNCTION(profiler::colors::DarkRed);

    ctx->updatePath(path, fillRule);

    auto &subpaths = path.subpaths();

    glm::vec2 range = path.boundBottomRight() - path.boundTopLeft();

    size_t vertexCount = 0;
    for(size_t i = 0; i < subpaths.size(); ++i)
    {
        auto &pts = subpaths.points(i);

        if (pts.size() == 0)
        {
            continue;
        }

        vertexCount += pts.size();

        size_t leftTurnCount = 0;
        size_t prevPointID = pts.size() - 1;
        for (size_t j = 0; j < pts.size(); j++)
        {
            glm::vec2 &dir = pts.dir(j);
            glm::vec2 &prevDir = pts.dir(prevPointID);

            float cross = dir.x * prevDir.y - prevDir.x * dir.y;
            if (cross > 0.0f)
            {
                ++leftTurnCount;
            }

            prevPointID = j;
        }

        // convexity test.
        if (leftTurnCount == pts.size()) // convex
        {
            Vertex *verticies;
            ctx->addBatch(&ctx->default2DProgram, ctx->textures.back(), pts.size(), &verticies);

            for (size_t j = 0; j < pts.size(); ++j)
            {
                glm::vec2 &pos = pts.pos(j);
                glm::vec2 tcoord = TCoord(((pos.x - path.boundTopLeft()) / range) * 16.0f / static_cast<float>(detail::gfxStates.maxTexSize) * static_cast<float>(0xFFFF));
                verticies[j].pos = pos;
                verticies[j].tcoord.x = static_cast<uint16_t>(tcoord.s);
                verticies[j].tcoord.t = static_cast<uint16_t>(tcoord.t);
                verticies[j].color = fillStyle.innerColor();
            }
        }
        else // concave
        {
            MPEPolyContext PolyContext;
            memset(ctx->pool.data(), 0, ctx->pool.size());
            if (MPE_PolyInitContext(&PolyContext, ctx->pool.data(), TUNIS_VERTEX_MAX))
            {
                MPEPolyPoint* FirstPoint = MPE_PolyPushPointArray(&PolyContext, static_cast<uint32_t>(pts.size()));
                for (size_t j = 0; j < pts.size(); ++j)
                {
                    FirstPoint[j].X = pts.pos(j).x;
                    FirstPoint[j].Y = pts.pos(j).y;
                }

                // Add the polyline for the edge. This will consume all points added so far.
                MPE_PolyAddEdge(&PolyContext);

                // Triangulate the shape
                MPE_PolyTriangulate(&PolyContext);

                Vertex *verticies;
                size_t vertexCount = PolyContext.TriangleCount * 3;
                ctx->addBatch(&ctx->default2DProgram, ctx->textures.back(), vertexCount, &verticies);

                // The resulting triangles can be used like so
                for (size_t TriangleIndex = 0; TriangleIndex < PolyContext.TriangleCount; ++TriangleIndex)
                {
                    MPEPolyTriangle* Triangle = PolyContext.Triangles[TriangleIndex];

                    for(size_t j = 0; j < 3; ++j)
                    {
                        MPEPolyPoint* Point = Triangle->Points[2-j];
                        Position pos(Point->X, Point->Y);
                        glm::vec2 tcoord = TCoord(((pos.x - path.boundTopLeft()) / range) * 16.0f / static_cast<float>(detail::gfxStates.maxTexSize) * static_cast<float>(0xFFFF));
                        size_t id = (TriangleIndex*3)+j;
                        verticies[id].pos = pos;
                        verticies[id].tcoord.x = static_cast<uint16_t>(tcoord.s);
                        verticies[id].tcoord.t = static_cast<uint16_t>(tcoord.t);
                        verticies[id].color = fillStyle.innerColor();
                    }
                }
            }
        }

    }
}

void Context::stroke(Path2D &path)
{
    EASY_FUNCTION(profiler::colors::DarkRed);
    //ctx->stroke(path, strokeStyle, lineJoin, miterLimit);
}


}
