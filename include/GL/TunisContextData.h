#ifndef TUNISCONTEXTDATA_H
#define TUNISCONTEXTDATA_H

#ifdef _WIN32
#define NOMINMAX 1
#endif

#include <TunisContextState.h>
#include <TunisPath2D.h>
#include <TunisPaint.h>
#include <TunisTypes.h>
#include <TunisVertex.h>
#include <TunisTexture.h>
#include <TunisGL.h>

#include <easy/profiler.h>

#include <glm/gtc/epsilon.hpp>

#ifndef TUNIS_VERTEX_MAX
#define TUNIS_VERTEX_MAX 16384
#endif

//#ifndef TUNIS_MAX_TEXTURE_SIZE
#define TUNIS_MAX_TEXTURE_SIZE 2048
//#endif

#define TUNIS_CURVE_RECURSION_LIMIT 14

namespace tunis
{

class Context;

namespace detail
{

struct ShaderProgram;

struct GraphicStates
{
    Color backgroundColor = Transparent;
    Viewport viewport = Viewport(0, 0, 100, 100);
    GLuint textureId = 0;
    GLuint program = 0;
    int32_t maxTexSize = 0;
};
extern GraphicStates gfxStates;

struct ShaderProgram
{
    GLuint vert = 0;
    GLuint frag = 0;
    GLuint program = 0;

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

    constexpr static const char * vertSrc =
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
            "    v_texcoord  = a_texcoord;\n"
            "    v_color     = a_color;\n"
            "    gl_Position = vec4(2.0*a_position.x/u_viewSize.x - 1.0, 1.0 - 2.0*a_position.y/u_viewSize.y, 0, 1);\n"
            "}\n";

    constexpr static const char * fragSrc =
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

    virtual ~ShaderProgram() = default;

    void useProgram()
    {
        EASY_FUNCTION(profiler::colors::Red);
        if (gfxStates.program != program)
        {
            glUseProgram(program);
            gfxStates.program = program;
        }
    }

    void setTexture0Uniform(int32_t value)
    {
        EASY_FUNCTION(profiler::colors::Red);
        assert(value >= 0 && value < 32);
        assert(gfxStates.program == program);

        if (texture0 != value)
        {
            glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(value));
            glUniform1i(u_texture0, value);
            texture0 = value;
        }
    }

    void setViewSizeUniform(int32_t width, int32_t height)
    {
        EASY_FUNCTION(profiler::colors::Red);
        assert(gfxStates.program == program);

        if (viewWidth != width || viewHeight != height)
        {
            glUniform2f(u_viewSize,
                        static_cast<GLfloat>(width),
                        static_cast<GLfloat>(height));

            viewWidth = width;
            viewHeight = height;
        }
    }

    virtual void init()
    {
        EASY_FUNCTION(profiler::colors::Red);

        vert = glCreateShader(GL_VERTEX_SHADER);
        frag = glCreateShader(GL_FRAGMENT_SHADER);
        program = glCreateProgram();

        GLint vert_len = static_cast<GLint>(strlen(vertSrc));
        GLint frag_len = static_cast<GLint>(strlen(fragSrc));

        glShaderSource(vert, 1, &vertSrc, &vert_len);
        glShaderSource(frag, 1, &fragSrc, &frag_len);

        glCompileShader(vert);
        glCompileShader(frag);

        glAttachShader(program, vert);
        glAttachShader(program, frag);

        glLinkProgram(program);

        // attribute locations
        a_position = glGetAttribLocation(program, "a_position");
        a_texcoord = glGetAttribLocation(program, "a_texcoord");
        a_color = glGetAttribLocation(program, "a_color");

        // uniform locations
        u_viewSize = glGetUniformLocation(program, "u_viewSize");
        u_texture0 = glGetUniformLocation(program, "u_texture0");

        // activate shader program.
        useProgram();

        // shader's sampler setup
        setTexture0Uniform(0);
    }

    virtual void shutdown()
    {
        EASY_FUNCTION(profiler::colors::Red);

        if (gfxStates.program == program)
        {
            glUseProgram(0);
            gfxStates.program = 0;
        }

        glDeleteProgram(program);
        glDeleteShader(vert);
        glDeleteShader(frag);

        vert = 0;
        frag = 0;
        program = 0;
        a_position = 0;
        a_texcoord = 0;
        a_color = 0;
        u_viewSize = 0;
        u_texture0 = 0;
        viewWidth = 0;
        viewHeight = 0;
        texture0 = 0;
    }

};

struct BatchArray : public SoA<ShaderProgram*, Texture, size_t, size_t>
{
    inline ShaderProgram* &program(size_t i) { return get<0>(i); }
    inline Texture &texture(size_t i) { return get<1>(i); }
    inline size_t &offset(size_t i) { return get<2>(i); }
    inline size_t &count(size_t i) { return get<3>(i); }
};

struct ContextData
{
    std::vector<ContextState> states;

    Path2D currentPath;

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

    float tessTol = 0.25f;
    float distTol = 0.01f;

    void addBatch(ShaderProgram *program, Texture texture, size_t offset, size_t count)
    {
        EASY_FUNCTION(profiler::colors::DarkRed);

        if (batches.size() > 0)
        {
            size_t id = batches.size() - 1;

            if (batches.program(id) == program && batches.texture(id) == texture)
            {
                // the batch may continue
                batches.count(id) += count;
                return;
            }
        }

        // start a new batch. RenderDefault2D can use any textures for now, as long
        // as they have that little white square in them.
        batches.push(std::move(program),
                     std::move(texture),
                     std::move(offset),
                     std::move(count));
    }

    void pushColorRect(float x, float y, float w, float h, const Color &color)
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

        // TODO prevent 16bit index overflow here.
        uint16_t tl = static_cast<uint16_t>(vertexBuffer.size());
        uint16_t bl = tl+1;
        uint16_t br = tl+2;
        uint16_t tr = tl+3;

        addBatch(&default2DProgram, textures.back(), indexBuffer.size(), 6); // two triangles, 6 points

        vertexBuffer.insert(vertexBuffer.end(), {
                                {{x,   y  }, {0, 0}, color}, // top left
                                {{x,   y+h}, {0, 1}, color}, // bottom left
                                {{x+w, y+h}, {1, 1}, color}, // bottom right
                                {{x+w, y  }, {1, 0}, color}, // top right
                            });

        indexBuffer.insert(indexBuffer.end(), {
                               tl, bl, br,   // first triangle
                               tl, br, tr    // second triangle
                           });

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

    }

    void renderViewport(int w, int h, float devicePixelRatio)
    {
        EASY_FUNCTION(profiler::colors::DarkRed);

        viewWidth = std::move(w);
        viewHeight = std::move(h);
        tessTol = 0.25f / devicePixelRatio;
        distTol = 0.01f / devicePixelRatio;

    }

    void renderFlush()
    {
        EASY_FUNCTION(profiler::colors::DarkRed);

        // flush the vertex buffer.
        if (vertexBuffer.size() > 0) {
            glBufferData(GL_ARRAY_BUFFER,
                         static_cast<GLsizeiptr>(vertexBuffer.size() * sizeof(Vertex)),
                         &vertexBuffer.front(),
                         GL_STREAM_DRAW);
            vertexBuffer.resize(0);
        }


        // flush the index buffer.
        if (indexBuffer.size() > 0) {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         static_cast<GLsizeiptr>(indexBuffer.size() * sizeof(uint16_t)),
                         &indexBuffer.front(),
                         GL_STREAM_DRAW);
            indexBuffer.resize(0);
        }


        // flush the batches
        for (size_t i = 0; i < batches.size(); ++i)
        {
            batches.program(i)->useProgram();
            batches.program(i)->setViewSizeUniform(viewWidth, viewHeight);
            batches.texture(i).bind();

            glDrawElements(GL_TRIANGLES,
                           static_cast<GLsizei>(batches.count(i)),
                           GL_UNSIGNED_SHORT,
                           reinterpret_cast<void*>(batches.offset(i) * sizeof(GLushort)));
        }

        batches.resize(0);
    }


    size_t addSubPath(detail::SubPath2DArray &subpaths, float startX, float startY)
    {
        detail::PointArray points;
        addPoint(points, std::move(startX), std::move(startY), PointCorner);
        subpaths.push(std::move(points), false, 0);
        return subpaths.size();
    }

    void addPoint(detail::PointArray &points, float x, float y, PointMask flags)
    {
        size_t ptID = points.size();
        if (ptID-- > 0)
        {
            if (glm::epsilonEqual(points.x(ptID), x, distTol) &&
                glm::epsilonEqual(points.y(ptID), y, distTol))
            {
                points.flags(ptID) |= flags;
                return;
            }
        }

        points.push(std::move(x), std::move(y), 0, 0, 0, 0, 0, std::move(flags));
    }

    float calcSqrtDistance(float x1, float y1, float x2, float y2)
    {
        float dx = x2-x1;
        float dy = y2-y1;
        return dx * dx + dy * dy;
    }

    // based of http://antigrain.com/__code/src/agg_curves.cpp.html by Maxim Shemanarev
    inline void recursiveBezier(detail::PointArray &points,
                                float x1, float y1,
                                float x2, float y2,
                                float x3, float y3,
                                float x4, float y4,
                                unsigned level, PointMask type)
    {
        static constexpr float curve_angle_tolerance_epsilon = 0.01f;
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
                    addPoint(points, x2, y2, type);
                    return;
                }
            }
            else
            {
                if(d3 < tessTol)
                {
                    addPoint(points, x3, y3, type);
                    return;
                }
            }
            break;

        case 1:
            // p1,p2,p4 are collinear, p3 is significant
            //----------------------
            if(d3 * d3 <= tessTol * (dx*dx + dy*dy))
            {
                addPoint(points, x23, y23, type);
                return;
            }
            break;

        case 2:
            // p1,p3,p4 are collinear, p2 is significant
            //----------------------
            if(d2 * d2 <= tessTol * (dx*dx + dy*dy))
            {
                addPoint(points, x23, y23, type);
                return;
            }
            break;

        case 3:
            // Regular case
            //-----------------
            if((d2 + d3)*(d2 + d3) <= tessTol * (dx*dx + dy*dy))
            {
                addPoint(points, x23, y23, type);
                return;
            }
            break;
        }

        // Continue subdivision
        //----------------------
        recursiveBezier(points, x1, y1, x12, y12, x123, y123, x1234, y1234, level + 1, 0);
        recursiveBezier(points, x1234, y1234, x234, y234, x34, y34, x4, y4, level + 1, type);
    }

    float triarea2(float ax, float ay, float bx, float by, float cx, float cy)
    {
        float abx = bx - ax;
        float aby = by - ay;
        float acx = cx - ax;
        float acy = cy - ay;
        return acx*aby - abx*acy;
    }

    float polyArea(detail::PointArray &pts)
    {
        float area = 0;
        for (size_t i = 2; i < pts.size(); ++i)
        {
            area += triarea2(pts.x(0), pts.y(0),
                             pts.x(i-1), pts.y(i-1),
                             pts.x(i), pts.y(i));
        }
        return area * 0.5f;
    }

    void polyReverse(detail::PointArray &pts)
    {
        size_t i = 0, j = pts.size()-1;
        while (i < j)
        {
            pts.swap(i, j);
            i++;
            j--;
        }
    }

    float normalize(float &x, float &y)
    {
        float d = glm::sqrt( x * x + y * y);
        if (glm::epsilonNotEqual(d, 0.0f, glm::epsilon<float>()))
        {
            float id = 1.0f / d;
            x *= id;
            y *= id;
        }
        return d;
    }

    void updatePath(Path2D &path, Fill fillRule)
    {
        uint8_t &dirty = path.dirty();
        Fill &rFillRule = path.fillRule();

        if (dirty || rFillRule != fillRule)
        {
            detail::SubPath2DArray &subpaths = path.subpaths();
            detail::PathCommandArray &commands = path.commands();
            float &boundTop = path.boundTop();
            float &boundRight = path.boundRight();
            float &boundBottom = path.boundBottom();
            float &boundLeft = path.boundLeft();

            size_t lastPointID;
            size_t subPathCount = 0;

            // reset to default.
            subpaths.resize(0);
            boundTop = FLT_MAX;
            boundRight = -FLT_MAX;
            boundBottom = -FLT_MAX;
            boundLeft = FLT_MAX;

            for(size_t i = 0; i < commands.size(); ++i)
            {
                switch(commands.type(i))
                {
                case detail::CLOSE:
                    if (subPathCount > 0)
                    {
                        subpaths.closed(subPathCount-1) = true;
                    }
                    break;
                case detail::MOVE_TO:
                    subPathCount = addSubPath(subpaths, commands.param0(i), commands.param1(i));
                    break;
                case detail::LINE_TO:
                    if (subPathCount == 0) { subPathCount = addSubPath(subpaths, 0, 0); }
                    addPoint(subpaths.points(subPathCount-1), commands.param0(i), commands.param1(i), PointCorner);
                    break;
                case detail::BEZIER_TO:
                    if (subPathCount == 0) { subPathCount = addSubPath(subpaths, 0, 0); }
                    lastPointID = subpaths.points(subPathCount-1).size() - 1;
                    recursiveBezier(subpaths.points(subPathCount-1),
                                    subpaths.points(subPathCount-1).x(lastPointID),
                                    subpaths.points(subPathCount-1).y(lastPointID),
                                    commands.param0(i), commands.param1(i),
                                    commands.param2(i), commands.param3(i),
                                    commands.param4(i), commands.param5(i),
                                    0, PointCorner);

                    break;
                case detail::QUAD_TO:
                {
                    if (subPathCount == 0) { subPathCount = addSubPath(subpaths, 0, 0); }
                    lastPointID = subpaths.points(subPathCount-1).size() - 1;
                    float x0 = subpaths.points(subPathCount-1).x(lastPointID);
                    float y0 = subpaths.points(subPathCount-1).y(lastPointID);
                    float cx = commands.param0(i);
                    float cy = commands.param1(i);
                    float x = commands.param2(i);
                    float y = commands.param3(i);
                    float c1x = x0 + 2.0f/3.0f*(cx - x0);
                    float c1y = y0 + 2.0f/3.0f*(cy - y0);
                    float c2x = x + 2.0f/3.0f*(cx - x);
                    float c2y = y + 2.0f/3.0f*(cy - y);
                    recursiveBezier(subpaths.points(subPathCount-1),
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

            for (size_t i = 0; i < subPathCount; ++i)
            {
                auto &points = subpaths.points(i);
                size_t &bevelCount = subpaths.bevelCount(i);

                bevelCount = 0;
                lastPointID = points.size() - 1;

                // Check if the first and last point are the same. Get rid of
                // the last point if that is the case, and close the subpath.
                if (glm::epsilonEqual(points.x(0), points.x(lastPointID), distTol) &&
                        glm::epsilonEqual(points.y(0), points.y(lastPointID), distTol))
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
                    float &currX = points.x(j);
                    float &currY = points.y(j);
                    float &prevX = points.x(prevPointID);
                    float &prevY = points.y(prevPointID);
                    float &prevDirX = points.dirX(prevPointID);
                    float &prevDirY = points.dirY(prevPointID);
                    float &prevLenght = points.dirLen(prevPointID);
                    prevDirX = currX - prevX;
                    prevDirY = currY - prevY;
                    prevLenght = normalize(prevDirX, prevDirY);

                    boundLeft = glm::min(boundLeft, prevX);
                    boundTop = glm::min(boundTop, prevY);
                    boundRight = glm::max(boundRight, prevX);
                    boundBottom = glm::max(boundBottom, prevY);

                    prevPointID = j;
                }

            }

            dirty = false;
            rFillRule = std::move(fillRule);
        }
    }

    inline void fill(Path2D &path, Fill fillRule, Paint &fillStyle)
    {
        updatePath(path, fillRule);

        auto &subpaths = path.subpaths();

        // convexity calculations
        size_t vertexCount = 0;
        bool convex = true;
        for(size_t i = 0; i < subpaths.size(); ++i)
        {
            auto &points = subpaths.points(i);
            vertexCount += points.size();

            if (convex)
            {
                size_t leftTurnCount = 0;
                size_t prevPointID = points.size() - 1;
                for (size_t j = 0; j < points.size(); j++)
                {
                    float &dirX = points.dirX(j);
                    float &dirY = points.dirY(j);
                    float &prevDirX = points.dirX(prevPointID);
                    float &prevDirY = points.dirY(prevPointID);
                    float cross = dirX * prevDirY - prevDirX * dirY;
                    if (cross > 0.0f)
                    {
                        ++leftTurnCount;
                    }

                    prevPointID = j;
                }

                convex = leftTurnCount == points.size();
            }
        }

        if (convex)
        {
            size_t offset = indexBuffer.size();
            size_t elemCount = (vertexCount - 2) * 3;

            size_t vstart = vertexBuffer.size();
            size_t vend = vstart + vertexCount;

            addBatch(&default2DProgram,
                         textures.back(),
                         static_cast<GLuint>(offset),
                         static_cast<GLsizei>(elemCount));

            indexBuffer.resize(offset + elemCount);
            vertexBuffer.resize(vend);

            float rangeX = path.boundRight() - path.boundLeft();
            float rangeY = path.boundBottom() - path.boundTop();

            for (size_t i = 0; i < subpaths.size(); i++)
            {
                auto &pts = subpaths.points(i);

                for (size_t j = vstart; j < vend; ++j)
                {
                    vertexBuffer[j].pos.x = pts.x(j);
                    vertexBuffer[j].pos.y = pts.y(j);
                    vertexBuffer[j].tcoord.s = static_cast<uint16_t>(((((pts.x(j) - path.boundLeft()) / rangeX) * 16.0f) / detail::gfxStates.maxTexSize) * 0xFFFF);
                    vertexBuffer[j].tcoord.t = static_cast<uint16_t>(((((pts.y(j) - path.boundTop()) / rangeY) * 16.0f) / detail::gfxStates.maxTexSize) * 0xFFFF);
                    vertexBuffer[j].color = fillStyle.innerColor();

                    if (j > 2)
                    {
                        indexBuffer[offset++] = static_cast<uint16_t>(vstart);
                        indexBuffer[offset++] = static_cast<uint16_t>(j - 1);
                    }
                    indexBuffer[offset++] = static_cast<uint16_t>(j);
                }
            }
        }
        else
        {
            // TODO : Concave shape by triangulation using libtess2
        }

    }

};

}
}

#endif // TUNISCONTEXTDATA_H
